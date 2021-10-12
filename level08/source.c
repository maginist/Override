#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


void log_wrapper(FILE *file, char *str, char *str2)
{
	char buff[272];

	strcpy(buff, str);
	snprintf(&buff[strlen(buff)], 254 - strlen(buff), str2);
	buff[strcspn(buff, "\n")] = 0;
	fprintf(file, "LOG: %s\n", buff);
	return;
}

int main(int ac,char **av)
{
	FILE *backup;
	FILE *file;
	char buffer[99];
	int fd;
	char c;

	c = -1;
	if (ac <= 2)
	{
		printf("Usage: %s filename\n");
	}
	backup = fopen("./backups/.log", "w");
	if (!backup)
	{
		printf("ERROR: Failed to open %s\n");
		exit(1);
	}
	log_wrapper(backup, "Starting back up: ", av[1]);
	file = fopen(av[1],"r");
	if (!file)
	{
		printf("ERROR: Failed to open %s\n");
		exit(1);
	}
	strcpy(buffer, "./backups/");
	strncat(buffer, av[1], 99 - strlen(buffer) - 1);
	fd = open(av[1], O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	if (!fd)
	{
		printf("ERROR: Failed to open %s%s\n","./backups/", av[1]);
		exit(1);
	}
	while((c = fgetc(file)) != EOF)
		write(fd, c, 1);
	log_wrapper(backup, "Finished back up ", av[1]);
	fclose(backup);
	close(fd);
}
