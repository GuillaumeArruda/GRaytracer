#include "stdafx.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fmt/format.h>

#include "gmath/vector.h"
#include "gscene/resources/mesh_resource.h"

#include "gserializer/serializer.h"

GSERIALIZER_DEFINE_SUBCLASS_FACTORY_REGISTRATION(gscene::mesh_resource);

namespace gscene
{
    void mesh_resource::process(gserializer::serializer& serializer)
    {
        serializer.process("m_filepath", m_filepath);
    }

    void gscene::mesh_resource::load()
    {
        uint32_t const post_process_flag = aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FixInfacingNormals | aiProcess_SplitLargeMeshes | aiProcess_ImproveCacheLocality | aiProcess_PreTransformVertices;
        Assimp::Importer importer;
        importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, 33 );
        aiScene const* scene = importer.ReadFile(m_filepath.c_str(), post_process_flag);
        if (!scene)
        {
            throw std::runtime_error(fmt::format("Trying to load a mesh from a file that doesnt exist: {0}", m_filepath));
        }

        if (scene->mNumMeshes == 0)
        {
            throw std::runtime_error(fmt::format("Trying to load a scene without a mesh inside: {0}", m_filepath));
        }
        
        m_submeshes.reserve(scene->mNumMeshes);
        for (std::size_t mesh_id = 0; mesh_id < scene->mNumMeshes; ++mesh_id)
        {
            aiMesh const& mesh = *scene->mMeshes[mesh_id];
            if (!mesh.HasPositions() || !mesh.HasNormals() || !mesh.HasFaces())
            {
                throw std::runtime_error(fmt::format("Trying to load a mesh that doesnt have all necessary info: {0}", m_filepath));
            }

            submesh& submesh = m_submeshes.emplace_back();
            submesh.m_vertices_position.reserve(mesh.mNumVertices);
            submesh.m_vertices_normal.reserve(mesh.mNumVertices);
            for (std::size_t i = 0; i < mesh.mNumVertices; ++i)
            {
                auto const& vertice = mesh.mVertices[i];
                submesh.m_vertices_position.emplace_back(vertice.x, vertice.y, vertice.z);
                auto const& normal = mesh.mNormals[i];
                submesh.m_vertices_normal.emplace_back(normal.x, normal.y, normal.z);
            }

            submesh.m_faces_index.reserve(mesh.mNumFaces);
            submesh.m_edges.reserve(static_cast<std::size_t>(mesh.mNumFaces) * 3);
            for (std::size_t i = 0; i < mesh.mNumFaces; ++i)
            {
                auto const& triangle_face = mesh.mFaces[i];
                submesh.m_faces_index.emplace_back(triangle_face.mIndices[0], triangle_face.mIndices[1], triangle_face.mIndices[2], submesh.m_vertices_position);
                gmath::position const v0 = submesh.m_vertices_position[triangle_face.mIndices[0]];
                gmath::position const v1 = submesh.m_vertices_position[triangle_face.mIndices[1]];
                gmath::position const v2 = submesh.m_vertices_position[triangle_face.mIndices[2]];
                submesh.m_edges.push_back(v1 - v0);
                submesh.m_edges.push_back(v2 - v1);
                submesh.m_edges.push_back(v0 - v2);
            }
        }
        
    }
    
    mesh_resource::face::face(std::uint32_t v0, std::uint32_t v1, std::uint32_t v2, std::vector<gmath::position<gmath::model_space>> const& vertices)
        : m_indices{ v0,v1,v2 }
        , m_normal(compute_normal(vertices[m_indices[0]], vertices[m_indices[1]], vertices[m_indices[2]]))
        , m_area2(m_normal.length2())
        , m_d(-m_normal.dot(gmath::vector(vertices[m_indices[0]])))
    {
    }

    gmath::vector<gmath::model_space> mesh_resource::face::compute_normal(gmath::position<gmath::model_space> const& v0, gmath::position<gmath::model_space> const& v1, gmath::position<gmath::model_space> const& v2)
    {
        gmath::vector const a = v1 - v0;
        gmath::vector const b = v2 - v0;
        return a.cross(b);
    }
}

