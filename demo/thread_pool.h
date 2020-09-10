#pragma once

#include<vector>
#include<queue>
#include<thread>
#include<iostream>
#include<stdexcept>
#include<condition_variable>
#include<memory>
#include<functional>

const int MAX_THREADS = 1000;

typedef std::function<void(void)> Task;

int num = 0;

class threadPool
{
public:
	threadPool(int number = 1);
	~threadPool();
	bool append(Task task);

private:
	static void* worker(void* arg);
	void run();

private:
	std::vector<std::thread> work_threads;
	std::queue<Task> tasks_queue;

	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;

};
threadPool::threadPool(int number) : stop(false)
{
	if (number <= 0 || number > MAX_THREADS) {
		throw std::exception();
	}
	for (int i = 0; i < number; i++) {
		std::cout << "创建第" << i << "个进程" << std::endl;
		work_threads.emplace_back(threadPool::worker, this);
	}
}

inline threadPool::~threadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
		condition.notify_all();
		for (auto& ww : work_threads)
			ww.join();
	}
}

bool threadPool::append(Task task)
{
	queue_mutex.lock();
	tasks_queue.push(task);
	queue_mutex.unlock();
	condition.notify_one();
	return true;
}
void* threadPool::worker(void* arg)
{
	threadPool* pool = (threadPool*)arg;
	pool->run();
	return pool;
}

void threadPool::run()
{
	while (!stop) {
		std::unique_lock<std::mutex> lk(this->queue_mutex);
		//queue_mutex.lock();
        std::cout<<"wait for task ..."<<std::endl;
		this->condition.wait(lk, [this] {return !this->tasks_queue.empty(); });
		if (this->tasks_queue.empty()) {
			continue;
		}
		else {
			Task task = tasks_queue.front();
			tasks_queue.pop();
			lk.unlock();
			//在获取到任务队列的任务之后，应该马上解锁任务队列，再执行这个任务
			task();
		}
	}
}