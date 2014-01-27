/*
 * Exercise 6-4. Write a program that prints the distinct words in its input 
 * sorted into decreasing order of frequency of occurrence. 
 * Precede each word by its count.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100

/* the tree node: */
struct tnode 
{
	char *word;				/* points to the text */
	int count;				/* number of occurrences */
	struct tnode *left;			/* left child */
	struct tnode *right;			/* right child */
};


struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

struct tnode* dup_by_count(struct tnode *, struct tnode *);

/* word frequency count */
main()
{
	struct tnode *root, *countree = NULL;
	char word[MAXWORD];
	root = NULL;
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			root = addtree(root, word);

	countree = dup_by_count(countree, root);

	treeprint(countree);
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
		p->left = p->right = NULL;
	}
	else if ((cond = str_cmp(w, p->word)) == 0)
		p->count++; /* repeated word */
	else if (cond < 0) /* less than into left subtree */
		p->left = addtree(p->left, w);
	else /* greater than into right subtree */
		p->right = addtree(p->right, w);
	return p;
}


struct tnode *addcountnode(struct tnode *newtree, struct tnode *oldtree)
{
	if (newtree == NULL)
	{
		newtree = talloc();
		newtree->word = str_dup(oldtree->word);
		newtree->count = oldtree->count;
		newtree->left = newtree->right = NULL;
	}
	else if (newtree->count < oldtree->count)
		newtree->left = addcountnode(newtree->left, oldtree);
	else if (newtree->count >= oldtree->count)
		newtree->right = addcountnode(newtree->right, oldtree);
	return newtree;
}

struct tnode* dup_by_count(struct tnode *newtree, struct tnode *oldtree)
{
	if (oldtree)
	{
		newtree = addcountnode(newtree, oldtree);
		newtree = dup_by_count(newtree, oldtree->left);
		newtree = dup_by_count(newtree, oldtree->right);
	}
	return newtree;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p)
{
	if (p != NULL) {
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
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

int getch(void);
void ungetch(int);

/* getword: get next word or character from input */
int getword(char *word, int lim)
{
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
