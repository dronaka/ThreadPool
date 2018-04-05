#pragma once
#include <functional>
#include <atomic>
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
	explicit ThreadPool(size_t size);
	~ThreadPool();
	void setExitSignal() { exit = true; };
	void setPauseSignal(const bool& signal) { pause = signal; };
	void addTask(std::function<void()> const &func);

private:
	std::atomic_bool end, pause, exit;
	std::vector<std::thread> threads;
	std::condition_variable cond_var;
	std::queue<std::function<void()>> tasks;
	std::mutex mutex_t;
};