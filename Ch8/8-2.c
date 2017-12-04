/*
 * K&R2 Exercise 8-1 by Yuzhe Wu
 * Rewrite fopen and _fillbuf with fields instead of explicit bit oper-
 * ations. Compare code size and execution speed.
 *
 * To avoid confusion with standard library function names, my implem-
 * entation has a suffix 'x'. Tests on opening and reading files does
 * not show difference in speed between two versions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define PERMS 0666		/* RW for owner, group, others */

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

#define stdinx 	(&_iob[0])
#define stdoutx	(&_iob[1])
#define stderrx	(&_iob[2])

int _fillbufx(FILEx *);
// int _flushbufx(int, FILEx *);

#define feofx(p)	((p)->flag._EOF == 1)
#define ferrorx(p)	((p)->flag._ERR == 1)
#define filenox(p)	((p)->fd)

#define getcx(p)	(--(p)->cnt >= 0 \
				? (unsigned char) *(p)->ptr++ : _fillbufx(p))
// #define putcx(x, p)	(--(p)->cnt >= 0 \
// 				? *(p)->ptr++ = (x) : _flushbufx((x), (p)))

#define getcharx()	getcx(stdinx)
// #define putcharx(x)	putcx((x), stdoutx)

/* fopen implementation with bit fields */
FILEx *fopenx(char *name, char *mode) {
	int fd;
	FILEx *fp;

	if (*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULLx;
	for (fp = _iob; fp < _iob + OPEN_MAXx; fp++) {
		if (fp->flag._READ == 0 && fp->flag._WRITE == 0)
			break;
	}
	if (fp >= _iob + OPEN_MAXx)		/* no free slots */
		return NULLx;

	if (*mode == 'w')
		fd = creat(name, PERMS);
	else if (*mode == 'a') {
		if ((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	}
	else
		fd = open(name, O_RDONLY, 0);
	if (fd == -1)
		return NULLx;
	fp->fd = fd;
	fp->cnt = 0;
	fp -> base = NULLx;
	if (*mode == 'r')
		fp->flag._READ = 1;
	else
		fp->flag._WRITE = 1;
	return fp;
}

/* _fillbuf implementation with bit fields */
int _fillbufx(FILEx *fp){
	int bufsize;

	if (fp->flag._READ != 1 || fp->flag._EOF == 1 || fp->flag._ERR == 1)
		return EOFx;
	bufsize = (fp->flag._UNBUF) ? 1 : BUFSIZx;
	if (fp->base == NULLx) {
		if ((fp->base = (char *) malloc(bufsize)) == NULL)
			return EOFx;
	}
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if(--fp->cnt < 0) {
		if(fp->cnt == -1)
			fp->flag._EOF = 1;
		else
			fp->flag._ERR = 1;
		fp->cnt = 0;
		return EOFx;
	}
	return (unsigned char) *fp->ptr++;
}

FILEx _iob[OPEN_MAXx] = {	 /* stdin, stdout, stderr */
	{ 0, (char *) 0, (char *) 0, {1, 0, 0, 0, 0}, 0},
	{ 0, (char *) 0, (char *) 0, {0, 1, 0, 0, 0}, 1},
	{ 0, (char *) 0, (char *) 0, {0, 1, 1, 0, 0}, 2}
};

/* open and print file with new I/O implementation */
int main() {
	int c;
	FILEx *fp;

	clock_t begin = clock();
	fp = fopenx("8-2.c", "r");
	while ((c = getcx(fp)) != EOFx)
		putchar(c);
	clock_t end = clock();
	printf("\nTIME ELAPSED: %lu clocks\n", end - begin);
}
