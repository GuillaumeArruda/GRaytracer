#include "Catch2/catch.hpp"
#include "gmath/vec3f.hpp"
#include <utility>

TEST_CASE("gmath.normalized_vec3f.construction", "[gmath::normalized_vec3f]")
{

	SECTION("From vec3")
	{
		constexpr gmath::vec3f v(10.f, 0.f, 0.f);
		gmath::normalized_vec3f nv(v);
		REQUIRE(nv[0] == 1.f);
		REQUIRE(nv[1] == 0.f);
		REQUIRE(nv[2] == 0.f);
	}

	SECTION("Copy construction")
	{
		constexpr gmath::vec3f v(0.f, 10.f, 0.f);
		gmath::normalized_vec3f nv(v);
		gmath::normalized_vec3f nv1(nv);
		REQUIRE(nv1[0] == nv[0]);
		REQUIRE(nv1[1] == nv[1]);
		REQUIRE(nv1[2] == nv[2]);
	}

	SECTION("Assignement")
	{
		constexpr gmath::vec3f v(0.f, 0.f, 10.f);
		gmath::normalized_vec3f nv(v);
		nv = v;
		REQUIRE(nv[0] == 0.f);
		REQUIRE(nv[1] == 0.f);
		REQUIRE(nv[2] == 1.f);
	}
}

TEST_CASE("gmath.normalized_vec3f.addition", "[gmath::normalized_vec3f]")
{
	SECTION("+ operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(-1.f, -2.f, -3.f);
		gmath::normalized_vec3f nv = v + v1;
		REQUIRE(nv.is_normalized());
		REQUIRE(nv[0] == 0.267261237f);
		REQUIRE(nv[1] == 0.534522474f);
		REQUIRE(nv[2] == 0.801783741f);
	}
}

TEST_CASE("gmath.normalized_vec3f.subtraction", "[gmath::normalized_vec3f]")
{
	SECTION("- operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		constexpr gmath::vec3f v1(1.f, 2.f, 3.f);
		gmath::normalized_vec3f nv = v - v1;
		REQUIRE(nv.is_normalized());
		REQUIRE(nv[0] == 0.267261237f);
		REQUIRE(nv[1] == 0.534522474f);
		REQUIRE(nv[2] == 0.801783741f);
	}
}

TEST_CASE("gmath.normalized_vec3f.multiplication", "[gmath::normalized_vec3f]")
{
	SECTION("* operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		gmath::normalized_vec3f nv = v * 0.2f;
		REQUIRE(nv.is_normalized());
		REQUIRE(nv[0] == 0.267261237f);
		REQUIRE(nv[1] == 0.534522474f);
		REQUIRE(nv[2] == 0.801783681f);
	}
}

TEST_CASE("gmath.normalized_vec3f.division", "[gmath::normalized_vec3f]")
{
	SECTION("/ operator")
	{
		constexpr gmath::vec3f v(10.f, 20.f, 30.f);
		gmath::normalized_vec3f nv = v / 5;
		REQUIRE(nv.is_normalized());
		REQUIRE(nv[0] == 0.267261237f);
		REQUIRE(nv[1] == 0.534522474f);
		REQUIRE(nv[2] == 0.801783681f);
	}
}


TEST_CASE("gmath.normalized_vec3f.length", "[gmath::normalized_vec3f]")
{
	SECTION("length_squared")
	{
		gmath::normalized_vec3f nv(gmath::vec3f(10.f, 20.f, 30.f));
		REQUIRE(nv.length_squared() == 1.f);
	}

	SECTION("length")
	{
		gmath::normalized_vec3f v(gmath::vec3f(-1203.f, 41220.f, 12310.f));
		REQUIRE(v.length() == 1.f);
	}
}

TEST_CASE("gmath.normalized_vec3f.negation", "[gmath::normalized_vec3f]")
{
	SECTION("negate")
	{
		gmath::normalized_vec3f v(gmath::vec3f(10.f, 20.f, 30.f));
		gmath::normalized_vec3f v1(-v);
		REQUIRE(v != v1);
		REQUIRE(v1.x() == -v.x());
		REQUIRE(v1.y() == -v.y());
		REQUIRE(v1.z() == -v.z());
	}
}

TEST_CASE("gmath.normalized_vec3f.cross", "[gmath::normalized_vec3f]")
{
	SECTION("normalized cross product")
	{
		gmath::normalized_vec3f v(gmath::vec3f(1.f, 0.f, 0.f));
		gmath::normalized_vec3f v1(gmath::vec3f(0.f, 1.f, 0.f));
		static_assert(std::is_same_v<decltype(v.cross(v1)), gmath::normalized_vec3f>, "Check to make sure cross product of normalized vector gives back a normalized vector");
		REQUIRE(v.cross(v1) == gmath::vec3f(0.f, 0.f, 1.f));
		REQUIRE(v1.cross(v) == gmath::vec3f(0.f, 0.f, -1.f));
	}

	SECTION("not normalized cross product")
	{
		gmath::normalized_vec3f v(gmath::vec3f(1.f, 0.f, 0.f));
		gmath::vec3f v1(gmath::vec3f(0.f, 10.f, 0.f));
		static_assert(std::is_same_v<decltype(v.cross(v1)), gmath::vec3f>, "Check to make sure cross product of between a non normalized vector and a normalized vector gives back a non normalized vector");
		gmath::vec3f notNormalCross = v.cross(v1);
		gmath::normalized_vec3f normalCross = v.cross(v1);
		REQUIRE(notNormalCross == gmath::vec3f(0.f, 0.f, 10.f));
		REQUIRE(v1.cross(v) == gmath::vec3f(0.f, 0.f, -10.f));
		REQUIRE(normalCross == gmath::vec3f(0.f, 0.f, 1.f));
	}
}