#pragma once
#include "gmath/internal/vec_impl.hpp"

#include "gmath/units.hpp"

namespace gmath
{
	struct yprangles;
	template<>
	struct vector_typetraits<yprangles>
	{
		using value_type = radian;
		static constexpr std::size_t size = 3;
	};

	struct yprangles
		: vec_impl<yprangles, false>
	{
		enum
		{
			Yaw,
			Pitch,
			Roll,
			Count,
		};
		using super = vec_impl<yprangles, false>;
		using value_type = super::value_type;

		constexpr yprangles(radian yaw = 0._r, radian pitch = 0._r, radian roll = 0._r) noexcept : m_values{ yaw, pitch, roll } {}

		constexpr yprangles(yprangles const&) = default;
		constexpr yprangles& operator=(yprangles const&) = default;
		constexpr yprangles(yprangles&&) = default;
		constexpr yprangles& operator=(yprangles&&) = default;

		~yprangles() = default;

		constexpr radian& operator[](std::size_t index) noexcept { return m_values[index]; }
		constexpr const radian operator[](std::size_t index) const noexcept { return m_values[index]; }

		constexpr radian yaw() const noexcept { return m_values[Yaw]; }
		constexpr radian pitch() const noexcept { return m_values[Pitch]; }
		constexpr radian roll() const noexcept { return m_values[Roll]; }

		constexpr radian& yaw() noexcept { return m_values[Yaw]; }
		constexpr radian& pitch() noexcept { return m_values[Pitch]; }
		constexpr radian& roll() noexcept { return m_values[Roll]; }

	private:
		using super::dot;
		using super::is_normalized;
		using super::length;
		using super::length_squared;

		radian m_values[3];
	};
}