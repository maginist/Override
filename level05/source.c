#include <stdio.h>
#include <string.h>

int main(void)
{
	int i;
	char str[100];

	i = 0;
	fgets(str, 100, stdin);
	i = 0;
	while(i < strlen(str))
	{
		if (str[i] > 64)
		{
			if (str[i] <= 90)
				str[i] ^= 32;
		}
		i++;
	}
	printf(str);
	exit(0);
}