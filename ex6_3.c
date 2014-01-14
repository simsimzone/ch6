/*
 * Exercise 6-3. Write a cross-referencer that prints a list of all words
 * in a document, and for each word, a list of the line numbers 
 * on which it occurs. Remove noise words like ``the,'' ``and,'' and so on.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD				100
#define MAXLINES			100

struct tnode {
	/* the tree node: */
	char *word;				/* points to the text */
	int count;				/* number of occurrences */
	int lines [MAXLINES];
	struct tnode *left;		/* left child */
	struct tnode *right;	/* right child */
};

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);
int str_match_any(char *, char **);

int ln = 1;		/* current line number */

main()
{
	struct tnode *root;
	char word[MAXWORD];
	char *words [] = { "the", "and" , 0};
	root = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]) && !str_match_any(word, words))
			root = addtree(root, word);
	treeprint(root);
	getchar();
	return 0;
}

struct tnode *talloc(void);
char *str_dup(char *);

void str_cpy(char *, char *);
int str_cmp(char *, char *);

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;
	if (p == NULL) {
		/* a new word has arrived */
		p = talloc(); /* make a new node */
		p->word = str_dup(w);
		p->count = 1;
		p->lines[0] = ln;
		p->left = p->right = NULL;
	}
	else if ((cond = str_cmp(w, p->word)) == 0)
		p->lines[p->count++] = ln; /* repeated word */
	else if (cond < 0) /* less than into left subtree */
		p->left = addtree(p->left, w);
	else /* greater than into right subtree */
		p->right = addtree(p->right, w);
	return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p)
{
	int i;
	if (p != NULL) {
		treeprint(p->left);
		printf("\n%15s  ", p->word);
		printf("%d", p->lines[0]);
		for (i = 1; i < p->count; i++)
			printf(", %d", p->lines[i]);
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

int str_match_any(char *s, char *t[])
{
	//char *tt = *t;
	while (*t)
		if (strcmp(s, *t++) == 0)
			return 1;
	return 0;
}


/* getword: get next word or character from input */
int getword(char *word, int lim)
{
	int c, getch(void);
	void ungetch(int);
	char *w = word;
	while (isspace(c = getch()) && c != '\n')
		;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c)) 
	{
		if (c == '\n')
			ln++;
		*w = '\0';
		return c;
	}
	for (; --lim > 0; w++)
		if (!isalnum(*w = getch())) {
			ungetch(*w);
			break;
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