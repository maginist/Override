#include <stdio.h>
#include <stdlib.h>

int main()
{
	int var;

	puts("***********************************");
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &var);
	if (5276 == var)
	{
		puts("\nAuthenticated!");
		system("/bin/sh");
	}
	else
	{
		puts("Invalid Password!");
		return(1);
	}
	return(0);
}