#include <cmath>
#include "gmath/utils.hpp"

namespace gmath
{
    template<class T, bool UseSIMD>
    struct vec_impl;

    template<class T>
    struct vector_typetraits;

    template<class T>
    struct vec_impl<T, false>
    {
        using type_traits = vector_typetraits<T>;
        using value_type = typename type_traits::value_type;

		constexpr T operator-() const noexcept
        {
            const T& self = selff();
            T result;
            for(std::size_t i = 0; i < type_traits::size; ++i)
                result[i] = -self[i];
            return result;
        }

		constexpr T& operator+=(T const& rhs) noexcept
        {
            T& self = selff();
            for(std::size_t i = 0; i < type_traits::size; ++i)
                self[i] += rhs[i];
            return self;
        }

		constexpr T& operator-=(T const& rhs) noexcept
        {
            T& self = selff();
            for(std::size_t i = 0; i < type_traits::size; ++i)
                self[i] -= rhs[i];
            return self;
        }

		constexpr T& operator/=(value_type const& rhs) noexcept
        {
            T& self = selff();
            for(std::size_t i = 0; i < type_traits::size; ++i)
                self[i] /= rhs;
            return self;
        }

		constexpr T& operator*=(value_type const& rhs) noexcept
        {
            T& self = selff();
            for(std::size_t i = 0; i < type_traits::size; ++i)
                self[i] *= rhs;
            return self;
        }

        constexpr value_type length_squared() const noexcept
        {
            T const& self = selff();
            value_type result = 0.f;
            for(std::size_t i = 0; i < type_traits::size; ++i)
                result += self[i] * self[i];
            return result;
        }

        constexpr value_type length() const noexcept
        {
            return std::sqrt(length_squared());
        }

		bool is_normalized() const noexcept
		{
			return almost_equal(1.f, length_squared());
		}

         constexpr void normalize() noexcept
         {
             const value_type vecLength = length();
             *this /= vecLength;
         }

		 constexpr value_type dot(T const& rhs) const noexcept
		 {
			 T const& self = selff();
			 value_type result = 0.f;
			 for (std::size_t i = 0; i < type_traits::size; ++i)
				 result += self[i] * rhs[i];
			 return result;
		 }

		 friend constexpr T operator+(T const& lhs, T const& rhs) noexcept
		 {
			 T result(lhs);
			 return result += rhs;
		 }

		 friend constexpr T operator-(T const& lhs, T const& rhs) noexcept
		 {
			 T result(lhs);
			 return result -= rhs;
		 }

		 friend constexpr T operator/(T const& lhs, value_type const& rhs) noexcept
		 {
			 T result(lhs);
			 return result /= rhs;
		 }

		 friend constexpr T operator*(T const& lhs, value_type const& rhs) noexcept
		 {
			 T result(lhs);
			 return result *= rhs;
		 }

		 friend constexpr bool operator==(T const& lhs, T const& rhs) noexcept
		 {
			 bool areEqual = true;
			 for (std::size_t i = 0; i < type_traits::size; ++i)
				 areEqual = areEqual && (lhs[i] == rhs[i]);
			 return areEqual;
		 }

		 friend constexpr bool operator!=(T const& lhs, T const& rhs) noexcept
		 {
			 return !(lhs == rhs);
		 }

        private:
        constexpr T& selff() noexcept { return static_cast<T&>(*this); }
		constexpr const T& selff() const noexcept { return static_cast<const T&>(*this); }
    };
}