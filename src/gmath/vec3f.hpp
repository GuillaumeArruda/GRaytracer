#include "gmath/internal/vector_impl.hpp"

 namespace gmath
 {
	struct vec3f;
    template<>
    struct vector_typetraits<vec3f>
    {
       using value_type = float;
       static constexpr std::size_t size = 3; 
    };

    struct vec3f : vector_impl<vec3f, false>
    {
        using super = vector_impl<vec3f, false>;
        using value_type = super::value_type;

        vec3f(no_init_t) noexcept {}
		constexpr vec3f(value_type x = {}, value_type y = {}, value_type z = {}) noexcept : m_values{x,y,z} {}
		 
        constexpr vec3f(vec3f const&) = default;
        constexpr vec3f& operator=(vec3f const&) = default;
        constexpr vec3f(vec3f&&) = default;
        constexpr vec3f& operator=(vec3f&&) = default;

        ~vec3f() = default;

		constexpr value_type& operator[](std::size_t index) noexcept { return m_values[index]; }
        constexpr const value_type operator[](std::size_t index) const noexcept { return m_values[index]; }

        constexpr value_type x() const noexcept { return m_values[0]; }
        constexpr value_type y() const noexcept { return m_values[1]; }
        constexpr value_type z() const noexcept { return m_values[2]; }

        constexpr value_type& x() noexcept { return m_values[0]; }
        constexpr value_type& y() noexcept { return m_values[1]; }
        constexpr value_type& z() noexcept { return m_values[2]; }


		constexpr vec3f cross(vec3f const& rhs) const
		{
			return vec3f((m_values[1] * rhs[2]) - (m_values[2] * rhs[1])
						, -(m_values[0] * rhs[2]) + (m_values[2] * rhs[0])
						, (m_values[0] * rhs[1]) - (m_values[1] * rhs[0]));
		}

        private:
        value_type m_values[3];
    };


	struct normalized_vec3f;
	template<>
	struct vector_typetraits<normalized_vec3f>
	{
		using value_type = float;
		static constexpr std::size_t size = 3;
		
	};

	struct normalized_vec3f : vec3f
	{
		constexpr normalized_vec3f(vec3f const& vec) noexcept : vec3f(vec) { normalize(); }

		constexpr normalized_vec3f(normalized_vec3f const&) noexcept = default;
		constexpr normalized_vec3f& operator=(normalized_vec3f const&) noexcept = default;
		constexpr normalized_vec3f(normalized_vec3f&&) noexcept = default;
		constexpr normalized_vec3f& operator=(normalized_vec3f&&) noexcept = default;

		~normalized_vec3f() = default;

		using vec3f::cross;
		constexpr normalized_vec3f cross(normalized_vec3f const& rhs) const noexcept 
		{
			return normalized_vec3f(vec3f::cross(rhs), garanty_normal_t::garanty_normal);
		}
		constexpr value_type x() const noexcept { return vec3f::x(); }
		constexpr value_type y() const noexcept { return vec3f::y(); }
		constexpr value_type z() const noexcept { return vec3f::z(); }
		constexpr value_type x() noexcept { return vec3f::x(); }
		constexpr value_type y() noexcept { return vec3f::y(); }
		constexpr value_type z() noexcept { return vec3f::z(); }
		constexpr const value_type operator[](std::size_t index) const noexcept { return vec3f::operator[](index); }
		constexpr const value_type operator[](std::size_t index) noexcept { return vec3f::operator[](index); }

	private:
		using vec3f::normalize;
		using vec3f::operator+=;
		using vec3f::operator-=;
		using vec3f::operator/=;
		using vec3f::operator*=;
		enum class garanty_normal_t
		{
			garanty_normal
		};
		constexpr normalized_vec3f(vec3f const& vec, garanty_normal_t) noexcept : vec3f(vec) {}
	};
 }