/*
 * K&R2 Exercise 8-3 by Yuzhe Wu
 * Design and write _flushbuf, fflush, and fclose.
 *
 * To avoid confusion with standard library function names, my implem-
 * entation has a suffix 'x'.
 */

#include <stdlib.h>
#include <unistd.h>

/* data structures and constants */
#define NULLx		0
#define EOFx		(-1)
#define BUFSIZx		1024
#define OPEN_MAXx	20	/* max #files open at once */

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
extern FILEx _iob[OPEN_MAXx];

#define feofx(p)	((p)->flag._EOF == 1)
#define ferrorx(p)	((p)->flag._ERR == 1)
#define filenox(p)	((p)->fd)

/* _flushbuf implementation with bit fields */
int _flushbufx(int c, FILEx *fp) {
	int bufsize, num_written;

	if (fp->flag._WRITE != 1 || fp->flag._EOF == 1 || fp->flag._ERR == 1)
		return EOFx;
	bufsize = (fp->flag._UNBUF) ? 1 : BUFSIZx;
	if (fp->base == NULLx) {
		if ((fp->base = (char *) malloc(bufsize)) == NULL)
			return EOFx;
	}
	else {
		num_written = fp->ptr - fp->base;
		if (write(fp->fd, fp->base, num_written) < num_written) {
			fp->flag._ERR = 1;
			return EOFx;
		}
	}
	fp->ptr = fp->base;
	if (c == EOFx)
		return EOFx;
	*fp->ptr++ = c;
	fp->cnt = bufsize - 1;
	return c;
}

/* flush output buffer */
int fflushx(FILEx *fp) {
	int retval;

	retval = 0;
	if (fp == NULL) {
		for (int i = 0; i < OPEN_MAXx; i++) {
			if (_iob[i].flag._WRITE == 1 && fflushx(&_iob[i]) == -1)
				retval = -1;
		}
	}
	else {
		if (fp->flag._WRITE == 0)
			return -1;
		_flushbufx(EOFx, fp);
		if (ferrorx(fp))
			retval = -1;
	}
	return retval;
}

/* close file */
int fclosex(FILEx *fp) {
	int fd;

	if (fp == NULL)
		return -1;
	fflushx(fp);
	fd = fp->fd;
	fp->cnt = 0;
	fp->ptr = NULL;
	if (fp->base != NULL)
		free(fp->base);
	fp->base = NULL;
	fp->flag._READ = fp->flag._WRITE = fp->flag._UNBUF = fp->flag._EOF = fp->flag._ERR = 0;
	fp->fd = -1;
	return close(fd);
}

FILEx _iob[OPEN_MAXx] = {	 /* stdin, stdout, stderr */
	{ 0, (char *) 0, (char *) 0, {1, 0, 0, 0, 0}, 0},
	{ 0, (char *) 0, (char *) 0, {0, 1, 0, 0, 0}, 1},
	{ 0, (char *) 0, (char *) 0, {0, 1, 1, 0, 0}, 2}
};
