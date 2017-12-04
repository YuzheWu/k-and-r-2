/*
 * K&R2 Exercise 8-4 by Yuzhe Wu
 * The standard library function
 *
 * int fseek (FILE *fp, long offset, int origin)
 *
 * is identical to lseek except that fp is a file pointer instead of a
 * file descriptor and the return value is an int status, not a pos-
 * ition. Write fseek. Make sure that your fseek coordinates properly
 * with the buffering done for the other functions of the library.
 *
 * To avoid confusion with standard library function names, my implem-
 * entation has suffix 'x' appended.
 */

#include <unistd.h>

/* file pointer structure */
typedef struct {	/* flag structure with bit fields */
	unsigned int _READ 	: 1;
	unsigned int _WRITE : 1;
	unsigned int _UNBUF	: 1;
	unsigned int _EOF	: 1;
	unsigned int _ERR	: 1;
} flag;

typedef struct {
	int cnt;		/* characters left */
	char *ptr;		/* next character position */
	char *base;		/* location of buffer */
	flag flag;		/* mode of file access */
	int fd;			/* file descriptor */
} FILEx;

/* fseek with buffering compatibility */
int fseekx(FILEx *fp, long offset, int origin) {
	if (fp == NULL)
		return -1;
	if (lseek(fp->fd, offset, origin) == -1)
		return -1;
	fp->cnt = 0;
	fp->flag._EOF = 0;
	return 0;
}
