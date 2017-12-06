/*
 * K&R2 Exercise 8-5 by Yuzhe Wu
 * Modify the fsize program to print the other information contained in
 * the inode entry
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

void finfo(char *);

/* print file info */
int main (int argc, char **argv) {
	if (argc == 1)
		finfo(".");
	else {
		while (--argc > 0)
			finfo(*++argv);
	}
	return 0;
}

void dirwalk(char *, void (*fcn)(char *));

/* finfo: print size and other info of file "name" */
void finfo(char *name) {
	struct stat stbuf;
	char t_buff1[20], t_buff2[20];
	struct tm *t_info1, *t_info2;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s\n", name);
		return;
	}
	if (S_ISDIR(stbuf.st_mode))
		dirwalk(name, finfo);
	t_info1 = localtime(&stbuf.st_atime);
	t_info2 = localtime(&stbuf.st_mtime);
	strftime(t_buff1, sizeof(t_buff1), "%Y %b %d %H:%M", t_info1);
	strftime(t_buff2, sizeof(t_buff2), "%Y %b %d %H:%M", t_info2);

	printf("%s\n", name);
	printf("         size: %lld bytes\n", stbuf.st_size);
	printf("last accessed: %s\n", t_buff1);
	printf("last modified: %s\n", t_buff2);
	printf("\n");
}

#define MAX_PATH 1024

/* dirwalk: apply fcn to all files in dir */
void dirwalk(char *dir, void (*fcn)(char *)) {
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0
		    || strcmp(dp->d_name, "..") == 0)
		    continue;
		if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}
