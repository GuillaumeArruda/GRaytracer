#include "Catch2/catch.hpp"

#include "gmath/units.hpp"
#include "static_assert_helper.hpp"

TEST_CASE("gmath.degree.construction", "[gmath::degree]")
{
	SECTION("Default Construction")
	{
		constexpr gmath::degree d;
		REQUIRE(d.value() == 0.f);
	}

	SECTION("Value Initialization Construction")
	{

		constexpr gmath::degree d(10.f);
		constexpr gmath::degree d2 = 10.0_d;
		REQUIRE(d.value() == 10.f);
		REQUIRE(d.value() == d2.value());
		REQUIRE(d == 10.0_d);
		REQUIRE(d == d2);
	}
}

TEST_CASE("gmath.degree.addition", "[gmath::degree]")
{
	constexpr gmath::degree d(10.f);
	constexpr gmath::degree d2(1.f);
	REQUIRE(d + d2 == 11.0_d);
}

TEST_CASE("gmath.degree.subtraction", "[gmath::degree]")
{
	constexpr gmath::degree d(10.f);
	constexpr gmath::degree d2(1.f);
	REQUIRE(d - d2 == 9.0_d);
}

TEST_CASE("gmath.degree.division", "[gmath::degree]")
{
	constexpr gmath::degree d(10.f);
	constexpr gmath::degree d2(2.f);
	REQUIRE(d / d2 == 5.0_d);
}

TEST_CASE("gmath.degree.multiplication", "[gmath::degree]")
{
	constexpr gmath::degree d(10.f);
	constexpr gmath::degree d2(2.f);
	REQUIRE(d * d2 == 20.0_d);
}

TEST_CASE("gmath.degree.comparaison", "[gmath::degree]")
{
	constexpr gmath::degree d(10.f);
	constexpr gmath::degree d2(2.f);
	REQUIRE_FALSE(d == d2);
	REQUIRE_FALSE(d < d2);
	REQUIRE_FALSE(d <= d2);
	REQUIRE(d != d2);
	REQUIRE(d > d2);
	REQUIRE(d >= d2);
}

TEST_CASE("gmath.degree.radian_transformation", "[gmath::degree]")
{
	constexpr gmath::degree d(10.f);
	constexpr gmath::degree d2(2.f);
	constexpr gmath::radian r(d);
	constexpr gmath::radian r2(d2);
	REQUIRE(gmath::radian(d) == r);
	REQUIRE(gmath::radian(d2) == r2);
	constexpr gmath::degree d3(r);
	REQUIRE(gmath::almost_equal(d3.value(),d.value()));
}

TEST_CASE("gmath.degree.static_assert", "[gmath::degree]")
{
	trivial_static_assert<gmath::degree>();
}