#pragma once

#include <utility>

NS_BEGIN

template <typename F>
struct Defer final
{
	Defer(F &&fnDefer) : m_fnDefer(std::move(fnDefer)) {}
	Defer(const Defer &src) = delete;
	Defer(Defer &&src) = default;		// Move constructor is required for C++14 support
	~Defer(void) {m_fnDefer();}

	Defer &operator=(const Defer &src) = delete;
	Defer &operator=(Defer &&src) = delete;

	F m_fnDefer;
};

// Helper function for C++14 support
template <typename F>
Defer<F> MakeDefer(F &&fnDefer)
{
	return Defer<F>(std::move(fnDefer));
}

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

// C++14 version
#define DEFER(code) const auto CONCAT(_defer_, __LINE__) = MakeDefer([&](){code;})
// C++17 version
//#define DEFER(code) const auto CONCAT(_defer_, __LINE__) = Defer([&](){code;})

NS_END
