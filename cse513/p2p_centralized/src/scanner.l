%{
#include <string.h>
#include "scanner.h"

extern char lexeme[];
%}

%%
[ \t\n]  ;

[a-zA-Z]* {
   strcpy( lexeme, yytext );
   return TEXT;
}

[0-9]* {
   strcpy( lexeme, yytext );
   return NUMBER;
}

[<]TITLE[>]	{
   strcpy( lexeme, yytext );
   return LTITLE;
}

[<][/]TITLE[>]	{
   strcpy( lexeme, yytext );
   return RTITLE;
}

[<]ARTIST[>] {
   strcpy( lexeme, yytext );
   return LARTIST;
}

[<][/]ARTIST[>] {
   strcpy( lexeme, yytext );
   return RARTIST;
}

[<]COUNTRY[>] {
   strcpy( lexeme, yytext );
   return LCOUNTRY;
}

[<][/]COUNTRY[>] {
   strcpy( lexeme, yytext );
   return RCOUNTRY;
}

[<]COMPANY[>] {
   strcpy( lexeme, yytext );
   return LCOMPANY;
}

[<][/]COMPANY[>] {
   strcpy( lexeme, yytext );
   return RCOMPANY;
}
[<]YEAR[>] {
   strcpy( lexeme, yytext );
   return LYEAR;
}

[<][/]YEAR[>] {
   strcpy( lexeme, yytext );
   return RYEAR;
}

%%
int yywrap() {
   return 1;
}
