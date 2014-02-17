/*
 * Exercise 6-5. Write a function undef that will remove a name and definition
 * from the table maintained by lookup and install.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* table entry: */
struct nlist {
	struct nlist *next; /* next entry in chain */
	char *name; /* defined name */
	char *defn; /* replacement text */
};

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];	/* pointer table */

struct nlist *install(char *name, char *defn);
void printhash();
char *undef(char *);

int main()
{
	install("sami", "engineer");
	install("rabab", "is a housewife");
	install("sami", "is writing c code");
	install("Y", "is a letter");	/* Here both Y and sami have the same hash*/
	printhash();

	printf("now deleting ");
	char *defn = undef("Y");

	printf("%s\n", defn);	
	free(defn);

	printf("\n\nwe got a new hash table\n");
	printhash();
	getchar();
	return 0;
}

/* hash: form hash value for string s */
unsigned hash(char *s)
{
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
	struct nlist *np;
	int str_cmp(char *, char *);
	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (str_cmp(s, np->name) == 0)
			return np; /* found */
	return NULL; /* not found */
}

char *str_dup(char *);

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;
	if ((np = lookup(name)) == NULL) {
		/* not found */
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = str_dup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}
	else /* already there */
		free((void *) np->defn); /*free previous defn */
	if ((np->defn = str_dup(defn)) == NULL)
		return NULL;
	return np;
}

void freenp(struct nlist *np)
{
	free((void*) np->name);
	free((void*) np->defn);
	free((void*) np);
}


char *undef(char *name)
{
	struct nlist *np, *backup = NULL;
	char* defn;
	unsigned hashval = hash(name);

	if ((np = lookup(name)) == NULL)
		return NULL;
	for (np = hashtab[hashval]; np != NULL; np = np->next)
	{
		if (strcmp(np->name, name) == 0)
		{
			if (NULL == backup)	/* first node */
				hashtab[hashval] = np->next;
			else				/* mid node */
				backup->next = np->next;
			defn = str_dup(np->defn);
			
			free(np->name);
			free(np->defn);
			free(np);

			return defn;
		}
		backup = np;
	}
	return NULL;
}

void printhash()
{
	struct nlist *np;
	int i;
	for (i = 0; i < HASHSIZE; i++)
	{
		if (hashtab[i])
		{
			np = hashtab[i];
			printf("%s = %s", np->name, np->defn);
			for (np = np->next; np; np = np->next)
				printf(" -> %s = %s", np->name, np->defn);
			putchar('\n');
		}
	}
}

void str_cpy(char *s, char *t);
/* make a duplicate of s */
char *str_dup(char *s)
{
	char *p;
	p = (char *) malloc(strlen(s) + 1); /* +1 for '\0' */
	if (p != NULL)
		str_cpy(p, s);
	return p;
}

/* strcmp: returns <0 if s<t, 0 if s==t, >0 if s>t */
int str_cmp(char *s, char *t)
{
	for (; *s == *t; s++, t++)
		if (*s == '\0')
			return 0;
	return *s - *t;
}

/* strcpy: copy t to s; pointer version 3 */
void str_cpy(char *s, char *t)
{
	while (*s++ = *t++)
		;
}