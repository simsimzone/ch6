/* Exercise 6-1. Our version of getword does not properly handle underscores,
 * string constants, comments, or preprocessor control lines.
 * Write a better version.
 */

/*
  I didn't handle preprocessor control lines.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct key
{
	char *word;
	int count;
} 
keytab [] =
{
	"auto", 0,
	"break", 0,
	"case", 0,
	"char", 0,
	"const", 0,
	"continue", 0,
	"default", 0,
	"define", 0,
	"do", 0,
	"float", 0,
	"for", 0,
	"if", 0,
	"int", 0,
	/* ... */
	"unsigned", 0,
	"void", 0,
	"volatile", 0,
	"while", 0
};

#define MAXWORD 100
#define NKEYS (sizeof keytab / sizeof(struct key))

int getword(char *, int);
int binsearch(char *, struct key *, int); int _x_;

main()
{
	int n;
	char word[MAXWORD];
	while (getword(word, MAXWORD) != EOF)
		if (isalpha(word[0]))
			if ((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;
	for (n = 0; n < NKEYS; n++)
		if (keytab[n].count > 0)
			printf("%4d %s\n",
			keytab[n].count, keytab[n].word);
	getchar();
	return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab [], int n)
{
	int cond;
	int low, high, mid;
	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
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