
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include "thread_team.h"
#include "task_collection.h"
#include <stdio.h>

static VOID CALLBACK apc_proc_run(ULONG_PTR dwParam)
{
	task_collection* tc = (task_collection*)dwParam;
	tc->run();
}

static DWORD WINAPI thread_func(void* param)
{
	thread_team* owner = (thread_team*)param;
	owner->thread_function();
	return 0;
}

void thread_team::thread_function()
{
	while (true) {
		if (WaitForSingleObjectEx(m_exit_threads_event, INFINITE, TRUE) == WAIT_OBJECT_0)
			return;
		printf("ho!");
	}
}

thread_team::thread_team(int num_of_threads) :
	m_num_of_threads(num_of_threads)
{
	m_exit_threads_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD thread_id;
	for (int i = 0; i<num_of_threads; ++i)
		m_thread_handles[i] = CreateThread(NULL, 1 << 24, thread_func, this, 0, &thread_id);
}

thread_team::~thread_team()
{
	SetEvent(m_exit_threads_event);
	WaitForMultipleObjects(m_num_of_threads, m_thread_handles, TRUE, INFINITE);
	for (int i = 0; i<m_num_of_threads; ++i)
		CloseHandle(m_thread_handles[i]);
	CloseHandle(m_exit_threads_event);
}

void thread_team::invoke_tasks(task_collection& tc)
{
	for (int i = 0; i<m_num_of_threads; ++i)
		QueueUserAPC(apc_proc_run, m_thread_handles[i], (ULONG_PTR)&tc);
}