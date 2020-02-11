#pragma once
#include <type_traits>
#include <limits>
#include <optional>
#include <cmath>

#include <glm/glm.hpp>
#include <cereal/cereal.hpp>

namespace gmath
{
    struct world_space {};
    struct model_space {};
    struct camera_space {};
    struct projection_space {};

	static constexpr float pi = 3.14159265358979323846f;
	static constexpr float pi_2 = pi / 2.f;
	static constexpr float pi_4 = pi / 4.f;

	struct no_init_t {};
	static constexpr no_init_t no_init;

	// Taken from https://en.cppreference.com/w/cpp/types/numeric_limits
	template<class T>
	constexpr auto almost_equal(T x, T y, int ulp = 1) noexcept -> typename std::enable_if_t<std::numeric_limits<T>::is_iec559, bool>
	{
		// the machine epsilon has to be scaled to the magnitude of the values used
		// and multiplied by the desired precision in ULPs (units in the last place)
		return std::abs(x - y) <= std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
			// unless the result is subnormal
			|| std::abs(x - y) < std::numeric_limits<T>::min();
	}

	template<class T>
	constexpr auto within_epsilon(T x, T y, T epsilon = 1e-6f) noexcept -> typename std::enable_if_t<std::numeric_limits<T>::is_iec559, bool>
	{
		return std::abs(x - y) <= epsilon;
	} 

	struct quadatric_result
	{
		float m_first_t = 0.f;
		float m_second_t = 0.f;
	};

	inline std::optional<quadatric_result> quadatric(float a, float b, float c) noexcept
	{
		float const discriminant = b * b - a * c * 4;
		if (discriminant <= 0.f)
			return {};
		float const sqrt_discriminant = std::sqrt(discriminant);
		float const q = b < 0 ? -0.5f * (b - sqrt_discriminant) : -0.5f * (b + sqrt_discriminant);
		float const t0 = q / a;
		float const t1 = c / q;
		if (t0 < t1)
			return quadatric_result{ t0, t1 };
		else
			return quadatric_result{ t1, t0 };
	}
}

namespace glm
{
	template<typename Archive>
	void serialize(Archive& ar, vec3& vec)
	{
		ar(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z));
	}

	template<typename Archive>
	void serialize(Archive& ar, vec4& vec)
	{
		ar(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z), cereal::make_nvp("w", vec.w));
	}

	template<typename Archive>
	void serialize(Archive& ar, mat4& mat)
	{
		ar(cereal::make_nvp("[0]", mat[0]), cereal::make_nvp("[1]", mat[1]), cereal::make_nvp("[2]", mat[2]), cereal::make_nvp("[3]", mat[3]));
	}
}
