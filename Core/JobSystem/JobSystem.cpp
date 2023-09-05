#include "JobSystem.h"

#include "Base/Log.h"
#include "JobSystem/JobSystem.h"
#include <mutex>
#include <sstream>

namespace wind {
bool                     JobSystem::s_running = false;
std::vector<std::thread> JobSystem::s_workerThreads{};
std::vector<Job>         JobSystem::s_waitJobs{};
std::vector<Job>         JobSystem::s_executedJobs{};

std::unordered_set<int> JobSystem::s_waitJobsLUT;
std::unordered_set<int> JobSystem::s_executedJobsLUT;

std::mutex JobSystem::s_mutex{};
std::mutex JobSystem::s_executedJobsMutex{};

std::condition_variable JobSystem::s_condition{};

void Job::AddDependencyJob(std::span<int> jobs) {
    for (const auto& jobId : jobs) {
        m_waitJobs.push_back(jobId);
    }
}

void JobSystem::Init() {
    s_running               = true;
    unsigned int numThreads = std::thread::hardware_concurrency();
    for (unsigned int i = 0; i < numThreads; ++i) {
        s_workerThreads.emplace_back(WorkerThread);
    }
}

void JobSystem::Quit() {
    s_running = false;
    s_condition.notify_all();

    for (auto& thread : s_workerThreads) {
        thread.join();
    }
}

void JobSystem::AddJob(std::function<void()> job) {
    Job newJob(job, -1);
    AddJob(std::move(newJob));
}

void JobSystem::AddJob(Job&& job) {
    std::unique_lock<std::mutex> lock(s_mutex);

    for (const auto& jobId : job.m_waitJobs) {
        bool found = false;
        for (const auto& job : s_waitJobs) {
            if (job.m_jobID == jobId) {
                found = true;
                break;
            }
        }
        if (!found) {
            job.m_paused = true;
            break;
        }
    }

    s_waitJobs.push_back(job);
    s_waitJobsLUT.insert(job.m_jobID);

    s_condition.notify_one();
}

void JobSystem::WorkerThread() {
    while (true) {
        bool canExecute = true;
        Job  job{};
        {
            std::unique_lock<std::mutex> lock(s_mutex);
            s_condition.wait(lock, [] { return !s_waitJobs.empty() || !s_running; });

            if (!s_running) { break; }

            job = s_waitJobs.back();
            s_waitJobs.pop_back();
        }

        for (const auto& jobId : job.m_waitJobs) {
            bool found = false;

            if (s_executedJobsLUT.contains(jobId)) {
                found = true;
                break;
            }

            if (!found) {
                canExecute = false;
                break;
            }
        }

        if (canExecute) {
            job.Execute();
            std::unique_lock<std::mutex> lock(s_mutex);
            s_executedJobs.push_back(job);
            s_executedJobsLUT.insert(job.m_jobID);
            s_waitJobsLUT.erase(job.m_jobID);
        } else {
            std::unique_lock<std::mutex> lock(s_mutex);
            s_waitJobs.push_back(job);
        }
    }
}

void JobEntry(Job&& job) { wind::JobSystem::AddJob(std::move(job)); }
void JobEntry(std::function<void()> jobFunc, int id, std::vector<int> deps) {
    Job job{jobFunc, id, deps};
    wind::JobSystem::AddJob(std::move(job));
}
} // namespace wind
