/*
 * K&R2 Exercise 7-8 by Yuzhe Wu
 * Write a program to print a set of files, starting each new one on a
 * new page, with a title and a running page count for each file.
 */

#include <stdio.h>

#define MAXLINE 1000
#define LINES_PER_PAGE 15

void print_file(FILE *iop, char *fname);

int main(int argc, char *argv[]) {
	char *prog = argv[0];
	FILE *iop;

	if (argc == 1) {
		fprintf(stderr, "%s: no file to print\n", prog);
			return 1;
	}

	for (int i = 1; i < argc; i++) {
		if ((iop = fopen(argv[i], "r")) == NULL) {
			fprintf(stderr, "%s: can't open %s\n", prog, argv[i]);
			return 1;
		}
		printf("Printing %s...\n", argv[i]);
		print_file(iop, argv[i]);
	}
	return 0;
}

void print_file(FILE *iop, char *fname) {
	char line[MAXLINE];
	int line_count = 0;
	int page_count = 0;

	printf("========== %s ==========\n", fname);
	while (fgets(line, MAXLINE, iop) != NULL) {
		line_count++;
		printf("%s", line);
		if (line_count == LINES_PER_PAGE) {
			page_count++;
			line_count = 0;
			printf("======= END OF PAGE %d =======\n", page_count);
		}
	}
	if (line_count > 0) {
		page_count++;
		printf("======= END OF PAGE %d =======\n", page_count);
	}
	printf("======= END OF %s =======\n\n", fname);
}
