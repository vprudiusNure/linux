#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>

int listener_d;

void error(char* str)
{
	fprintf(stderr, "%s: %s\n", str, strerror(errno));
	exit(1);
}

int open_listener_socket()
{
	int s = socket(PF_INET, SOCK_STREAM, 0);

	if (s == -1)
		error("Не удалось открыть сокет");

	return s;
}

void bind_to_port(int socket, int port)
{
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t) htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	int reuse = 1;

	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == -1)
		error("Не удалось установить параметры сокета");

	int c = bind(socket, (struct sockaddr*) &name, sizeof(name));
	
	if (c == -1)
		error("Не могу связаться с сокетом");
}

int read_in(int socket, char* buf, int len)
{
	char* s = buf;
	int slen = len;
	int c = recv(socket, s, slen, 0);

	while ((c > 0) && (s[c - 1] != '\n'))
	{
		s += c;
		slen -= c;
		c = recv(socket, s, slen, 0);
	}

	if (c < 0)
		return c;
	else if (c == 0)
		buf[0] = '\0';
	else
		s[c - 1] = '\0';

	return len - slen;
}

int say(int socket, char* str)
{
	int res = send(socket, str, strlen(str), 0);
	if (res == -1)
		fprintf(stderr, "%s: %s\n", "Ошибка при общении с клиентом", strerror(errno));
	return res;
}

int catch_signal(int sig, void (*handler) (int))
{
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction(sig, &action, NULL);
}

void shut(int sig)
{
	if (listener_d)
		close(listener_d);

	printf("Server has been closed by signal\n");
	exit(0);
}

int main()
{
	char str[150];

	if (catch_signal(SIGINT, shut) == - 1)
	{
		fprintf(stderr, "Не могу подобрать обработчик");
		return 1;
	}

	listener_d = open_listener_socket();

	bind_to_port(listener_d, 30000);

	if (listen(listener_d, 10) == -1)
		error("Не могу прослушать порт");

	printf("Ожидание подключения\n");

	int pid = -1;
	int connect = 0;

	while (1)
	{
		int connect_d;
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);

		connect_d = accept(listener_d, (struct sockaddr*) &client_addr, &address_size);

		if (connect_d == -1)
			error("Не могу открыть второй сокет");

		pid = fork();

		if (!pid)
		{
			close(listener_d);
			char* msg = "Server: Hello!\n";
			char str[64];

			read_in(connect_d, str, 63);
			printf("Msg from client: %s\n", str);
			say(connect_d, msg);
			close(connect_d);
			return 0;
		}

		close(connect_d);
	}

	return 0;
}