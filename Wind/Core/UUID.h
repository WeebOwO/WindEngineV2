#pragma once

#include "std.h"

namespace wind {
class UUID {
public:
    UUID();
    UUID(u64 uuid);
    UUID(const UUID& other);

    operator u64() { return m_UUID; }
    operator const u64() const { return m_UUID; }

private:
    u64 m_UUID;
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