#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

#define DIRSIZE 14

int spname(char *path, char *newpath);
void error_msg(const char *msg);

int main(int argc, char *argv[]){
	char newpath[512];
	if (argc < 2) error_msg("need 1 argument at least!\n");
	switch (spname(argv[1], newpath)){
		case 0:
		printf("exactly %s.\n", newpath);
		break;
		case 1:
		printf("You mean %s right?\n", newpath);
		break;
		case -1:
		printf("don't find such a path:%s\n", argv[1]);
		break;
	}
	return 0;
}

int spdist(char *str1, char *str2){
	char *s1=str1, *s2=str2;
	while (*s1 == *s2){
		if (*s1 == '\0' && *s2 == '\0') break;
		s1++;s2++;
	}
	if (*s1 == '\0' && *s2 == '\0') return 0;
	else {
		if (strcmp(s1+1, s2+1) == 0) return 1;
		else if (strcmp(s1+1, s2) == 0) return 1;
		else if (strcmp(s1, s2+1) == 0) return 1;
		else if (*s1 == *(s2+1) && *(s1+1) == *s2 && strcmp(s1+2, s2+2) == 0) return 2;
	}
	return 3;
}

int mindist(char *path, char *guess, char *best){
	DIR *pdir;
	struct dirent *dent;
	int d,md=3;

	if ((pdir=opendir(path)) == NULL) return -1;
	while ((dent=readdir(pdir)) != NULL){
		d = spdist(dent->d_name, guess);
		if (d < 3) printf("\"%s\" match \"%s\" scores %d\n", dent->d_name, guess, d);	
		if (d < md) {
			strcpy(best, dent->d_name);
			md = d;
		}
		if (d == 0) return 0;
	}

	return 1;
}

int spname(char *path, char *newpath){
	char *p, guess[DIRSIZE+1], best[DIRSIZE+1]; 
	char *oldp=path, *newp=newpath;
	int res, sameflag = 1;

	for (;;){
		while (*oldp == '/') 
			*newp++ = *oldp++;
		*newp = '\0';
		if (*oldp == '\0') break;
		memset(guess, 0, sizeof(guess));
		p = guess;
		while (*oldp != '/' && *oldp != '\0')
			*p++ = *oldp++;
		*p = '\0';

		if ((res=mindist(newpath, guess, best)) < 0)
			return -1;
		else if (res > 0) sameflag = 0;
		p = best;
		while (*p) *newp++ = *p++;
	}
	if (sameflag) return 0;
	else return 1;	
}

void error_msg(const char *msg){
	fprintf(stderr, "%s", msg);
	exit(1);
}
