#include "Catch2/catch.hpp"
#include "gmath/vec3f.hpp"
#include <utility>

TEST_CASE("gmath.vec3f.construction", "[gmath::vec3f]")
{
	SECTION("Default Construction")
	{
		constexpr gmath::vec3f v;
		REQUIRE(v[0] == 0.f);
		REQUIRE(v[1] == 0.f);
		REQUIRE(v[2] == 0.f);
	}

	SECTION("Per Element construction")
	{
		constexpr gmath::vec3f v(1.f,2.f,3.f);
		REQUIRE(v[0] == 1.f);
		REQUIRE(v[1] == 2.f);
		REQUIRE(v[2] == 3.f);
	}

	SECTION("Copy construction")
	{
		constexpr gmath::vec3f v1(1.f, 2.f, 3.f);
		constexpr gmath::vec3f v(v1);
		REQUIRE(v[0] == 1.f);
		REQUIRE(v[1] == 2.f);
		REQUIRE(v[2] == 3.f);
	}

	SECTION("Move construction")
	{
		gmath::vec3f v1(1.f, 2.f, 3.f);
		gmath::vec3f v(std::move(v1));
		REQUIRE(v[0] == 1.f);
		REQUIRE(v[1] == 2.f);
		REQUIRE(v[2] == 3.f);
	}
}

TEST_CASE("gmath.vec3f.comparison", "[gmath::vec3f]")
{
	SECTION("Equality")
	{
		gmath::vec3f v;
		gmath::vec3f v1;
		REQUIRE(v == v1);
		REQUIRE(v == v);

		v.x() = 1.f;
		v.y() = 2.f;
		v.z() = 3.f;
		REQUIRE_FALSE(v == v1);

		v1.x() = 1.f;
		v1.y() = 2.f;
		v1.z() = 3.f;
		REQUIRE(v == v1);
	}

	SECTION("Inequality")
	{
		gmath::vec3f v;
		gmath::vec3f v1;
		REQUIRE_FALSE(v != v1);

		v.x() = 1.f;
		v.y() = 2.f;
		v.z() = 3.f;
		REQUIRE(v != v1);

		v1.x() = 1.f;
		v1.y() = 2.f;
		v1.z() = 3.f;
		REQUIRE_FALSE(v != v1);
	}
}

TEST_CASE("gmath.vec3f.addition", "[gmath::vec3f]")
{
	SECTION("Empty vector")
	{
		gmath::vec3f v;
		constexpr gmath::vec3f v1;
		v += v1;
		REQUIRE(v == v1);
	}

	SECTION("+= operator")
	{
		gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(1.f,2.f,3.f);
		v += v1;
		REQUIRE(v[0] == 11.f);
		REQUIRE(v[1] == 22.f);
		REQUIRE(v[2] == 33.f);
	}

	SECTION("+ operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(1.f, 2.f, 3.f);
		constexpr gmath::vec3f v2 = v + v1;
		REQUIRE(v2[0] == 11.f);
		REQUIRE(v2[1] == 22.f);
		REQUIRE(v2[2] == 33.f);
	}
}

TEST_CASE("gmath.vec3f.subtraction", "[gmath::vec3f]")
{
	SECTION("Empty vector")
	{
		gmath::vec3f v;
		constexpr gmath::vec3f v1;
		v -= v1;
		REQUIRE(v == v1);
	}

	SECTION("-= operator")
	{
		gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(1.f, 2.f, 3.f);
		v -= v1;
		REQUIRE(v[0] == 9.f);
		REQUIRE(v[1] == 18.f);
		REQUIRE(v[2] == 27.f);
	}

	SECTION("- operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(1.f, 2.f, 3.f);
		constexpr gmath::vec3f v2 = v - v1;
		REQUIRE(v2[0] == 9.f);
		REQUIRE(v2[1] == 18.f);
		REQUIRE(v2[2] == 27.f);
	}
}

