#pragma once

#include "std.h"

#define NO_COPYABLE(TypeName) \
    TypeName(const TypeName&)            = delete; \
    TypeName(TypeName&&)                 = delete; \
    TypeName& operator=(TypeName&&)      = delete; \
    TypeName& operator=(const TypeName&) = delete

#define BIT(x) (1 << x)

template <typename T> struct Handle {
    u32 handle;
};