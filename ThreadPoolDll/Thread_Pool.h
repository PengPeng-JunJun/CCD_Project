#pragma once

#include <vector>
#include <deque>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <memory>


class CThread_Pool{
public:
	typedef std::function<void()> task_t;

	CThread_Pool(int init_size = 3);
	~CThread_Pool();

	void stop();
	void add_task(const task_t&);  //thread safe;

private:
	CThread_Pool(const CThread_Pool&);//½ûÖ¹¸´ÖÆ¿½±´.
	const CThread_Pool& operator=(const CThread_Pool&);

	bool is_started() { return m_is_started; }
	void start();

	void thread_loop();
	task_t take();

	typedef std::vector<std::thread*> threads_t;
	typedef std::deque<task_t> tasks_t;

	int m_init_threads_size;

	threads_t m_threads;
	tasks_t m_tasks;

	std::mutex m_mutex;
	std::condition_variable m_cond;
	bool m_is_started;
};