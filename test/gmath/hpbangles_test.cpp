#include <utility>

#include "Catch2/catch.hpp"

#include "gmath/yprangles.hpp"

#include "static_assert_helper.hpp"

TEST_CASE("gmath.hpbangles.construction", "[gmath::hpbangles]")
{
	SECTION("Default Construction")
	{
		constexpr gmath::yprangles v;
		REQUIRE(v[0] == 0._r);
		REQUIRE(v[1] == 0._r);
		REQUIRE(v[2] == 0._r);
	}

	SECTION("Per Element construction")
	{
		constexpr gmath::yprangles v(1.0_r, 2.0_r, 3.0_r);
		REQUIRE(v[0] == 1.0_r);
		REQUIRE(v[1] == 2.0_r);
		REQUIRE(v[2] == 3.0_r);
	}

	SECTION("Copy construction")
	{
		constexpr gmath::yprangles v1(1.0_r, 2.0_r, 3.0_r);
		constexpr gmath::yprangles v(v1);
		REQUIRE(v[0] == 1.0_r);
		REQUIRE(v[1] == 2.0_r);
		REQUIRE(v[2] == 3.0_r);
	}

	SECTION("Move construction")
	{
		gmath::yprangles v1(1.0_r, 2.0_r, 3.0_r);
		gmath::yprangles v(std::move(v1));
		REQUIRE(v[0] == 1.0_r);
		REQUIRE(v[1] == 2.0_r);
		REQUIRE(v[2] == 3.0_r);
	}
}

TEST_CASE("gmath.hpbangles.comparison", "[gmath::hpbangles]")
{
	SECTION("Equality")
	{
		gmath::yprangles v;
		gmath::yprangles v1;
		REQUIRE(v == v1);
		REQUIRE(v == v);

		v.yaw() = 1._r;
		v.pitch() = 2._r;
		v.roll() = 3._r;
		REQUIRE_FALSE(v == v1);

		v1.yaw() = 1._r;
		v1.pitch() = 2._r;
		v1.roll() = 3._r;
		REQUIRE(v == v1);
	}

	SECTION("Inequality")
	{
		gmath::yprangles v;
		gmath::yprangles v1;
		REQUIRE_FALSE(v != v1);

		v.yaw() = 1._r;
		v.pitch() = 2._r;
		v.roll() = 3._r;
		REQUIRE(v != v1);

		v1.yaw() = 1._r;
		v1.pitch() = 2._r;
		v1.roll() = 3._r;
		REQUIRE_FALSE(v != v1);
	}
}

TEST_CASE("gmath.hpbangles.addition", "[gmath::hpbangles]")
{
	SECTION("Empty vector")
	{
		gmath::yprangles v;
		constexpr gmath::yprangles v1;
		v += v1;
		REQUIRE(v == v1);
	}

	SECTION("+= operator")
	{
		gmath::yprangles v(10._r, 20._r, 30._r);
		constexpr gmath::yprangles v1(1._r, 2._r, 3._r);
		v += v1;
		REQUIRE(v[0] == 11._r);
		REQUIRE(v[1] == 22._r);
		REQUIRE(v[2] == 33._r);
	}

	SECTION("+ operator")
	{
		constexpr gmath::yprangles v(10._r, 20._r, 30._r);
		constexpr gmath::yprangles v1(1._r, 2._r, 3._r);
		gmath::yprangles v2 = v + v1;
		REQUIRE(v2[0] == 11._r);
		REQUIRE(v2[1] == 22._r);
		REQUIRE(v2[2] == 33._r);
	}
}

TEST_CASE("gmath.hpbangles.subtraction", "[gmath::hpbangles]")
{
	SECTION("Empty vector")
	{
		gmath::yprangles v;
		constexpr gmath::yprangles v1;
		v -= v1;
		REQUIRE(v == v1);
	}

	SECTION("-= operator")
	{
		gmath::yprangles v(10._r, 20._r, 30._r);
		constexpr gmath::yprangles v1(1._r, 2._r, 3._r);
		v -= v1;
		REQUIRE(v[0] == 9._r);
		REQUIRE(v[1] == 18._r);
		REQUIRE(v[2] == 27._r);
	}

	SECTION("- operator")
	{
		constexpr gmath::yprangles v(10._r, 20._r, 30._r);
		constexpr gmath::yprangles v1(1._r, 2._r, 3._r);
		gmath::yprangles v2 = v - v1;
		REQUIRE(v2[0] == 9._r);
		REQUIRE(v2[1] == 18._r);
		REQUIRE(v2[2] == 27._r);
	}
}

TEST_CASE("gmath.hpbangles.multiplication", "[gmath::hpbangles]")
{
	SECTION("Zero multiplication")
	{
		gmath::yprangles v(1._r, 1._r, 1._r);
		v *= 0._r;
		REQUIRE(v == gmath::yprangles());
	}

	SECTION("*= operator")
	{
		gmath::yprangles v(10._r, 20._r, 30._r);
		v *= 10._r;
		REQUIRE(v[0] == 100._r);
		REQUIRE(v[1] == 200._r);
		REQUIRE(v[2] == 300._r);
	}

	SECTION("* operator")
	{
		constexpr gmath::yprangles v(10._r, 20._r, 30._r);
		gmath::yprangles v1 = v * 5._r;
		REQUIRE(v1[0] == 50._r);
		REQUIRE(v1[1] == 100._r);
		REQUIRE(v1[2] == 150._r);
	}
}

TEST_CASE("gmath.hpbangles.division", "[gmath::hpbangles]")
{
	SECTION("/= operator")
	{
		gmath::yprangles v(10._r, 20._r, 30._r);
		v /= 2._r;
		REQUIRE(v[0] == 5._r);
		REQUIRE(v[1] == 10._r);
		REQUIRE(v[2] == 15._r);
	}

	SECTION("/ operator")
	{
		constexpr gmath::yprangles v(10._r, 20._r, 30._r);
		gmath::yprangles v1 = v / 5._r;
		REQUIRE(v1[0] == 2._r);
		REQUIRE(v1[1] == 4._r);
		REQUIRE(v1[2] == 6._r);
	}
}


TEST_CASE("gmath.hpbangles.negation", "[gmath::hpbangles]")
{
	SECTION("empty vector")
	{
		constexpr gmath::yprangles v;
		REQUIRE(v == -v);
	}

	SECTION("negate")
	{
		constexpr gmath::yprangles v(10._r, 20._r, 30._r);
		constexpr gmath::yprangles v1(-v);
		REQUIRE(v != v1);
		REQUIRE(v1[0] == -10._r);
		REQUIRE(v1[1] == -20._r);
		REQUIRE(v1[2] == -30._r);
	}
}

TEST_CASE("gmath.hpbangles.static_assert", "[gmath::hpbangles]")
{
	trivial_static_assert<gmath::yprangles>();
}