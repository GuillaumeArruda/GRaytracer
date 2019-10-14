#include "gmath/quaternion.hpp"

namespace gmath
{
	quaternion::quaternion(yprangles const& angles) noexcept : m_storage(no_init_t::no_init)
	{
		const value_type halfYaw = angles.yaw().value() * 0.5f;
		const value_type halfPitch = angles.pitch().value() * 0.5f;
		const value_type halfRoll = angles.roll().value() * 0.5f;
		const value_type cy = std::cos(halfYaw);
		const value_type sy = std::sin(halfYaw);
		const value_type cp = std::cos(halfPitch);
		const value_type sp = std::sin(halfPitch);
		const value_type cr = std::cos(halfRoll);
		const value_type sr = std::sin(halfRoll);

		m_storage.w() = cy * cp * cr + sy * sp * sr;
		m_storage.x() = cy * cp * sr - sy * sp * cr;
		m_storage.y() = sy * cp * sr + cy * sp * cr;
		m_storage.z() = sy * cp * cr - cy * sp * sr;
	}

	quaternion::operator yprangles() const noexcept
	{
		const value_type x2 = x() * x();
		const value_type y2 = y() * y();
		const value_type z2 = z() * z();
		const value_type sinp = 2.f * (w() * y() - z() * x());
		const value_type pitch = std::abs(sinp) >= 1.f ? std::copysign(pi_2, sinp) : std::asin(sinp);

		const value_type siny_cosp = 2.f * (w() * z() + y() * x());
		const value_type cosy_cosp = 1.f - 2.f * (y2 + z2);
		const value_type yaw = std::atan2(siny_cosp, cosy_cosp);

		const value_type sinr_cosp = 2.f * (w() * x() + y() * z());
		const value_type cosr_cosp = 1.f - 2.f * (x2 + y2);
		const value_type roll = std::atan2(sinr_cosp, cosr_cosp);

		return yprangles(radian(yaw), radian(pitch), radian(roll));
	}

	constexpr quaternion quaternion::inverse() const noexcept
	{
		return -(*this);
	}

	constexpr quaternion& quaternion::operator*=(quaternion const& rhs) noexcept
	{
		const vec3f lhsvec3 = m_storage;
		const vec3f rhsvec3 = rhs.m_storage;
		const vec3f resultVec3 = lhsvec3.cross(rhsvec3) + w() * rhsvec3 + rhs.w() * lhsvec3;
		const value_type resultW = w() * rhs.w() - lhsvec3.dot(rhsvec3);
		*this = quaternion(resultVec3[0], resultVec3[1], resultVec3[2], resultW);
		return *this;
	}

	constexpr quaternion quaternion::operator-() const noexcept
	{
		return quaternion(-x(), -y(), -z(), w());
	}

	constexpr quaternion operator*(quaternion const& lhs, quaternion const& rhs) noexcept
	{
		quaternion result(lhs);
		return result *= rhs;
	}

	vec3f operator*(quaternion const& quat, vec3f const& vec) noexcept
	{
		const quaternion v(vec.x(), vec.y(), vec.z(), 0.f);
		const quaternion quat_inverse = quat.inverse();
		const quaternion result = quat * v * quat_inverse;
		return result.m_storage;
	}

}
