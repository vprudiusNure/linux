#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void startAlarm(int sig)
{
	printf("[Sig: %d] Закрою программу через 3 секунды\n", sig);
	alarm(3);
}

void winch(int sig)
{
	printf("[Sig: %d] Смена размера рамки окна\n", sig);
}

void tio(int sig)
{
	printf("[Sig: %d] Прошло 3 секунды\n", sig);
	exit(0);
}

int setSignal(int sig, void (*handler) (int))
{
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction(sig, &action, NULL);
}

int main()
{
	if (setSignal(SIGINT, startAlarm) == - 1)
	{
		fprintf(stderr, "Не могу подобрать обработчик");
		return 1;
	}

	if (setSignal(SIGWINCH, winch) == - 1)
	{
		fprintf(stderr, "Не могу подобрать обработчик");
		return 1;
	}

	if (setSignal(SIGALRM, tio) == - 1)
	{
		fprintf(stderr, "Не могу подобрать обработчик");
		return 1;
	}

	printf("Ожидаю сигнал...\n");
	while (1);

	return 0;
}
