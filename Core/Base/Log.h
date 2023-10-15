#pragma once

#include <spdlog/spdlog.h>

#include "std.h"

namespace wind {
class Log {
public:
    static void Init();

    static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static Ref<spdlog::logger> s_CoreLogger;
    static Ref<spdlog::logger> s_ClientLogger;
};

// Core log macros
#define WIND_CORE_TRACE(...) ::wind::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define WIND_CORE_INFO(...) ::wind::Log::GetCoreLogger()->info(__VA_ARGS__)
#define WIND_CORE_WARN(...) ::wind::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define WIND_CORE_ERROR(...) ::wind::Log::GetCoreLogger()->error(__VA_ARGS__)
#define WIND_CORE_CRITICAL(...) ::wind::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define WIND_TRACE(...) ::wind::Log::GetClientLogger()->trace(__VA_ARGS__)
#define WIND_INFO(...) ::wind::Log::GetClientLogger()->info(__VA_ARGS__)
#define WIND_WARN(...) ::wind::Log::GetClientLogger()->warn(__VA_ARGS__)
#define WIND_ERROR(...) ::wind::Log::GetClientLogger()->error(__VA_ARGS__)
#define WIND_CRITICAL(...) ::wind::Log::GetClientLogger()->critical(__VA_ARGS__)
} // namespace wind