#include <stdio.h>
#include <unistd.h>

int main()
{
	int fd[2];

	if (pipe(fd) == -1)
		return 1;

	int n;
	pid_t id = fork();
	if (!id)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		execlp("/bin/ls", "", NULL);
	}

	dup2(fd[0], 0);
	close(fd[1]);

	char line[32];

	while (fgets(line, 31, stdin))
			printf("File: %s", line);

	return 0;
}