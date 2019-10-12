#pragma once
#include "gmath/internal/vec_impl.hpp"

#include "gmath/units.hpp"

namespace gmath
{
	struct hpbangles;
	template<>
	struct vector_typetraits<hpbangles>
	{
		using value_type = radian;
		static constexpr std::size_t size = 3;
	};

	struct hpbangles
		: vec_impl<hpbangles, false>
	{
		using super = vec_impl<hpbangles, false>;
		using value_type = super::value_type;

		constexpr hpbangles(radian x = 0._r, radian y = 0._r, radian z = 0._r) noexcept : m_values{ x,y,z } {}
		constexpr hpbangles(degree x, degree y = 0._d, degree z = 0._d) noexcept : m_values {radian(x), radian(y), radian(z) } {}

		constexpr hpbangles(hpbangles const&) = default;
		constexpr hpbangles& operator=(hpbangles const&) = default;
		constexpr hpbangles(hpbangles&&) = default;
		constexpr hpbangles& operator=(hpbangles&&) = default;

		~hpbangles() = default;

		constexpr radian& operator[](std::size_t index) noexcept { return m_values[index]; }
		constexpr const radian operator[](std::size_t index) const noexcept { return m_values[index]; }

		constexpr radian heading() const noexcept { return m_values[0]; }
		constexpr radian pitch() const noexcept { return m_values[1]; }
		constexpr radian bank() const noexcept { return m_values[2]; }

		constexpr radian& heading() noexcept { return m_values[0]; }
		constexpr radian& pitch() noexcept { return m_values[1]; }
		constexpr radian& bank() noexcept { return m_values[2]; }

	private:
		using super::dot;
		using super::is_normalized;
		using super::length;
		using super::length_squared;

		radian m_values[3];
	};
}