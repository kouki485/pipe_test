#include <stdio.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int popen(int *fd_r,int *fd_w)
{
	int pipe_p2c[2];
	int pipe_c2p[2];

	pid_t pid;
	if (pipe(pipe_c2p) < 0)
	{
		perror("popen");
		close(pipe_c2p[READ]);
		close(pipe_c2p[WRITE]);
		return (1);
	}
	if (pipe(pipe_p2c) < 0)
	{
		perror("popen");
		close(pipe_p2c[READ]);
		close(pipe_p2c[WRITE]);
		return (1);
	}
	//プロセスの作成
	if ((pid = fork()) < 0)
	{
		perror("fork");
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


	}



}

int main()
{
	const int buf_size = 256;
	char buf[buf_size];

	ssize_t rc;

	rc = read(STDIN_FILENO,buf,buf_size);
	if (rc == -1)
		perror("write error");
	
	printf("test: ");
	fwrite(buf,1,rc,stdout);
	//printf("\n");
	return (0);
}