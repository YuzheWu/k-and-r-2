/*
 * K&R2 Exercise 8-1 by Yuzhe Wu
 * Rewrite the program cat from Chapter 7 using read, write, open and
 * close instead of their standard library equivalents. Perform experi-
 * ments to determine the relative speeds of the two version.
 *
 * Comparison with the version in Chapter 7 shows that using system
 * to read and write is faster than using standard library functions.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void error(char *, ...);

int main(int argc, char *argv[]) {
	int fd;
	char buf[BUFSIZ];
	int n;

	clock_t begin = clock();
	if (argc == 1) {	/* copy from stdin to stdout */
		while ((n = read(0, buf, BUFSIZ)) > 0) {
			if (write(1, buf, n) < n)
				error("cat: error writing stdout");
		}
		return 0;
	}
	else {
		while (--argc > 0) {
			if ((fd = open(*++argv, O_RDONLY, 0)) == -1)
				error("cat: can't open %s", *argv);
			while ((n = read(fd, buf, BUFSIZ)) > 0) {
				if (write(1, buf, n) < n)
					error("cat: error writing stdout");
			}
		}
	}
	clock_t end = clock();
	printf("\nTIME ELAPSED: %lu clocks\n", end - begin);

	return 0;
}

/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

/*
 * error: from K&R2, page 173
 * print an error message and die
 */
void error(char *fmt, ...) {
	va_list args;

	va_start(args, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}
