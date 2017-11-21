/*
 * K&R2 Exercise 7-4 by Yuzhe Wu
 * Write a private version of scanf analogous to minprintf from the
 * previous section.
 *
 * This solution handles formatting for integer, float and string
 * using the sscanf library function and matched literal characters.
 * Optional format specification is not supported.
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#define MAXVALLEN 100	/* max field length for formatting */

/* minscanf: minimal scanf with variable argument list */
void minscanf(char *fmt, ...) {
	int getch(void);
	void ungetch(int);

	va_list ap;
	void *inp;
	char *p, *sval, field[MAXVALLEN];
	int c, count = 0;

	va_start(ap, fmt);
	for (p = fmt; *p; p++) {
		while (isspace(*p))		/* ignore white spaces in format string */
			p++;
		if (*p != '%') {		/* match literal chars */
			while(isspace(c = getch()))
				;
			while (!isspace(*p) && *p != '%' && *p) {
				if (c != *p)
					return;
				p++;
				c = getch();
			}
			p--;
			ungetch(c);
			continue;
		}
		inp = (void *) va_arg(ap, void *);
		while(isspace(c = getch()))
			;
		count = 0;
		switch (*++p) {			/* format input */
			case 'd':
				if (c == '-') {
					field[count++] = c;
					c = getch();
				}
				while (isdigit(c)) {
					field[count++] = c;
					c = getch();
				}
				field[count] = '\0';
				ungetch(c);
				sscanf(field, "%d", (int *) inp);
				break;
			case 'f':
				if (c == '-') {
					field[count++] = c;
					c = getch();
				}
				while (isdigit(c)) {
					field[count++] = c;
					c = getch();
				}
				if (c == '.') {
					field[count++] = c;
					c = getch();
					while (isdigit(c)) {
						field[count++] = c;
						c = getch();
					}
				}
				field[count] = '\0';
				ungetch(c);
				sscanf(field, "%f", (float *) inp);
				break;
			case 's':
				sval = (char *) inp;
				while (!isspace(c) && c != EOF) {
					*sval++ = c;
					c = getch();
				}
				ungetch(c);
				break;
			default:
				continue;
		}
	}
}

/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

/* getch and ungetch are from K&R2, page 79 */
#define BUFSIZE 100

char buf[BUFSIZE];  /* buffer for ungetch() */
int bufp = 0;       /* next free position in buf */

int getch(void) { /* get a (possibly pushed back) character */
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {  /* push character back on input */
	if(bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
	return;
}
