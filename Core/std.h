#pragma once

#include <span>
#include <string>
#include <vector>

#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <span>
#include <stack>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string_view>

#include <cstdint>

using i8  = int8_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;

namespace wind {
template <typename T> using Ref   = std::shared_ptr<T>;
template <typename T> using Scope = std::unique_ptr<T>;
} // namespace wind

namespace wind::ref {
template <typename T, typename... Args> 
Ref<T> Create(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace wind::ref

namespace wind::scope {
template <typename T, typename... Args>
Scope<T> Create(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}
} // namespace wind::scope