#pragma once

#include "std.h"

namespace wind {
class UUID {
public:
    UUID();
    UUID(uint64_t uuid);
    UUID(const UUID& other);

    operator uint64_t() { return m_UUID; }
    operator const uint64_t() const { return m_UUID; }

private:
    uint64_t m_UUID;
};
} // namespace wind

namespace std {

template <> struct hash<wind::UUID> {
    std::size_t operator()(const wind::UUID& uuid) const {
        // uuid is already a randomly generated number, and is suitable as a hash key as-is.
        // this may change in future, in which case return hash<uint64_t>{}(uuid); might be more
        // appropriate
        return uuid;
    }
};
} // namespace std