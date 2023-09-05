#include "JobSystem.h"

#include "Base/Log.h"
#include "JobSystem/JobSystem.h"
#include <mutex>
#include <sstream>
#include <thread>
#include <winbase.h>

namespace wind {
bool                     JobSystem::s_running = false;
std::vector<std::thread> JobSystem::s_workerThreads{};
std::stack<Job>          JobSystem::s_jobsList{};
std::mutex               JobSystem::s_mutex{};
std::condition_variable  JobSystem::s_condition{};

void JobSystem::Init() {
    uint32_t numCores = std::thread::hardware_concurrency();
    s_running         = true;
    WIND_CORE_INFO("Worker thread num is {}", numCores);
    for (uint32_t i = 0; i < numCores; ++i) {
        s_workerThreads.emplace_back([]() {
            // std::ostringstream oss;
            // oss << std::this_thread::get_id();
            // WIND_CORE_INFO("Thread id is {}", oss.str());
            while (s_running) {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(s_mutex);
                    s_condition.wait(lock, []() { return !s_running || !s_jobsList.empty(); });
                    if (!s_running && s_jobsList.empty()) { return; }
                    job = std::move(s_jobsList.top());
                    s_jobsList.pop();
                }
                job.Execute();
            }
        });
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
    std::unique_lock<std::mutex> lock;
    s_jobsList.push(std::move(job));
    s_condition.notify_one();
}

} // namespace wind
