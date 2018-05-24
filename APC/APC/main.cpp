#ifdef SIMPLE


#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <iostream>

class Printer
{

public:
	void print_hello()
	{
		std::cout<<"hello~ "<<std::endl;
	}
};



DWORD WINAPI thread_function(LPVOID lpParameter) 
{ 
	while (true) 
	{ SleepEx(INFINITE, TRUE);
	} 
}
VOID CALLBACK apc_function_1(ULONG_PTR dwParam) 
{ 
	Printer* printer = (Printer*)dwParam;
	printer->print_hello();
}

void main() {
	Printer obj;
	DWORD thread_id;
	HANDLE thread_handle = CreateThread(NULL, 0, thread_function, NULL, 0, &thread_id);
	Sleep(1000);
	QueueUserAPC(apc_function_1, thread_handle, (ULONG_PTR)&obj);
	Sleep(1000);
}

#else


#include "thread_team.h"
#include "task_collection.h"

class apply_function_to_array : public task_collection
{
public:
	apply_function_to_array(double* x, int n, int num_of_tasks) : m_x(x), m_n(n), m_num_of_tasks(num_of_tasks) {}
	virtual void perform_task(int index) {
		int nx_per_task = (m_n + 1) / m_num_of_tasks;
		int i0 = index * nx_per_task;
		int i1 = (index + 1) * nx_per_task;
		if (i1>m_n)
			i1 = m_n;
		for (int i = i0; i<i1; ++i)
			m_x[i] = func(m_x[i]);
	}
	virtual int  get_num_of_tasks() const { return m_num_of_tasks; }

private:
	double func(double x) const { return 2 * x; }
	int m_n;
	int m_num_of_tasks;
	double* m_x;
};

void main()
{
#define NX 10000000
	double* x = new double[NX];
	for (int i = 0; i<NX; ++i)
		x[i] = 1.0;

	apply_function_to_array tc(x, NX, 1000);
	thread_team tt(4);
	tt.invoke_tasks(tc);
	tc.wait_for_completion();

	delete[] x;
	system("pause");
}


#endif




