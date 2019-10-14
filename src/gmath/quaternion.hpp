#pragma once
#include "gmath/vec4f.hpp"
#include "gmath/yprangles.hpp"

namespace gmath
{
	struct quaternion
	{
		using value_type = vec4f::value_type;
		static constexpr quaternion identity() { return quaternion(); }

		constexpr quaternion() noexcept : m_storage(0.f, 0.f, 0.f, 1.f) {}
		explicit quaternion(yprangles const& angles) noexcept;

		~quaternion() noexcept = default;
		constexpr quaternion(quaternion const&) noexcept = default;
		constexpr quaternion& operator=(quaternion const&) noexcept = default;
		constexpr quaternion(quaternion&&) noexcept = default;
		constexpr quaternion& operator=(quaternion&&) noexcept = default;

		constexpr value_type x() const noexcept { return m_storage.x(); }
		constexpr value_type y() const noexcept { return m_storage.y(); }
		constexpr value_type z() const noexcept { return m_storage.z(); }
		constexpr value_type w() const noexcept { return m_storage.w(); }

		constexpr explicit operator normalized_vec3f() const noexcept { return (*this) * normalized_vec3f::forward(); }
		explicit operator yprangles() const noexcept;

		constexpr quaternion inverse() const noexcept;

		constexpr quaternion operator-() const noexcept;
		constexpr quaternion& operator*=(quaternion const& rhs) noexcept;

		friend vec3f operator*(quaternion const& quat, vec3f const& vec) noexcept;
		friend constexpr quaternion operator*(quaternion const& lhs, quaternion const& rhs) noexcept;

		friend constexpr bool operator==(quaternion const& lhs, quaternion const& rhs)noexcept { return lhs.m_storage == rhs.m_storage; }
		friend constexpr bool operator!=(quaternion const& lhs, quaternion const& rhs)noexcept { return lhs.m_storage != rhs.m_storage; }

		friend constexpr bool almost_equal(quaternion const& lhs, quaternion const& rhs, int ulp = 1) noexcept
		{
			return almost_equal(lhs.m_storage, rhs.m_storage, ulp);
		}

		friend constexpr bool within_epsilon(quaternion const& lhs, quaternion const& rhs, quaternion::value_type epsilon = quaternion::value_type{ 1e-6f }) noexcept
		{
			return within_epsilon(lhs.m_storage, rhs.m_storage, epsilon);
		}

	private:
		constexpr quaternion(value_type x, value_type y, value_type z, value_type w) : m_storage(x, y, z, w) {}
		vec4f m_storage;
	};
}