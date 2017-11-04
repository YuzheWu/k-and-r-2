/* K&R2 Exercise 6-2 by Yuzhe Wu
 *
 * Write a program that reads a C program and prints in alphabetical
 * order each group of variable names that are identical in the first
 * 6 characters, but different somewhere thereafter.  Don't count words
 * within strings and comments.  Make 6 a parameter that can be set
 * from the command line.
 *
 * This solution takes advantage of the fact that elements of a tnode
 * tree are already stored in order. We only need to keep track of when
 * the next word falls into a different n-char group, which happens
 * when the comparison (up to the first n chars) yields an inequality.
 * The tracking is accomplished through a static pointer to the
 * previous word encountered.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100		/* max word length */
#define COMPLIM 6		/* default no. of chars for comparison */

struct tnode {		/* the tree node */
	char *word;				/* points to the text */
	int count;				/* no. of occurrences */
	struct tnode *left;		/* left child */
	struct tnode *right;	/* right child */
};

struct tnode *addtree(struct tnode *, char *);
void groupprint(struct tnode *, int);
int getword(char *, int);

/* print out words in input file grouped by n-char headings */
int main(int argc, char *argv[]) {
	struct tnode *root;
	char word[MAXWORD];

	int complim = COMPLIM;
	if (argc > 1)
		complim = atoi(argv[1]);

	root = NULL;
	while (getword(word, MAXWORD) != EOF) {
		if (isalpha(word[0]))
			root = addtree(root, word);
	}
	groupprint(root, complim);
	return 0;
}

/* groupprint: in-order print of p by group */
void groupprint(struct tnode *p, int complim) {
	static struct tnode *prevNode = NULL;	/* node encountered last time */
	static int groupcount = 0;

	if (p != NULL) {
		groupprint(p->left, complim);
		if (prevNode == NULL || strncmp(p->word, prevNode->word, complim) != 0) {	/* new group arrives */
			groupcount++;
			printf("\nGroup %d\n", groupcount);
		}
		printf("%4d %s\n", p->count, p->word);
		prevNode = p;
		groupprint(p->right, complim);
	}
}

int getch(void);
void ungetch(int);

/* getword: get next token, skipping comments and strings */
int getword(char *word, int lim) {
	int c, c2;
	char *w = word;

	/* skip spaces */
	while (isspace(c = getch()))
		;

	/* skip comments */
	if (c == '/') {
		if ((c = getch()) == '/') {
			while ((c = getch()) != '\n')
				;
			while (isspace(c = getch()))
				;
		}
		else if (c == '*') {
			c = getch();
			c2 = getch();
			while (!(c == '*' && c2 == '/')) {
				c = c2;
				c2 = getch();
			}
			while (isspace(c = getch()))
				;
		}
		else {
			ungetch(c);
			*w++ = '/';
			*w = '\0';
			return '/';
		}
	}

	/* skip string constants */
	if (c == '"') {
		c2 = getch();
		while (c == '\\' || c2 != '"') {
			c = c2;
			c2 = getch();
		}
		while (isspace(c = getch()))
				;
	}

	/* skip characters */
	if (c == '\'') {
		c2 = getch();
		while (c == '\\' || c2 != '\'') {
			c = c2;
			c2 = getch();
		}
		while (isspace(c = getch()))
				;
	}

	if (c != EOF)
		*w++ = c;
	if (!isalpha(c) && c != '_') {	/* c is a single char */
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++) {		/* c is part of a name */
		if (!isalnum(*w = getch()) && *w != '_') {
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	return word[0];
}

/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

struct tnode *talloc(void);

/*
 * addtree: from K&R2 page 141
 * add a node with w, at or below p
 */
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if(p == NULL) {
		p = talloc();
		p->word = strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	}
	else if((cond = strcmp(w, p->word)) == 0)
		p->count++;
	else if(cond < 0)
		p->left = addtree(p->left, w);
	else
		p->right = addtree(p->right, w);
	return p;
}

/* talloc: From K&R2 page 142. Makes a tnode. */
struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}

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

