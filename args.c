#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int i;
	char c;

	while ((c = getopt(argc, argv, "a:b")) != EOF)
		switch (c)
		{
			case 'a':
				printf("a: %s\n", optarg);
			break;

			case 'b':
				printf("b is exist\n");
			break;

			default:
				fprintf(stderr, "Неизвестный параметр: '%c'\n", optarg);
				return 1;
		}

	argc -= optind;
	argv += optind;

	for(i = 0; i < argc; ++i)
		printf("%d: %s\n", i + 1, *(argv + i));


	return 0;
}
