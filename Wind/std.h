#pragma once

#include <span>
#include <array>
#include <string>
#include <vector>

#include <condition_variable>
#include <functional>
#include <map>
#include <cassert>
#include <memory>
#include <mutex>
#include <span>
#include <stack>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <random>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include <cstdint>
#include <variant>

namespace wind {
template <typename T> using Ref     = std::shared_ptr<T>;
template <typename T> using Scope   = std::unique_ptr<T>;
template <typename T> using WeakRef = std::weak_ptr<T>;
} // namespace wind

namespace wind::ref {
template <typename T, typename... Args> Ref<T> Create(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace wind::ref

namespace wind::scope {
template <typename T, typename... Args> Scope<T> Create(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
} // namespace wind::scope