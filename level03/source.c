void decrypt(int mask){
	// int var1 = 0x757c7d51; u|}Q
	// int var2 = 0x67667360; gfs`
	// int var3 = 0x7b66737e; {fs~
	// int var4 = 0x33617c7d; 3a|}
	char str[17] = "Q}|u`sfg~sf{}|a3";
	int len = strlen(str);
	int i = 0;

	while(i < len)
	{
		str[i] = str[i] ^ mask;
		i++;
	}
	if (strncmp(str, "Congratulations!", 17) == 0)
		system("/bin/sh");
	else
		puts("\nInvalid Password");
	return ;
}

void test(int nb1, int nb2)
{
	nb1 -= nb2;
	if (nb1 <= 21)
		decrypt(nb1);
	else
		decrypt(rand());
	return ;
}

int main(void)
{
	int password;

	srand(time(0));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &password);
	test(password, 322424845);
	return(0);
}