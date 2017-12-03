/*
 * K&R2 Exercise 7-6 by Yuzhe Wu
 * Write a program to compare two files, printing the first line where
 * they differ
 */

#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

int main(int argc, char *argv[]) {
	char line1[1000];
	char line2[1000];
	char *prog = argv[0];	/* program name for errors */

	if (argc < 3) {
		fprintf(stderr, "%s: require two files for comparison\n", prog);
		return 1;
	}

	FILE *iop1 = fopen(argv[1], "r");
	FILE *iop2 = fopen(argv[2], "r");
	if (iop1 == NULL) {
		fprintf(stderr, "%s: can't open %s\n", prog, argv[1]);
		return 1;
	}
	else if (iop2 == NULL) {
		fprintf(stderr, "%s: can't open %s\n", prog, argv[2]);
		return 1;
	}

	int count = 0;
	while (fgets(line1, MAXLINE, iop1) != NULL) {
		count++;
		if (fgets(line2, MAXLINE, iop2) == NULL || strcmp(line1, line2) != 0) {
			printf("Line no. %d in %s:\n", count, argv[1]);
			printf("%s\n", line1);
			return 0;
		}
	}
	if (fgets(line2, MAXLINE, iop2) != NULL) {
		printf("Line no. %d in %s:\n", ++count, argv[2]);
		return 0;
	}

	printf("%s\n", "Identical files");
	return 0;
}
