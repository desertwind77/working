#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# ifndef YYLMAX 
# define YYLMAX BUFSIZ
# endif 
#ifndef __cplusplus
# define output(c) (void)putc(c,yyout)
#else
# define lex_output(c) (void)putc(c,yyout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int yyback(int *, int);
	int yyinput(void);
	int yylook(void);
	void yyoutput(int);
	int yyracc(int);
	int yyreject(void);
	void yyunput(int);
	int yylex(void);
#ifdef YYLEX_E
	void yywoutput(wchar_t);
	wchar_t yywinput(void);
#endif
#ifndef yyless
	int yyless(int);
#endif
#ifndef yywrap
	int yywrap(void);
#endif
#ifdef LEXDEBUG
	void allprint(char);
	void sprint(char *);
#endif
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void exit(int);
#ifdef __cplusplus
}
#endif

#endif
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
#ifndef __cplusplus
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#else
# define lex_input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#endif
#define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng;
#define YYISARRAY
char yytext[YYLMAX];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
#include <string.h>
#include "scanner.h"

extern char lexeme[];
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
#ifdef __cplusplus
/* to avoid CC and lint complaining yyfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto yyfussy;
#endif
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:

# line 9 "./src/scanner.l"
	;
break;
case 2:

# line 10 "./src/scanner.l"
		{
					strcpy(lexeme, yytext);
					return yytext[0];
				}
break;
case 3:

# line 14 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return TEXT; 
				}
break;
case 4:

# line 18 "./src/scanner.l"
	{
					strcpy(lexeme, yytext);
					return NUMBER; 
				}
break;
case 5:

# line 22 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return LTITLE; 
				}
break;
case 6:

# line 26 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return RTITLE; 
				}
break;
case 7:

# line 30 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return LARTIST; 
				}
break;
case 8:

# line 34 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return RARTIST; 
				}
break;
case 9:

# line 38 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return LCOUNTRY; 
				}
break;
case 10:

# line 42 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return RCOUNTRY; 
				}
break;
case 11:

# line 46 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return LCOMPANY; 
				}
break;
case 12:

# line 50 "./src/scanner.l"
{
					strcpy(lexeme, yytext);
					return RCOMPANY; 
				}
break;
case 13:

# line 54 "./src/scanner.l"
	{
					strcpy(lexeme, yytext);
					return LYEAR; 
				}
break;
case 14:

# line 58 "./src/scanner.l"
	{
					strcpy(lexeme, yytext);
					return RYEAR; 
				}
break;
case -1:
break;
default:
(void)fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

# line 64 "./src/scanner.l"
yywrap() {
	return 1;
}
int yyvstop[] = {
0,

3,
4,
0, 

3,
4,
0, 

1,
0, 

1,
2,
0, 

4,
0, 

3,
0, 

13,
0, 

14,
0, 

5,
0, 

6,
0, 

7,
0, 

8,
0, 

11,
0, 

9,
0, 

12,
0, 

10,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,3,	1,3,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,4,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,5,	1,5,	1,5,	
1,5,	1,5,	1,5,	1,5,	
1,5,	1,5,	1,5,	0,0,	
0,0,	1,6,	0,0,	39,49,	
44,54,	48,58,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
9,17,	10,18,	11,19,	12,20,	
13,21,	14,22,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
1,7,	1,7,	1,7,	1,7,	
5,5,	5,5,	5,5,	5,5,	
5,5,	5,5,	5,5,	5,5,	
5,5,	5,5,	6,8,	15,23,	
8,13,	16,24,	8,14,	17,25,	
18,26,	19,28,	20,29,	21,30,	
23,33,	22,31,	24,34,	25,35,	
18,27,	26,36,	27,37,	28,38,	
6,9,	22,32,	6,10,	8,15,	
29,39,	30,40,	31,41,	32,42,	
8,16,	33,43,	34,44,	35,45,	
36,46,	37,47,	38,48,	40,50,	
41,51,	42,52,	43,53,	6,11,	
45,55,	46,56,	47,57,	50,59,	
6,12,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	51,60,	
52,61,	53,62,	55,63,	56,64,	
57,65,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	7,7,	
7,7,	7,7,	7,7,	59,66,	
60,67,	61,68,	64,69,	65,70,	
67,71,	68,72,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+1,	0,		yyvstop+1,
yycrank+0,	yysvec+1,	yyvstop+4,
yycrank+0,	0,		yyvstop+7,
yycrank+0,	0,		yyvstop+9,
yycrank+76,	0,		yyvstop+12,
yycrank+87,	0,		0,	
yycrank+112,	0,		yyvstop+14,
yycrank+71,	0,		0,	
yycrank+10,	0,		0,	
yycrank+14,	0,		0,	
yycrank+21,	0,		0,	
yycrank+26,	0,		0,	
yycrank+14,	0,		0,	
yycrank+18,	0,		0,	
yycrank+62,	0,		0,	
yycrank+68,	0,		0,	
yycrank+55,	0,		0,	
yycrank+63,	0,		0,	
yycrank+57,	0,		0,	
yycrank+77,	0,		0,	
yycrank+59,	0,		0,	
yycrank+68,	0,		0,	
yycrank+60,	0,		0,	
yycrank+81,	0,		0,	
yycrank+74,	0,		0,	
yycrank+69,	0,		0,	
yycrank+72,	0,		0,	
yycrank+75,	0,		0,	
yycrank+74,	0,		0,	
yycrank+84,	0,		0,	
yycrank+78,	0,		0,	
yycrank+81,	0,		0,	
yycrank+85,	0,		0,	
yycrank+80,	0,		0,	
yycrank+80,	0,		0,	
yycrank+99,	0,		0,	
yycrank+81,	0,		0,	
yycrank+97,	0,		0,	
yycrank+1,	0,		0,	
yycrank+84,	0,		0,	
yycrank+103,	0,		0,	
yycrank+85,	0,		0,	
yycrank+101,	0,		0,	
yycrank+2,	0,		0,	
yycrank+88,	0,		0,	
yycrank+95,	0,		0,	
yycrank+92,	0,		0,	
yycrank+3,	0,		0,	
yycrank+0,	0,		yyvstop+16,
yycrank+91,	0,		0,	
yycrank+125,	0,		0,	
yycrank+122,	0,		0,	
yycrank+143,	0,		0,	
yycrank+0,	0,		yyvstop+18,
yycrank+144,	0,		0,	
yycrank+118,	0,		0,	
yycrank+119,	0,		0,	
yycrank+0,	0,		yyvstop+20,
yycrank+173,	0,		0,	
yycrank+147,	0,		0,	
yycrank+148,	0,		0,	
yycrank+0,	0,		yyvstop+22,
yycrank+0,	0,		yyvstop+24,
yycrank+176,	0,		0,	
yycrank+177,	0,		0,	
yycrank+0,	0,		yyvstop+26,
yycrank+178,	0,		0,	
yycrank+179,	0,		0,	
yycrank+0,	0,		yyvstop+28,
yycrank+0,	0,		yyvstop+30,
yycrank+0,	0,		yyvstop+32,
yycrank+0,	0,		yyvstop+34,
0,	0,	0};
struct yywork *yytop = yycrank+241;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,   9,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
 32,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,  47, 
 48,  48,  48,  48,  48,  48,  48,  48, 
 48,  48,   1,   1,  60,   1,  62,   1, 
  1,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,   1,   1,   1,   1,   1, 
  1,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.12	97/12/08 SMI"

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int yylook(void)
#else
yylook()
#endif
{
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
#ifndef __cplusplus
			*yylastch++ = yych = input();
#else
			*yylastch++ = yych = lex_input();
#endif
#ifdef YYISARRAY
			if(yylastch > &yytext[YYLMAX]) {
				fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
#else
			if (yylastch >= &yytext[ yytextsz ]) {
				int	x = yylastch - yytext;

				yytextsz += YYTEXTSZINC;
				if (yytext == yy_tbuf) {
				    yytext = (char *) malloc(yytextsz);
				    memcpy(yytext, yy_tbuf, sizeof (yy_tbuf));
				}
				else
				    yytext = (char *) realloc(yytext, yytextsz);
				if (!yytext) {
				    fprintf(yyout,
					"Cannot realloc yytext\n");
				    exit(1);
				}
				yylastch = yytext + x;
			}
#endif
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (uintptr_t)yyt > (uintptr_t)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((uintptr_t)yyt < (uintptr_t)yycrank) {	/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
#ifndef __cplusplus
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
#else
		yyprevious = yytext[0] = lex_input();
		if (yyprevious>0)
			lex_output(yyprevious);
#endif
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int yyback(int *p, int m)
#else
yyback(p, m)
	int *p;
#endif
{
	if (p==0) return(0);
	while (*p) {
		if (*p++ == m)
			return(1);
	}
	return(0);
}
	/* the following are only used in the lex library */
#if defined(__cplusplus) || defined(__STDC__)
int yyinput(void)
#else
yyinput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyoutput(int c)
#else
yyoutput(c)
  int c; 
#endif
{
#ifndef __cplusplus
	output(c);
#else
	lex_output(c);
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyunput(int c)
#else
yyunput(c)
   int c; 
#endif
{
	unput(c);
	}
