#pragma once

#include "Core/Log.h"

namespace wind {

class Job {
public:
    friend class JobSystem;
    Job() = default;
    Job(std::function<void()> newJob, int id = -1, std::span<int> deps = {})
        : m_jobHandle(newJob), m_jobID(id) {
        AddDependencyJob(deps);
    }

    void AddDependencyJob(std::span<int> jobs);
    void Execute() { m_jobHandle(); }

private:
    std::function<void()> m_jobHandle;
    std::vector<int>      m_waitJobs;
    int                   m_jobID;
    bool                  m_paused;
};

class JobSystem {
public:
    static void Init();
    static void Quit();

    static void AddJob(Job&& job);
    static void AddJob(std::function<void()> job);

private:
    static void WorkerThread();

    static std::vector<std::thread> s_workerThreads;

    static std::unordered_set<int> s_waitJobsLUT;
    static std::unordered_set<int> s_executedJobsLUT;

    static std::vector<Job> s_waitJobs;
    static std::vector<Job> s_executedJobs;

    static bool                    s_running;
    static std::mutex              s_mutex;             // mutex for vector
    static std::mutex              s_executedJobsMutex; // mutex for execute
    static std::condition_variable s_condition;
};

void JobEntry(Job&& job);
void JobEntry(std::function<void()> job, int id = -1, std::vector<int> deps = {});
} // namespace wind