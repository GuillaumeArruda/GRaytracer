#include "Catch2/catch.hpp"

#include "gmath/units.hpp"
#include "static_assert_helper.hpp"

TEST_CASE("gmath.radian.construction", "[gmath::radian]")
{
	SECTION("Default Construction")
	{
		constexpr gmath::radian r;
		REQUIRE(r.value() == 0.f);
	}

	SECTION("Value Initialization Construction")
	{

		constexpr gmath::radian r(10.f);
		constexpr gmath::radian r2 = 10.0_r;
		REQUIRE(r.value() == 10.f);
		REQUIRE(r.value() == r2.value());
		REQUIRE(r == 10.0_r);
		REQUIRE(r == r2);
	}
}

TEST_CASE("gmath.radian.addition", "[gmath::radian]")
{
	constexpr gmath::radian r(10.f);
	constexpr gmath::radian r2(1.f);
	REQUIRE(r + r2 == 11.0_r);
}

TEST_CASE("gmath.radian.subtraction", "[gmath::radian]")
{
	constexpr gmath::radian r(10.f);
	constexpr gmath::radian r2(1.f);
	REQUIRE(r - r2 == 9.0_r);
}

TEST_CASE("gmath.radian.division", "[gmath::radian]")
{
	constexpr gmath::radian r(10.f);
	constexpr gmath::radian r2(2.f);
	REQUIRE(r / r2 == 5.0_r);
}

TEST_CASE("gmath.radian.multiplication", "[gmath::radian]")
{
	constexpr gmath::radian r(10.f);
	constexpr gmath::radian r2(2.f);
	REQUIRE(r * r2 == 20.0_r);
}

TEST_CASE("gmath.radian.comparaison", "[gmath::radian]")
{
	constexpr gmath::radian r(10.f);
	constexpr gmath::radian r2(2.f);
	REQUIRE_FALSE(r == r2);
	REQUIRE_FALSE(r < r2);
	REQUIRE_FALSE(r <= r2);
	REQUIRE(r != r2);
	REQUIRE(r > r2);
	REQUIRE(r >= r2);
}

TEST_CASE("gmath.radian.static_assert", "[gmath::radian]")
{
	trivial_static_assert<gmath::radian>();
}