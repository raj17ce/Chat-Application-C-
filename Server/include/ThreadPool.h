#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

class ThreadPool {
public:
	ThreadPool(int _ThreadsCount) : ThreadsCount{ _ThreadsCount }, Stop{ false } {
		for (int i = 0; i < ThreadsCount; ++i) {
			Threads.emplace_back([this]() {
				while (true) {
					std::unique_lock<std::mutex> Lock(Mutex);
					CV.wait(Lock, [this]() {
							return (!Tasks.empty() || Stop);
						});

					if (Stop) {
						return;
					}

					auto Task = std::move(Tasks.front());
					Tasks.pop();

					Lock.unlock();

					Task();
				}
				});
		}
	}

	~ThreadPool() {
		std::unique_lock<std::mutex> Lock(Mutex);
		Stop = true;

		Lock.unlock();
		CV.notify_all();

		for (auto& Thread : Threads) {
			Thread.join();
		}
	}

	template<typename T, typename... Args>
	auto ExecuteTask(T&& Task, Args&&... args) -> std::future<decltype(Task(args...))> {
		using ReturnType = decltype(Task(args...));

		auto NewTask = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<T>(Task), std::forward<Args>(args)...));
		auto Future = NewTask->get_future();

		std::unique_lock<std::mutex> Lock(Mutex);

		Tasks.push([NewTask]() -> void {
			(*NewTask)();
		});

		Lock.unlock();
		CV.notify_one();

		return Future;
	};

private:
	int ThreadsCount;
	std::vector<std::thread> Threads;
	std::queue<std::function<void()>> Tasks;
	std::mutex Mutex;
	std::condition_variable CV;
	std::atomic<bool> Stop;
};