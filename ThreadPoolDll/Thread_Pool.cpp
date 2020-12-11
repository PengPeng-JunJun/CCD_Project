#include "stdafx.h"
#include "Thread_Pool.h"


#include <assert.h>
#include <iostream>
#include <sstream>

CThread_Pool::CThread_Pool(int init_size)
  :m_init_threads_size(init_size)
  , m_mutex()
  , m_cond()
  , m_is_started(false)
{
  start();
}

CThread_Pool::~CThread_Pool()
{
  if(m_is_started)
  {
    stop();
  }
}

void CThread_Pool::start()
{
  assert(m_threads.empty());
  m_is_started = true;
  m_threads.reserve(m_init_threads_size);
  for (int i = 0; i < m_init_threads_size; ++i)
  {
    m_threads.push_back(new std::thread(std::bind(&CThread_Pool::thread_loop, this)));
  }

}

void CThread_Pool::stop()
{
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_is_started = false;
    m_cond.notify_all();
  }

  for (threads_t::iterator it = m_threads.begin(); it != m_threads.end() ; ++it)
  {
    (*it)->join();
    delete *it;
  }
  m_threads.clear();
}


void CThread_Pool::thread_loop()
{
  while(m_is_started)
  {
    task_t task = take();
    if(task)
    {
      task();
    }
  }
}


void CThread_Pool::add_task(const task_t& task)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  /*while(m_tasks.isFull())
    {//when m_tasks have maxsize
      cond2.notify_one();
    }
  */
  m_tasks.push_back(task);
  m_cond.notify_one();
}


CThread_Pool::task_t CThread_Pool::take()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  //always use a while-loop, due to spurious wakeup
  while(m_tasks.empty() && m_is_started)
  {
    m_cond.wait(lock);
  }

  task_t task;
  tasks_t::size_type size = m_tasks.size();
  if(!m_tasks.empty() && m_is_started)
  {
    task = m_tasks.front();
    m_tasks.pop_front();
    assert(size - 1 == m_tasks.size());
    /*if (TaskQueueSize_ > 0)
    {
      cond2.notify_one();
    }*/
  }

  return task;

}