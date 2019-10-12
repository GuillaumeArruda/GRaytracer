#pragma once

template<typename T>
void trivial_static_assert()
{

	static_assert(std::is_trivially_destructible_v<T>, "not trivially_destructible");
	static_assert(std::is_trivially_copyable_v<T>, "not is_trivially_copyable");
	static_assert(std::is_trivially_copy_assignable_v<T>, "not trivially_copy_assignable");
	static_assert(std::is_trivially_move_assignable_v<T>, "not trivially_move_assignable");
}