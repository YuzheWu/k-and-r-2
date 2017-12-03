/*
 * K&R2 Exercise 7-3 by Yuzhe Wu
 * Revise minprintf to handle more of the other facilities of printf
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#define MAXFORMATLEN 20

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...) {
	va_list ap;		/* points to each unnamed arg in turn */
	char *p, *sval, format[MAXFORMATLEN];
	int ival, count = 0;
	double dval;

	va_start(ap, fmt);	/* make ap point to 1st unnamed arg */
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		count = 0;
		format[count++] = '%';
		while ((*++p == '-') || (isdigit(*p)) || (*p == '.')) {
			format[count++] = *p;
		}
		switch (*p) {
			case 'd':
				ival = va_arg(ap, int);
				format[count++] = 'd';
				format[count] = '\0';
				printf(format, ival);
				break;
			case 'f':
				dval = va_arg(ap, double);
				format[count++] = 'f';
				format[count] = '\0';
				printf(format, dval);
				break;
			case 's':
				sval = va_arg(ap, char *);
				format[count++] = 's';
				format[count] = '\0';
				printf(format, sval);
				// for (sval = va_arg(ap, char *); *sval; sval++)
				// 	putchar(*sval);
				break;
			default:
				putchar(*p);
		}
	}
	va_end(ap);		/* clean up when done */
}
