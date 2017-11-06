/* K&R2 Exercise 6-5 by Yuzhe Wu
 * Write a function undef that will remove a name and definition from
 * the table maintained by lookup and install.
 *
 * Remark: Using the lookup function for retrieving the block to be
 * removed does not help, because the its precedent needs to be updated
 * as well.
 */

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
