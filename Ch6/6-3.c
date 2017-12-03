/* K&R2 Exercise 6-3 by Yuzhe Wu
 * Write a cross-referencer that prints a list of all words in a
 * document, and, for each word, a list of the line numbers on which it
 * occurs. Remove noise words like "the", "and", and so on.
 *
 * This solustion uses linked lists to store line numbers for each word.
 * Repeated occurrences in the same line are kept. The tnode struct
 * comes from the book and is adapted to include the list.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100		/* max word length */

struct intlist {	/* linked list of integers */
	int val;
	struct intlist *next;
};

struct tnode {		/* the tree node */
	char *word;				/* points to the text */
	struct intlist *lines;	/* list of line numbers */
	struct tnode *left;		/* left child */
	struct tnode *right;	/* right child */
};

char *noisewords[] = {	/* the noise words need to be in ascending order for binary search to work */
	"a",
	"an",
	"and",
	"the"
};
#define NNOISEWORDS (sizeof noisewords / sizeof noisewords[0])
#define SIZENOISE sizeof noisewords[0]

int getword(char *, int);
struct tnode *addtree(struct tnode *, char *, int);
int isnoise(char *);
void printtree(struct tnode *);

/* print a list of words with line numbers for occurrences */
int main() {
	struct tnode *root = NULL;
	int line_count = 1;
	char word[MAXWORD];

	while (getword(word, MAXWORD) != EOF) {
		if (word[0] == '\n')
			line_count++;
		else if (isalpha(word[0]) && !isnoise(word))
			root = addtree(root, word, line_count);
	}
	printtree(root);
	return 0;
}

/* addtree: integrate the new occurrence of word (w) with line no. (line) into the binary tree (p) */
struct tnode *addtree(struct tnode *p, char *w, int line) {
	struct tnode *talloc(void);
	struct intlist *addlist(struct intlist *, int);
	int cond;

	if (p == NULL) {
		p = talloc();
		p->word = strdup(w);
		p->lines = NULL;
		p->lines = addlist(p->lines, line);
		p->left = p->right = NULL;
	}
	else if ((cond = strcmp(w, p->word)) == 0)
		p->lines = addlist(p->lines, line);
	else if(cond < 0)
		p->left = addtree(p->left, w, line);
	else
		p->right = addtree(p->right, w, line);
	return p;
}

/* addlist: add new value to the tail of list */
struct intlist *addlist(struct intlist *l, int val) {
	struct intlist *listalloc(void);

	if (l == NULL) {
		l = listalloc();
		l->val = val;
		l->next = NULL;
	}
	else
		l->next = addlist(l->next, val);
	return l;
}

/* listalloc: make an intlist */
struct intlist *listalloc(void)
{
	return (struct intlist *) malloc(sizeof(struct intlist));
}

/* isnoise: filter noise words */
int isnoise(char *w) {
	int mystrcmp(const void *, const void *);

	return bsearch(w, noisewords, NNOISEWORDS, SIZENOISE, mystrcmp) != NULL;
}

int mystrcmp(const void *s1, const void *s2) {
	const char *key = s1;
	const char * const *arg = s2;
	return strcmp(key, *arg);
}

/* printtree: print words along with lists of line numbers */
void printtree(struct tnode *p) {
	void printlist(struct intlist *);

	if (p != NULL) {
		printtree(p->left);
		printf("%s\n", p->word);
		printlist(p->lines);
		printf("\n");
		printtree(p->right);
	}
}

/* printlist: print list elements seperated by spaces */
void printlist(struct intlist *l) {
	if (l == NULL)
		printf("\n");
	else {
		printf("%d ", l->val);
		printlist(l->next);
	}
}

/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

/* talloc: From K&R2 page 142. Makes a tnode. */
struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}

/*
 * getword: from K&R page 136
 * get next word or character from input
 * modified so as no to skip newline
 */
int getword(char *word, int lim) {
	int c, getch(void);
	void ungetch(int);
	char *w = word;

	while (isspace(c = getch()) && c != '\n')
		;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c)) {
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++) {
		if (!isalnum(*w =getch())) {
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	return word[0];
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
