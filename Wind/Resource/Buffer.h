#pragma once

#include "std.h"

namespace wind {
struct Buffer {
    using byte = u8;
    void* Data;
    u64   Size;

    Buffer() : Data(nullptr), Size(0) {}

    Buffer(const void* data, u64 size = 0) : Data((void*)data), Size(size) {}

    static Buffer Copy(const Buffer& other) {
        Buffer buffer;
        buffer.Allocate(other.Size);
        memcpy(buffer.Data, other.Data, other.Size);
        return buffer;
    }

    static Buffer Copy(const void* data, u64 size) {
        Buffer buffer;
        buffer.Allocate(size);
        memcpy(buffer.Data, data, size);
        return buffer;
    }

    void Allocate(u64 size) {
        delete[] (byte*)Data;
        Data = nullptr;

        if (size == 0) return;

        Data = new byte[size];
        Size = size;
    }

    void Release() {
        delete[] (byte*)Data;
        Data = nullptr;
        Size = 0;
    }

    void ZeroInitialize() {
        if (Data) memset(Data, 0, Size);
    }

    template <typename T> T& Read(u64 offset = 0) { return *(T*)((byte*)Data + offset); }

    template <typename T> const T& Read(u64 offset = 0) const {
        return *(T*)((byte*)Data + offset);
    }

    byte* ReadBytes(u64 size, u64 offset) const {
        assert(offset + size <= Size);
        byte* buffer = new byte[size];
        memcpy(buffer, (byte*)Data + offset, size);
        return buffer;
    }

    void Write(const void* data, u64 size, u64 offset = 0) {
        assert(offset + size <= Size);
        memcpy((byte*)Data + offset, data, size);
    }

    operator bool() const { return Data; }

    byte& operator[](int index) { return ((byte*)Data)[index]; }

    byte operator[](int index) const { return ((byte*)Data)[index]; }

    template <typename T> T* As() const { return (T*)Data; }

    inline u64 GetSize() const { return Size; }
};
}; // namespace wind