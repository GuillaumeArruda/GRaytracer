#include "gmath/internal/vector_impl.hpp"
#include "gmath/vec3f.hpp"

 namespace gmath
 {
	struct vec4f;
    template<>
    struct vector_typetraits<vec4f>
    {
       using value_type = float;
	   using denormalazing_operation_return_type = vec3f;
       static constexpr std::size_t size = 4; 
    };

    struct vec4f : vector_impl<vec4f, false>
    {
        using super = vector_impl<vec4f, false>;
        using value_type = super::value_type;

        vec4f(no_init_t) noexcept {}
        vec4f(value_type x = {}, value_type y = {}, value_type z = {}, value_type w = {}) noexcept : m_values{x,y,z,w} {}
		vec4f(vec3f const& copy) : m_values{ copy[0], copy[1], copy[2], 1.f } {}


        vec4f(vec4f const&) = default;
        vec4f& operator=(vec4f const&) = default;
        vec4f(vec4f&&) = default;
        vec4f& operator=(vec4f&&) = default;

        ~vec4f() = default;

		operator vec3f() const 
		{
			return vec3f(m_values[0], m_values[1], m_values[2]);
		}

        constexpr value_type& operator[](std::size_t index) noexcept { return m_values[index]; }
        constexpr value_type operator[](std::size_t index) const noexcept { return m_values[index]; }

        constexpr value_type x() const noexcept { return m_values[0]; }
        constexpr value_type y() const noexcept { return m_values[1]; }
        constexpr value_type z() const noexcept { return m_values[2]; }
        constexpr value_type w() const noexcept { return m_values[3]; }

        constexpr value_type& x() noexcept { return m_values[0]; }
        constexpr value_type& y() noexcept { return m_values[1]; }
        constexpr value_type& z() noexcept { return m_values[2]; }
        constexpr value_type& w() noexcept { return m_values[3]; }

        private:
		static_assert(super::type_traits::size == 4, "gmath::vec4f expect a size 4 type traits");
        value_type m_values[type_traits::size];
    };
 }
