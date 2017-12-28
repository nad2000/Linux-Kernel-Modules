#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t tid;
	tid = syscall(SYS_gettid);
	printf("Thread ID: %d\n", tid);
	printf("PID is: %d\n", getpid());

	return 0;
}
