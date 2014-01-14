/*
 * Exercise 6-6. Implement a simple version of the #define processor 
 * (i.e., no arguments) suitable for use with C programs, 
 * based on the routines of this section. 
 * You may also find getch and ungetch helpful.
 */


/*
      the program will read a c program,
	  if we found a #define, we 'll save its name and its replacemet text.
	  if we found a defined name, we 'll replace it with its replacement text
	  if we found a comment, we 'll print it as it is (i.e. no replacement).
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* table entry: */
struct nlist {
	struct nlist *next; /* next entry in chain */
	char *name; /* defined name */
	char *defn; /* replacement text */
};

#define HASHSIZE 101
#define MAXWORD 100

static struct nlist *hashtab[HASHSIZE];	/* pointer table */

int getch();
void ungetch(int);
int definedirective();
int printcomment();
int parseword();
int printquote();

main()
{
	int c;
	while ((c = getch()) != EOF)
	{
		if (c == '#')	/* directive */
		{
			if (definedirective() == EOF)
				return;
		}
		else if (c == '/')
		{
			if (printcomment() == EOF)
				return;
		}
		else if (c == '\"')
		{
			if (printquote() == EOF)
				return;
		}
		else if (isspace(c))
			putchar(c);
		else
		{
			ungetch(c);
			if (parseword() == EOF)
				return;
		}
	}
}

int str_cmp(char *, char *);
int readword(char *);
int isidentifier(char *);
int readprintline();
struct nlist *install(char *, char *);

int definedirective()
{
	char word[MAXWORD];
	int c1,c2,c3;

	/* read define*/
	c1 = readword(word);
	if (c1 == EOF)
	{
		printf("#%s", word);
		return EOF;
	}
	else if (str_cmp(word, "define") == 0)
	{
		/* #define */
		c2 = readword(word);	/* search word */
		if (c2 == EOF)
		{
			printf("#define%c%s", c1, word);
			return c2;
		}
		else if (isidentifier(word))
		{
			char def[MAXWORD];
			c3 = readword(def);
			if (strlen(def) == 0)
				printf("#define%c%s%c%s", c1, word, c2, def);
			else
			{
				install(word, def);
			}
			if (c3 != EOF)
				putchar(c3);
			return c3;
		}
		else
		{
			printf("#define%c%s", c1, word);
			return c2;
		}
	}
	else
	{
		printf("#%s%c", word, c1);
		return readprintline();
	}
	
}

/* reads in ret and returns a space. */
int readword(char *ret)
{
	int c;
	while (isspace(c = getch()))
		putchar(c);
	for (; isalnum(c) || c == '_'; ret++)
	{
		*ret = c;
		c = getch();
	}
	*ret = '\0';
	return c;
}


int parseword()
{
	struct nlist *lookup(char *);
	char ret[MAXWORD];
	struct nlist *lu;
	int c = readword(ret);
	if (strlen(ret) == 0)
		return c;
	lu = lookup(ret);
	if (lu == NULL)
		printf("%s", ret);
	else
		printf("%s", lu->defn);
	if (c == EOF)
		return EOF;
	//ungetch(c);
	putchar(c);
	return c;
}

int readprintline()
{
	int c;
	while ((c = getch()) != EOF && c != '\n')
		putchar(c);
	return c;
}

int printcomment()
{
	int c1;
	putchar('/');
	if ((c1 = getch()) != '*')
	{
		ungetch(c1);
		return c1;
	}
	putchar('*');
	while ((c1 = getch()) != EOF)
	{
		putchar(c1);
		if (c1 == '*')
		{
			int c2;
			if ((c2 = getch()) == '/')
			{
				putchar(c2);
				return c2;
			}
			if (c2 == EOF)
				return EOF;
			putchar(c2);
		}
		/*else
			putchar(c1);*/
	}
	return EOF;
}

int printquote()
{
	int c;
	putchar('\"');
	while ((c = getch()) != EOF)
	{
		putchar(c);
		if (c == '\"')
		{
			return c;
		}
		else if (c == '\\')
		{
			int c2 = getch();
			if (c2 == EOF)
				return c2;
			putchar(c2);
		}
	}
	return EOF;
}

int isidentifier(char *word)
{
	if (strlen(word) == 0)
		return 0;
	if (!isalpha(*word) && *word != '_')
		return 0;
	while (*word)
	{
		if (!isalnum(*word) && *word != '_')
			return 0;
		word++;
	}
	return 1;
}


/*------------------------------------------------------------------*/

int isbeginvalid(int c)
{
	return isalpha(c) || c == '_';
}

int isvalid(int c)
{
	return isalnum(c) || c == '_';
}

/*-----------------------hash table functions-----------------------*/

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
/*-----------------------string functions---------------------------*/

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

/*---------------------getch-ungetch functions----------------------*/
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;
int getch()
{
	return bufp > 0 ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}