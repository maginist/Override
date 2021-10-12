#include <stdio.h>

void clear_stdin(void)
{
	char c;

	c = 0;
	while((c = getchar()) != -1)
		if (c == "\n")
			break ;
	return ;
}

unsigned int get_unum(void)
{
	unsigned int input;

	input = 0;
	fflush(stdout);
	scanf("%u", input);
	clear_stdin();
	return(input);
}

int store_number(int *tab)
{
	unsigned int input = 0;
	unsigned int index = 0;

	printf(" Number:");
	input = get_unum();
	printf(" Index:");
	index = get_unum();
	if (index % 3 == 0 || input >> 24 == 183)
	{
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return(1);
	}
	tab[index] = input;
	return(0);
}

int read_number(int *tab)
{
	unsigned int index = 0;

	printf(" Index:");
	index = get_unum();
	printf(" Number at data[%u] is %u\n", index, tab[index]);
	return(0);
}

int main(int ac, char **av, char **envp)
{
	int tab[100];

	int ret = 0;
	char buff[20];

	bzero(buff, 20);
	bzero(tab, 100);

	while(*av)
	{
		memset(*av, 0, strlen(*av) - 1);
		*av++;
	}
	while(*envp)
	{
		memset(*envp, 0, strlen(*envp) - 1);
		*envp++;
	}
	puts("----------------------------------------------------\n"\
		"  Welcome to wil's crappy number storage service!   \n"\
		"----------------------------------------------------\n"\
		"Commands:                                           \n"\
		"	store - store a number into the data storage      \n"\
		"	read  - read a number from the data storage       \n"\
		"	quit  - exit the program                          \n"\
		"---------------------------------------------------- \n"\
		"wil has reserved some storage :>                     \n"\
		"----------------------------------------------------");
	while (1)
	{
		printf("Input command: ");
		fgets(buff, 20, stdin);
		buff[strlen(buff) - 2] = 0;
		if (strncmp("store", buff, 5) == 0)
			ret = store_number(&tab);
		else if (strncmp("read", buff, 4) == 0)
			ret = read_number(&tab);
		else if (strncmp("quit", buff, 4) == 0)
			break;
		if (ret != 0)
			printf(" Completed %s command successfully\n", buff);
		else
			printf(" Failed to do %s command\n", buff);
		bzero(buff, 20);
	}
	return(0);
}