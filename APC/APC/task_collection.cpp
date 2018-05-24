#include "task_collection.h"
#include "thread_team.h"

task_collection::task_collection() : m_next_task_index(0), m_number_of_completed_tasks(0)
{
	m_finish_event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

task_collection::~task_collection()
{
	CloseHandle(m_finish_event);
}

void task_collection::wait_for_completion() const
{
	WaitForSingleObject(m_finish_event, INFINITE);
}

void task_collection::run() {
	int num_of_tasks = get_num_of_tasks();
	while (m_number_of_completed_tasks<num_of_tasks) {
		LONG task_index = InterlockedIncrement(&m_next_task_index) - 1;
		if (task_index >= num_of_tasks)
			break;
		perform_task(task_index);
		LONG num_of_completed_tasks = InterlockedIncrement(&m_number_of_completed_tasks);
		if (num_of_completed_tasks == num_of_tasks)
			SetEvent(m_finish_event);
	}
}