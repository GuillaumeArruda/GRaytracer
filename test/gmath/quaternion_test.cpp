#include "Catch2/catch.hpp"

#include "gmath/quaternion.hpp"

#include "static_assert_helper.hpp"

TEST_CASE("gmath.quaternion.construction", "[gmath::quaternion]")
{
	SECTION("Default Construction")
	{
		constexpr gmath::quaternion quat;
		REQUIRE(quat.x() == 0.f);
		REQUIRE(quat.y() == 0.f);
		REQUIRE(quat.z() == 0.f);
		REQUIRE(quat.w() == 1.f);
	}

	SECTION("Angles Construction")
	{
		{
			const gmath::yprangles angles(90._d, 0._r, 0._r);
			const gmath::quaternion quat(angles);
			REQUIRE(gmath::within_epsilon(quat.x(), 0.f));
			REQUIRE(gmath::within_epsilon(quat.y(), 0.f));
			REQUIRE(gmath::within_epsilon(quat.z(), 0.707106769f));
			REQUIRE(gmath::within_epsilon(quat.w(), 0.707106769f));
		}

		{
			const gmath::yprangles angles(0._r, 90._d, 0._r);
			const gmath::quaternion quat(angles);
			REQUIRE(gmath::within_epsilon(quat.x(), 0.f));
			REQUIRE(gmath::within_epsilon(quat.y(), 0.707106769f));
			REQUIRE(gmath::within_epsilon(quat.z(), 0.f));
			REQUIRE(gmath::within_epsilon(quat.w(), 0.707106769f));

		}

		{
			const gmath::yprangles angles(0._r, 0._r, 90._d);
			const gmath::quaternion quat(angles);
			REQUIRE(gmath::within_epsilon(quat.x(), 0.707106769f));
			REQUIRE(gmath::within_epsilon(quat.y(), 0.f));
			REQUIRE(gmath::within_epsilon(quat.z(), 0.f));
			REQUIRE(gmath::within_epsilon(quat.w(), 0.707106769f));
		}

		{
			const gmath::yprangles angles(45._d, 45._d, 0._d);
			const gmath::quaternion quat(angles);
			REQUIRE(gmath::within_epsilon(quat.x(), -0.1464466f));
			REQUIRE(gmath::within_epsilon(quat.y(), 0.3535534f));
			REQUIRE(gmath::within_epsilon(quat.z(), 0.3535534f));
			REQUIRE(gmath::within_epsilon(quat.w(), 0.8535534f));
		}
	}
}

TEST_CASE("gmath.quaternion.rotate_vec3f", "[gmath::quaternion]")
{
	{
		const gmath::yprangles angles(90._d, 0._r, 0._r);
		const gmath::quaternion quat(angles);
		const gmath::vec3f v(1.f, 0.f, 0.f);
		const gmath::vec3f v2 = quat * v;
		const gmath::vec3f v3 = -quat * v;

		REQUIRE(gmath::within_epsilon(v2.x(), 0.f));
		REQUIRE(gmath::within_epsilon(v2.y(), 1.f));
		REQUIRE(gmath::within_epsilon(v2.z(), 0.f));
		REQUIRE(gmath::within_epsilon(v3.x(), 0.f));
		REQUIRE(gmath::within_epsilon(v3.y(), -1.f));
		REQUIRE(gmath::within_epsilon(v3.z(), 0.f));

	}

	{
		const gmath::yprangles angles(0._r, 90._d, 0._r);
		const gmath::quaternion quat(angles);
		const gmath::vec3f v(1.f, 0.f, 0.f);
		const gmath::vec3f v2 = quat * v;
		const gmath::vec3f v3 = -quat * v;

		REQUIRE(gmath::within_epsilon(v2.x(), 0.f));
		REQUIRE(gmath::within_epsilon(v2.y(), 0.f));
		REQUIRE(gmath::within_epsilon(v2.z(), -1.f));
		REQUIRE(gmath::within_epsilon(v3.x(), 0.f));
		REQUIRE(gmath::within_epsilon(v3.y(), 0.f));
		REQUIRE(gmath::within_epsilon(v3.z(), 1.f));

	}

	{
		const gmath::yprangles angles(0._r, 0._r, 90._d);
		const gmath::quaternion quat(angles);
		const gmath::vec3f v(1.f, 0.f, 0.f);
		const gmath::vec3f v2 = quat * v;
		const gmath::vec3f v2_inverse = quat.inverse() * v2;
		const gmath::vec3f v3 = -quat * v;
		const gmath::vec3f v3_inverse = (-quat).inverse() * v3;

		REQUIRE(gmath::within_epsilon(v2.x(), 1.f));
		REQUIRE(gmath::within_epsilon(v2.y(), 0.f));
		REQUIRE(gmath::within_epsilon(v2.z(), 0.f));
		REQUIRE(within_epsilon(v2_inverse,v));
		REQUIRE(gmath::within_epsilon(v3.x(), 1.f));
		REQUIRE(gmath::within_epsilon(v3.y(), 0.f));
		REQUIRE(gmath::within_epsilon(v3.z(), 0.f));
		REQUIRE(within_epsilon(v3_inverse, v));
	}
}

TEST_CASE("gmath.quaternion.conversion", "[gmath::quaternion]")
{
	{
		const gmath::yprangles angles(45._d, 45._d, gmath::radian(gmath::pi));
		const gmath::quaternion quat(angles);
		const gmath::normalized_vec3f v(quat);
		const gmath::yprangles angles2(quat);
		REQUIRE(gmath::within_epsilon(v.x(), 0.5f));
		REQUIRE(gmath::within_epsilon(v.y(), 0.5f));
		REQUIRE(gmath::within_epsilon(v.z(), -0.707106769f));
	}

	{
		const gmath::yprangles angles(45._d, -43._d, 90._d);
		const gmath::quaternion quat(angles);
		const gmath::normalized_vec3f v(quat);
		const gmath::yprangles angles2(quat);
		REQUIRE(within_epsilon(angles, angles2));
	}
}

TEST_CASE("gmath.quaternion.static_assert", "[gmath::quaternion]")
{
	trivial_static_assert<gmath::quaternion>();
}