#pragma once

#define NO_COPYABLE(TypeName)                  \
	TypeName(const TypeName &) = delete;       \
	TypeName(TypeName &&)      = delete;       \
	TypeName &operator=(TypeName &&) = delete; \
	TypeName &operator=(const TypeName &) = delete

#define BIT(x) (1 << x)