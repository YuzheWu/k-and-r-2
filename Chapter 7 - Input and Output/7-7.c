/*
 * K&R2 Exercise 7-7 by Yuzhe Wu
 * Modify the pattern finding program of Chapter 5 to take its input
 * from a set of named files or, if no files are named as arguments,
 * from the standard input. Should the file name be printed when a
 * matching line is found?
 */

#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

int main(int argc, char *argv[]) {
	char line[MAXLINE];
	long lineno;
	int c, except = 0, number = 0;
	char *prog = argv[0];
	char *pattern;
	FILE *iop;

	while (--argc > 0 && (*++argv)[0] == '-') {	/* process flags */
		while ((c = *++argv[0])) {
			switch (c) {
				case 'x':
					except = 1;		/* find lines not matching pattern */
					break;
				case 'n':
					number = 1;		/* print line number for matching lines */
					break;
				default:
					printf("find: illegal option %c\n", c);
					argc = 0;
					break;
			}
		}
	}

	if (argc == 0) {
		printf("Usage: %s -x -n pattern\n", prog);
		return 0;
	}

	pattern = *argv;
	if (argc == 1) {		/* read from stdin */
		lineno = 0;
		while (fgets(line, MAXLINE, stdin) != NULL) {
			lineno++;
			if ((strstr(line, pattern) != NULL) != except) {
				if (number)
					printf("Line %ld in %s: \n", lineno, *argv);
				printf("%s\n", line);
			}
		}
	}
	while (--argc > 0) {	/* read from files */
		if ((iop = fopen(*++argv, "r")) == NULL) {
			fprintf(stderr, "%s: can't open %s\n", prog, *argv);
			return 1;
		}
		lineno = 0;
		while (fgets(line, MAXLINE, iop) != NULL) {
			lineno++;
			if ((strstr(line, pattern) != NULL) != except) {
				if (number)
					printf("Line %ld in %s: \n", lineno, *argv);
				printf("%s\n", line);
			}
		}
	}
	return 0;
}
