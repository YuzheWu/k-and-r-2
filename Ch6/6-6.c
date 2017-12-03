/* K&R2 Exercise 6-6 by Yuzhe Wu
 * Implement a simple version of the #define processor (i.e., no
 * arguments) suitable for use with C programs, based on the routines of
 * this section. You may also find getch and ungetch helpful.
 *
 * This simple version of preprocessor only handles #define and #undef
 * statements. Macro expansion does not take place inside string or
 * character constants. However, comments are not handled and various
 * buffers used for new words, macro names and definitions are assumed
 * to always have sufficient storage. Finally, recursive expansion is
 * not supported.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASHSIZE 101		/* size of hashtab */
#define MAXWORD 100			/* max word length */
#define MAXNAME 100			/* max name length */
#define MAXDEFN 1000		/* max definition length */
#define INSUFFBUF EOF - 1	/* signal for insufficient buffer space */

/* data structure for storing macro definitions */
struct nlist {		/* table entry */
	struct nlist *next;		/* next entry in chain */
	char *name;				/* defined name */
	char *defn;				/* replacement text*/
};
static struct nlist *hashtab[HASHSIZE]; /* pointer table */


int getword(char *word, int lim);
int getname(char *name, int lim);
void getdefn(char *defn, int lim);
struct nlist *lookup(char *s);
struct nlist *install(char *name,char *defn);
void undef(char *s);
int isdef(char *s);
int isundef(char *s);

/* take input code, substitute defined macros and print processed text */
int main() {
	char word[MAXWORD];
	struct nlist *p;
	char name[MAXNAME];
	char defn[MAXDEFN];
	int c, signal;
	while ((signal = getword(word, MAXWORD)) != EOF && signal != INSUFFBUF) {
		if ((p = lookup(word)) != NULL)	/* is predefined macro */
			printf("%s", p->defn);
		else if (isdef(word)) {		/* is macro definition */
			if (getname(name, MAXNAME) == 0)
				printf("error: macro name missing\n");
			getdefn(defn, MAXDEFN);
			install(name, defn);	/* define macro */
		}
		else if (isundef(word))	{	/* is undef statement */
			if (getname(name, MAXNAME) == 0)
				printf("error: macro name missing\n");
			getdefn(defn, MAXDEFN); /* get rid of extra tokens */
			undef(name);
		}
		else	/* normal word */
			printf("%s", word);
	}
	return 0;
}

int getch(void);
void ungetch(int);

/*
 * getword: get next word or character from input
 * String or character constants are treated as single words.
 * Return leading character or signal insufficient bufferspace.
 */
int getword(char *word, int lim) {
	int c;
	char *w = word;

	if ((c = getch()) != EOF)
		*w++ = c;
	if  (c == '"') {	/* get string constant */
		--lim;
		while ((*w = getch()) != '"') {
			if (--lim == 0 || (*w == '\\' && lim == 1)) {	/* no buffer space */
				printf("error: insufficient buffer space\n");
				if (*w == '\\')
					getch();
				while ((c = getch()) != '"') {	/* skip rest of string constant */
					if (c == '\\')
						getch();
				}
				return INSUFFBUF;			/* signal error */
			}
			if (*w == '\\') {
				*++w = getch();
				lim--;
			}
			w++;
		}
		*++w = '\0';
		return word[0];
	}
	if  (c == '\'') {	/* get char constant */
		--lim;
		while ((*w = getch()) != '\'') {
			if (--lim == 0 || (*w == '\\' && lim == 1)) {
				printf("error: insufficient buffer space\n");
				if (*w == '\\')
					getch();
				while ((c = getch()) != '\'') {
					if (c == '\\')
						getch();
				}
				return INSUFFBUF;
			}
			if (*w == '\\') {
				*++w = getch();
				lim--;
			}
			w++;
		}
		*++w = '\0';
		return word[0];
	}
	if (!isalpha(c) && c != '#' && c != '_') {	/* get single character */
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++) {	/* get identifier */
		if (!isalnum(*w = getch()) && *w != '_') {
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	if (lim == 0) {		/* no buffer space */
		printf("\nerror: insufficient buffer space\n");
		while (isalnum(c = getch()) || c == '_')
			;
		ungetch(c);
		return INSUFFBUF;
	}
	return word[0];
}

/*
 * getname: get macro name
 * return 1 for success, 0 for missing name
 */
int getname(char *name, int lim) {
	int c;

	while (isspace(c = getch()) && c != '\n')
		;
	if (!isalpha(c) && c != '_') {
		ungetch(c);
		return 0;
	}
	*name++ = c;
	for (; --lim > 0; name++) {
		if (!isalnum(*name =getch()) && *name != '_') {
			ungetch(*name);
			break;
		}
	}
	*name = '\0';
	return 1;
}

/*
 * getdefn: get macro definition up to newline
 * put empty string by default
 */
void getdefn(char *defn, int lim) {
	int c;

	while (isspace(c = getch()) && c != '\n')
		;
	*defn = c;
	while (--lim > 0) {
		if (*defn == '\n' || *defn == EOF)
			break;
		*++defn = getch();
	}
	if (*defn != '\n')
		ungetch(*defn);
	*defn = '\0';
}

/* undef: remove name from the hashtab if already defined */
void undef(char *name) {
	unsigned hash(char *);
	struct nlist *np1, *np2;
	unsigned hashval = hash(name);

	for (np1 = hashtab[hashval], np2 = NULL; np1 != NULL; np2 = np1, np1 = np1->next) {
		if (strcmp(name, np1->name) == 0) {		/* remove name and defn */
			free((void *) np1->name);
			free((void *) np1->defn);
		}
		if (np2 == NULL)	/* name found at head of list */
			hashtab[hashval] = np1->next;
		else
			np2->next = np1->next;
		free((void *) np1);
	}
}

/* isdef: check for #define keyword */
int isdef(char *s) {
	return (strcmp(s, "#define") == 0);
}

/* isundef: check for #undef keyword */
int isundef(char *s) {
	return (strcmp(s, "#undef") == 0);
}


/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

/* hash: from K&R2 page 144, form hash value for string s */
unsigned hash(char *s) {
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/* lookup: from K&R2 page 145, look for s in hashtab */
struct nlist *lookup(char *s) {
	unsigned hash(char *);
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0)
			return np;	/* found */
	}
	return NULL;		/* not found */
}

/* install: from K&R2 page 145, put (name, defn) in hashtab */
struct nlist *install(char *name,char *defn) {
	unsigned hash(char *);
	struct nlist *lookup(char *);
	struct nlist *np;
	unsigned hashval;

	if((np = lookup(name))== NULL) {	/* not found */
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name))==NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	else		/* already there */
		free((void *) np->defn);
	if((np->defn = strdup(defn))==NULL)
		return NULL;
	return np;
}

/* getch and ungetch: from K&R2 page 79 */
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
