/*
 * K&R2 Exercise 7-2 by Yuzhe Wu
 * Write a program that will print arbitrary input in a sensible way. As
 * a minimum, it should print non-graphic characters in octal or hexa-
 * decimal according to local custom, and break long text lines.
 *
 * In this implementation, non-graphic characters are converted to hexa-
 * decimals with leading 0x and delimited by '\'.
 */

#include <stdio.h>
#include <ctype.h>

#define MAXLEN 50

int main() {
	int c, count = 0;

	while ((c = getchar()) != EOF) {
		if (count >= MAXLEN) {
			putchar('\n');
			count = 0;
		}
		if (c == '\n') {
			putchar(c);
			count = 0;
		}
		else if (!isgraph(c)) {
			count += 6;
			if (count > MAXLEN) {
				putchar('\n');
				count = 6;
			}
			printf("\\0x%.2x\\", c);
		}
		else {
			count++;
			putchar(c);
		}
	}
}
