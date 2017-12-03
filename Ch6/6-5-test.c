/* K&R2 Exercise 6-5 with test by Yuzhe Wu
 * Write a function undef that will remove a name and definition from
 * the table maintained by lookup and install.
 *
 * This file provides a test routine for my implementation of undef.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

/* data structure for storing macro definitions */
struct nlist {		/* table entry */
	struct nlist *next;		/* next entry in chain */
	char *name;				/* defined name */
	char *defn;				/* replacement text*/
};
static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* test routine for the procedure undef */
int main() {
	void v_undef(char *s);
	struct nlist *v_install(char *name,char *defn);
	struct nlist *v_lookup(char *s);

	printf("Starting test...\n");
	v_install("a", "1");
	v_install("b", "2");
	v_lookup("a");
	v_lookup("b");
	v_undef("a");
	v_lookup("a");
	v_lookup("b");
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

/* v_undef: verbose version of undef */
void v_undef(char *s) {
	void undef(char *s);
	struct nlist *lookup(char *s);

	printf("Undefining %s...\n", s);
	undef(s);
	if (lookup(s) == NULL)
		printf("%s undefined with success!\n", s);
	else
		printf("Undefining failed.\n");
	printf("\n");
}

/* v_install: verbose version of install */
struct nlist *v_install(char *name,char *defn) {
	struct nlist *install(char *name,char *defn);
	struct nlist *lookup(char *s);
	struct nlist *p1;
	struct nlist *p2;

	printf("Defining %s to be %s...\n", name, defn);
	p1 = install(name, defn);
	if ((p2 = lookup(name)) != NULL && strcmp(p2->defn, defn) == 0)
		printf("%s defined to be %s with success!\n", name, defn);
	else
		printf("Installation failed.\n");
	printf("\n");
	return p1;
}

/* v_lookup: verbose version of lookup */
struct nlist *v_lookup(char *s) {
	struct nlist *lookup(char *s);
	struct nlist *p;

	printf("Looking up %s...\n", s);
	p = lookup(s);
	if (p == NULL)
		printf("%s undefined in lookup table\n", s);
	else
		printf("Definition for %s: %s\n", s, p->defn);
	printf("\n");
	return p;
}

/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

/* hash: from K&R2 page 144. Form hash value for string s */
unsigned hash(char *s) {
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/* lookup: from K&R2 page 145. Look for s in hashtab */
struct nlist *lookup(char *s) {
	unsigned hash(char *);
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0)
			return np;	/* found */
	}
	return NULL;		/* not found */
}

/* lookup: from K&R2 page 145. put (name, defn) in hashtab */
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

