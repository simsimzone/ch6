/*
 * Exercise 6-2. Write a program that reads a C program and prints in
 * alphabetical order each group of variable names that are identical
 * in the first 6 characters, but different somewhere thereafter.
 * Don't count words within strings and comments.
 * Make 6 a parameter that can be set from the command line.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define COMP_LEN 6

/* the tree node: */
struct tnode {
	char *word;				/* points to the text */
	int count;				/* number of occurrences */
	int group;				/* group id */
	struct tnode *left;		/* left child */
	struct tnode *right;	/* right child */
};

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

int lg = 0;			/* last group */
int currgroup = 0;	/* current group */
int n;				/* comparison length */

main(int argc, char *argv [])
{
	struct tnode *root;
	char word[MAXWORD];
	if (argc > 1)
	{
		n = atoi(*++argv);
		n = n <= 0 ? COMP_LEN : n;
	}
	else
		n = COMP_LEN;
	root = NULL;
	while (getword(word, MAXWORD) != EOF)
	{
		if (isalpha(word[0]) && (int) strlen(word) >= n)
		{
			currgroup = 0;
			root = addtree(root, word);
		}
	}
	lg = 0;
	treeprint(root);
	getchar();
	return 0;
}

struct tnode *talloc(void);
char *str_dup(char *);

void str_cpy(char *, char *);
int str_cmp(char *, char *);
int str_n_cmp(char *, char *, int);

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;
	if (p == NULL) {
		/* a new word has arrived */
		p = talloc(); /* make a new node */
		p->word = str_dup(w);
		p->count = 1;
		p->group = currgroup;
		p->left = p->right = NULL;
		return p;
	}
	if (str_n_cmp(w, p->word, n) == 0)	/* group matching */
	{
		if (!p->group)	/* if it doesn't has group set a new group for it */
			p->group = ++lg;
		currgroup = p->group;
	}
	if ((cond = str_cmp(w, p->word)) == 0)
		p->count++; /* repeated word */
	else if (cond < 0) /* less than into left subtree */
		p->left = addtree(p->left, w);
	else /* greater than into right subtree */
		p->right = addtree(p->right, w);
	return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p)
{
	if (p != NULL) {
		treeprint(p->left);
		if (p->group)
		{
			if (lg != p->group)
				printf("------------------(%d)------------------\n", p->group);
			printf("\t%4d %s\n", p->count, p->word);
			lg = p->group;
		}
		treeprint(p->right);
	}
}

#include <stdlib.h>

/* talloc: make a tnode */
struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}

/* make a duplicate of s */
char *str_dup(char *s)
{
	char *p;
	p = (char *) malloc(strlen(s) + 1); /* +1 for '\0' */
	if (p != NULL)
		str_cpy(p, s);
	return p;
}

/* strcpy: copy t to s; pointer version 3 */
void str_cpy(char *s, char *t)
{
	while (*s++ = *t++)
		;
}

/* strcmp: returns <0 if s<t, 0 if s==t, >0 if s>t */
int str_cmp(char *s, char *t)
{
	for (; *s == *t; s++, t++)
		if (*s == '\0')
			return 0;
	return *s - *t;
}

/* strncmp: compares most n characters of t to s.*/
int str_n_cmp(char *s, char *t, int n)
{
	while (n-- && *s++ == *t && s[-1] && *t++);
	return n > 0 ? *--s - *t : 0;
}

int isbeginvalid(int c)
{
	return isalpha(c) || c == '_';
}

int isvalid(int c)
{
	return isalnum(c) || c == '_';
}

int getch(void);
void ungetch(int);

/* consumes spaces and returns the next character. */
int consume_space()
{
	int c;
	while (isspace(c = getch()));
	return c;
}

/* consumes the string constant. */
int consume_double_quote()
{
	int c;
	while ((c = getch()) != EOF)
	{
		if (c == '\\') /* go through \" */
			getch();
		else if (c == '\"')
			break;
	}
	return c == EOF ? c : getch();
}

int consume_single_quote()
{
	int c;
	while ((c = getch()) != EOF)
	{
		if (c == '\\') /* go through \" */
			getch();
		else if (c == '\'')
			break;
	}
	return c == EOF ? c : getch();
}

/* consumes the comment. */
int consume_comment()
{
	int c;
	while ((c = getch()) != EOF)
	{
		if (c == '*')
		{
			c = getch();
			if (c == '/')
				break;
			else
				ungetch(c);
		}
	}
	return c == EOF ? c : getch();
}

/* getword: get next word or character from input */
int getword(char *word, int lim)
{
	int c;
	char *w = word;
	while (isspace(c = getch()))
		;
	/*------------------------------------*/
	while (c == '\"' || c == '\'' || c == '/')
	{
		if (c == '\"')
		{
			c = consume_double_quote(); /* either \" or EOF */
		}
		else if (c == '\'')
		{
			c = consume_single_quote(); /* either \' or EOF */
		}
		else if (c == '/')
		{
			if ((c = getch()) == '*')
				c = consume_comment();
		}
		if (isspace(c))
			c = consume_space();
	}
	/*------------------------------------*/
	if (c != EOF)
		*w++ = c;
	if (!isbeginvalid(c)) {
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++)
	{
		if (!isvalid(*w = getch())) /* isalnum has been relaced by isvalid */
		{
			ungetch(*w);
			break;
		}
	}
	*w = '\0';
	return word[0];
}

/*------------------------------------------------------------------*/
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