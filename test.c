#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1

int popen2(int *fd_r,int *fd_w)
{
	int pipe_p2c[2];
	int pipe_c2p[2];

	pid_t pid;
	if (pipe(pipe_c2p) < 0)
	{
		perror("popen2");
		return (1);
	}
	if (pipe(pipe_p2c) < 0)
	{
		perror("popen2");
		close(pipe_c2p[READ]);
		close(pipe_c2p[WRITE]);
		return (1);
	}
	//プロセスの作成
	if ((pid = fork()) < 0)
	{
		perror("popen2");
		close(pipe_c2p[READ]);
		close(pipe_c2p[WRITE]);
		close(pipe_p2c[READ]);
		close(pipe_p2c[WRITE]);
		return (1);
	}
	//子プロセスが正常に起動
	if (pid == 0)
	{
		close(pipe_p2c[WRITE]);
		close(pipe_c2p[READ]);

		// 親→子への出力を標準入力として割り当て
		dup2(pipe_p2c[READ],0);
		// 子→親への入力を標準出力に割り当て
		dup2(pipe_c2p[WRITE],1);

		close(pipe_p2c[READ]);
		close(pipe_c2p[WRITE]);

		char	*arg[2];
		arg[0] = "/bin/cat";
		arg[1] = NULL;
		execve("/bin/cat", arg, NULL);
		perror("popen2");
		close(pipe_p2c[READ]);
		close(pipe_c2p[WRITE]);
		return (1); 
	}

	close(pipe_p2c[READ]);
	close(pipe_c2p[WRITE]);

	*fd_r = pipe_c2p[READ];
	*fd_w = pipe_p2c[WRITE];

	return (pid);
}

int main(int argc, char *argv[])
{
	int fd_r = fileno(stdin);
	int fd_w = fileno(stdout);

	if (argc < 2)
	{
		printf("error\n");
		fflush(stdout);
		return (0);
	}
	popen2(&fd_r, &fd_w);

	write(fd_w, argv[1], strlen(argv[1]));

	char buf[255];
	int size = read(fd_r, buf, 255);
	if (size == -1)
	{
		perror("error");
		return (1);
	}
	printf("test2: ");
	fwrite(buf, 1, size, stdout);
	printf("\n");
	return (0);
}


