#pragma once

#include "task_collection.h"


class thread_team
{
public:
	thread_team(int num_of_threads);
	virtual ~thread_team();
	void invoke_tasks(task_collection& tc);

private:
	void thread_function();
	friend static DWORD WINAPI thread_func(void* param);

private:
	HANDLE m_thread_handles[1024];
	HANDLE m_exit_threads_event;
	int m_num_of_threads;
};