TEST_CASE("gmath.vec3f.multiplication", "[gmath::vec3f]")
{
	SECTION("Zero multiplication")
	{
		gmath::vec3f v(1.f,1.f,1.f);
		v *= 0.f;
		REQUIRE(v == gmath::vec3f());
	}

	SECTION("*= operator")
	{
		gmath::vec3f v(10.f, 20.f, 30.f);
		v *= 10;
		REQUIRE(v[0] == 100.f);
		REQUIRE(v[1] == 200.f);
		REQUIRE(v[2] == 300.f);
	}

	SECTION("* operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1 = v * 5;
		REQUIRE(v1[0] == 50.f);
		REQUIRE(v1[1] == 100.f);
		REQUIRE(v1[2] == 150.f);
	}
}

TEST_CASE("gmath.vec3f.division", "[gmath::vec3f]")
{
	SECTION("/= operator")
	{
		gmath::vec3f v(10.f, 20.f, 30.f);
		v /= 2;
		REQUIRE(v[0] == 5.f);
		REQUIRE(v[1] == 10.f);
		REQUIRE(v[2] == 15.f);
	}

	SECTION("/ operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1 = v / 5;
		REQUIRE(v1[0] == 2.f);
		REQUIRE(v1[1] == 4.f);
		REQUIRE(v1[2] == 6.f);
	}
}

TEST_CASE("gmath.vec3f.length", "[gmath::vec3f]")
{
	SECTION("empty length_squared")
	{
		constexpr gmath::vec3f v;
		REQUIRE(v.length_squared() == 0.f);
	}

	SECTION("length_squared")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		REQUIRE(v.length_squared() == 1400.f);
	}

	SECTION("empty length")
	{
		constexpr gmath::vec3f v;
		REQUIRE(v.length() == 0.f);
	}

	SECTION("length")
	{
		constexpr gmath::vec3f v(2.f,2.f,3.f);
		REQUIRE(v.length() == 4.123105617f);
	}
}

TEST_CASE("gmath.vec3f.normalize", "[gmath::vec3f]")
{
	SECTION("normalize already normalized")
	{
		gmath::vec3f v(1.f, 0.f, 0.f);
		REQUIRE(v.is_normalized());
		v.normalize();
		REQUIRE(v.is_normalized());
		REQUIRE(v.length_squared() == 1.f);
		REQUIRE(v[0] == 1.f);
		REQUIRE(v[1] == 0.f);
		REQUIRE(v[2] == 0.f);
	}

	SECTION("normalize")
	{
		gmath::vec3f v(10.f, 20.f, 30.f);
		REQUIRE_FALSE(v.is_normalized());
		v.normalize();
		REQUIRE(v.length_squared() == 1.f);
		REQUIRE(v.x() == 0.267261237f);
		REQUIRE(v.y() == 0.534522475f);
		REQUIRE(v.z() == 0.801783741f);
	}
}

TEST_CASE("gmath.vec3f.negation", "[gmath::vec3f]")
{
	SECTION("empty vector")
	{
		constexpr gmath::vec3f v;
		REQUIRE(v == -v);
	}

	SECTION("negate")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(-v);
		REQUIRE(v != v1);
		REQUIRE(v1.x() == -10.f);
		REQUIRE(v1.y() == -20.f);
		REQUIRE(v1.z() == -30.f);
	}
}

TEST_CASE("gmath.vec3f.dot", "[gmath::vec3f]")
{
	SECTION("empty vector")
	{
		constexpr gmath::vec3f v;
		constexpr gmath::vec3f v1;
		REQUIRE(v.dot(v1) == 0.f);
	}

	SECTION("dot product")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(1.f,1.f,1.f);
		REQUIRE(v.dot(v1) == v1.dot(v));
		REQUIRE(v.dot(v1) == 60.f);
	}
}

TEST_CASE("gmath.vec3f.cross", "[gmath::vec3f]")
{
	SECTION("empty vector")
	{
		constexpr gmath::vec3f v;
		constexpr gmath::vec3f v1;
		REQUIRE(v.cross(v1) == v);
	}

	SECTION("cross product")
	{
		constexpr gmath::vec3f v(1.f, 0.f, 0.f);
		constexpr gmath::vec3f v1(0.f, 1.f, 0.f);
		REQUIRE(v.cross(v1) == gmath::vec3f(0.f,0.f,1.f));
		REQUIRE(v1.cross(v) == gmath::vec3f(0.f, 0.f, -1.f));
	}
}