#pragma once

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <unordered_set>

#include "Base/Log.h"

namespace wind {

class Job {
public:
    Job() = default;
    Job(std::function<void()> newJob) : m_jobHandle(newJob) {}
    Job(std::function<void()> newJob, uint32_t id) : m_jobHandle(newJob), m_jobID(id) {}
    void Execute() { m_jobHandle(); }

private:
    std::function<void()>        m_jobHandle;
    std::unordered_set<uint32_t> m_waitJobs;
    uint32_t                     m_jobID{0};
    bool                         m_paused;
};

class JobSystem {
public:
    static void Init();
    static void Quit();

    static void AddJob(std::function<void()> job);

private:
    static std::vector<std::thread> s_workerThreads;
    static std::stack<Job>          s_jobsList;
    static bool                     s_running;
    static std::mutex               s_mutex;
    static std::condition_variable  s_condition;
};
} // namespace wind