#pragma once

namespace wind {
class Timestep {
public:
    Timestep() {}
    Timestep(float time);

    inline float GetSeconds() const { return m_time; }
    inline float GetMilliseconds() const { return m_time * 1000.0f; }

    operator float() { return m_time; }

private:
    float m_time = 0.0f;
};
} // namespace wind