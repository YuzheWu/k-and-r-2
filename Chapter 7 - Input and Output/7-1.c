/*
 * K&R2 Exercise 7-1 by Yuzhe Wu
 * Write a program that converts upper case to lower or lower case to
 * upper, depending on the name it is invoked with, as found in argv[0].
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* converts upper case to lower, or conversly if invoked with name upper */
int main(int argc, char *argv[]) {
	int c;
	int (*func)(int) = tolower;

	if (strcmp(argv[0], "upper") == 0)
		func = toupper;
	while ((c = getchar()) != EOF)
		putchar(func(c));
	return 0;
}
