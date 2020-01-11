#include "Catch2\catch.hpp"

#include "gmath/transform.hpp"
#include "gmath/position.hpp"
#include "gmath/vector.hpp"
#include "static_assert_helper.hpp"

TEST_CASE("gmath.transform.construction", "[gmath::transform]")
{
    SECTION("Default Construction")
    {
        constexpr gmath::transform<gmath::world_space, gmath::model_space> world_from_model;
        constexpr gmath::transform<gmath::camera_space, gmath::world_space> camera_from_world;
        constexpr gmath::transform<gmath::projection_space, gmath::camera_space> perspective_from_camera;
        gmath::transform<gmath::projection_space, gmath::model_space> mvp = perspective_from_camera * camera_from_world * world_from_model;
        constexpr gmath::position<gmath::model_space> model_space_pos;
        gmath::position<gmath::projection_space> projection_space_pos = mvp * model_space_pos;
        gmath::vector<gmath::projection_space> vec = projection_space_pos - gmath::position<gmath::projection_space>();
        constexpr gmath::direction up = gmath::direction<gmath::model_space>::up();
        REQUIRE(world_from_model == world_from_model);
    }

}

TEST_CASE("gmath.transform.static_assert", "[gmath::transform]")
{
    trivial_static_assert<gmath::transform<gmath::model_space, gmath::world_space>>();
    trivial_static_assert<gmath::position<gmath::model_space>>();
}