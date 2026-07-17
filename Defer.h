#pragma once

#include <utility>

NS_BEGIN

//#################################################################################################
template<typename F>
struct DeferGuard final
{
	explicit DeferGuard(F &&fnDefer) : m_fnDefer(std::move(fnDefer)) {}
	DeferGuard(const DeferGuard &src) = delete;
	DeferGuard(DeferGuard &&src) = default;		// Move constructor is required for C++14 support
	~DeferGuard(void) {m_fnDefer();}

	DeferGuard &operator=(const DeferGuard &src) = delete;
	DeferGuard &operator=(DeferGuard &&src) = delete;

	F m_fnDefer;
};

// Helper function for C++14 support
template<typename F>
DeferGuard<F> MakeDefer(F &&fnDefer)
{
	return DeferGuard<F>(std::move(fnDefer));
}

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

// C++14 version
#define DEFER(code) const auto CONCAT(_defer_, __LINE__) = MakeDefer([&](void){code;})
// C++17 version
//#define DEFER(code) const auto CONCAT(_defer_, __LINE__) = DeferGuard([&](void){code;})

NS_END
