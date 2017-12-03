/* K&R2 Exercise 6-4 by Yuzhe Wu
 * Write a program that prints the distict words in its input sorted
 * into decreasing order of frequency of occurrence. Procede each word
 * by its count.
 *
 * In this solution the word counting part is the same as in the book
 * using a binary tree for facilitating lookup of new words. We keep
 * track of the vocabulary size along the way. The resulting tree is
 * then flattened into an array on which we perform a quick sort by
 * frequency of occurrence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100		/* max word length */

struct tnode {		/* the tree node */
	char *word;				/* points to the text */
	int count;				/* no. of occurrences */
	struct tnode *left;		/* left child */
	struct tnode *right;	/* right child */
};

struct key {
	char *word;
	int count;
};

struct tnode *addtree(struct tnode *, char *);
int getword(char *, int);
struct key *flattentree(struct key keyarr[], struct tnode *tree);

int total = 0;	/* total number of distict words encountered */

/* print out words in descending frequency of occurrence */
int main() {
	char word[MAXWORD];		/* placeholder for new words */
	struct tnode *root = NULL;
	int comp(const void *, const void *);	/* comparison routine for qsort */

	while ((getword(word, MAXWORD)) != EOF) {
		if (isalpha(word[0])) {
			root = addtree(root, word);
		}
	}

	if (total > 0) {
		struct key words[total];

		flattentree(words, root);
		qsort(words, total, sizeof words[0], comp);
		for (int i = 0; i < total; i++)
			printf("%4d %s\n", words[i].count, words[i].word);
	}
}

/* comp: compare two key structs by word count */
int comp(const void *v1, const void *v2) {
	const struct key *key1 = v1, *key2 = v2;
	return key2->count - key1->count;
}

/*
 * flattentree: flatten tree into a key array pointed by keyarr and
 * return a pointer to the next avaliable position, assuming sufficient
 * space in keyarr.
 */
struct key * flattentree(struct key keyarr[], struct tnode *tree) {
	if (tree != NULL) {
		keyarr = flattentree(keyarr, tree->left);
		keyarr->word = tree->word;
		keyarr->count = tree->count;
		return flattentree(++keyarr, tree->right);
	}
	else
		return keyarr;
}

/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

 struct tnode *talloc(void);
// char *strdup(char *);

/*
 * addtree: from K&R2 page 141
 * add a node with w, at or below p, and update global variable total
 */
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if(p == NULL) {
		p = talloc();
		p->word = strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
		total++;
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

/*
 * getword: from K&R page 136
 * get next word or character from input
 */
int getword(char *word, int lim) {
	int c, getch(void);
	void ungetch(int);
	char *w = word;

	while (isspace(c = getch()))
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
