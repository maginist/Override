#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *file;
	char buffer[48];
	char login[112];
	char passwd[100];

	bzero(buffer, 5);
	bzero(login, 12);
	bzero(passwd, 12);

	if ((file = fopen("/home/users/level03/.pass",'r')) != NULL)
	{
		fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
		exit(1);
	}
	fread(buffer, 1, 41, file);
	if (41 != strcspn(buffer,"\n"))
	{
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
		exit(1);
	}
	fclose(file);
	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\**************************************/ ");
	printf("--[ Username: ");
	fgets(login,100,stdin);
	strcspn(login,"\n");
	printf("--[ Password: ");
	fgets(passwd,100,stdin);
	strcspn(passwd,"\n");
	puts("*****************************************");
	if (strncmp(buffer, passwd, 41) == 0)
	{
		printf("Greetings, %s!", login);
		system("/bin/sh");
	}
	else
	{
		printf(login);
		puts(" does not have access!");
		exit(1);
	}
}