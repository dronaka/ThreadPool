#include "ThreadPool.h"
#include <iostream>
#include <thread>

ThreadPool::ThreadPool(size_t size) : end(false), pause(false), exit(false) {
	for (int i = 0; i < int(size); i++) {
		threads.emplace_back(
			[this] {
			while (true) {
				std::unique_lock<std::mutex> lock(mutex_t);
				cond_var.wait(lock, [this] { return end || !tasks.empty()|| exit;});

				if ((end && tasks.empty()) || exit) {
					break;
				}

				cond_var.wait(lock, [this] { return !pause; });

				std::function<void()> work;
				work = tasks.front();
				tasks.pop();
				lock.unlock();
				work();
			}
		}
		);
	}
}

void ThreadPool::addTask(std::function<void()> const &func) {
	std::unique_lock<std::mutex> lock(this->mutex_t);
	this->tasks.push(func);
	this->cond_var.notify_one();
}

ThreadPool::~ThreadPool() {
	end = true;
	cond_var.notify_all();
	for (auto &i : threads) {
		i.join();
	}
}