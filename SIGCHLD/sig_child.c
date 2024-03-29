#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void catch(int signo)
{
	int status;
	pid_t pid;
	while((pid = waitpid(0,&status, WNOHANG)) > 0)
	{
		if(WIFEXITED(status))
			printf("child %d exit %d\n", pid, WEXITSTATUS(status));
		else if(WIFSIGNALED(status))
			printf("child %d cancel signal %d\n", pid, WTERMSIG(status));
	}
}
int main()
{
	pid_t pid;
	int i;
	for(i = 0; i < 10; i++)
	{
		if((pid = fork()) == 0)
			break;
		else if(pid < 0)
		{
			perror("fork error");
			exit(1);
		}
	}

	if(pid == 0)
	{
		sleep(1);
		return i+1;
	}
	else if(pid > 0)
	{
		struct sigaction act;
		act.sa_handler = catch;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD, &act, NULL);

		while(1);


	}

	return 0;
}
