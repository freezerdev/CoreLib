#pragma once

#include <thread>

NS_BEGIN

size_t GetThreadId(const std::thread::id &id = std::thread::id());

NS_END
