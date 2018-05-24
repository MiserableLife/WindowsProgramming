#pragma once
#include <windows.h>

class task_collection
{
public:
	task_collection();
	virtual ~task_collection();

	virtual void perform_task(int index) = 0;
	virtual int  get_num_of_tasks() const = 0;

	void wait_for_completion() const;

private:

	void run();
	friend VOID CALLBACK apc_proc_run(ULONG_PTR dwParam);

private:

	volatile LONG m_next_task_index;
	volatile LONG m_number_of_completed_tasks;
	HANDLE m_finish_event;
};