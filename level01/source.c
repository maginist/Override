#include <stdio.h>
#include <string.h>

char *a_user_name;

int verify_user_name(void){
	puts("verifying username....");
	return(strncmp(a_user_name, "dat_will", 7));
}

int verify_user_pass(char *str){
	return (strncmp(str, "admin", 5));
}

int main(void)
{
	int var;
	char pass[68];

	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username:");
	fgets(a_user_name, 256, stdin);
	var = verify_user_name();
	if (var != 0)
	{
		puts("nope, incorrect username...");
		return(1);
	}
	puts("Enter Password:");
	fgets(pass, 100, stdin);
	var = verify_user_pass(pass);
	if (var == 0 || var != 0)
	{
		puts("nope, incorrect password...");
		return(1);
	}
	return(0);
}