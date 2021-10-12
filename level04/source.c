#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <stdbool.h>

int main(int ac, char **av){
	int pid;
	char str[32];
	int var1;
	int var2;
	
	pid = fork();
	bzero(str, 32);
	var1 = 0;
	var2 = 0;
	if (pid == 0)
	{
		prctl(1, 1);
		ptrace(0, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(str);
	}
	else
	{
		while(true)
		{
			wait(&var2);
			if ((var2 & 0x7f) != 0)
			{
				puts("child is exiting...");
				return (0);
			}
			if (ptrace(3, pid, 44, 0) == 11)
				break ;
		}
		puts("no exec() for you");
		kill(pid, SIGKILL);
	}
	return(0);
}