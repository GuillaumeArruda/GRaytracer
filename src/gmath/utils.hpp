#pragma once
#include <type_traits>
#include <limits>

namespace gmath
{
    struct world_space {};
    struct model_space {};
    struct camera_space {};
    struct projection_space {};

	static constexpr float pi = 3.14159265358979323846f;
	static constexpr float pi_2 = pi / 2.f;
	static constexpr float pi_4 = pi / 4.f;

	enum class no_init_t
	{
		no_init
	};


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
}