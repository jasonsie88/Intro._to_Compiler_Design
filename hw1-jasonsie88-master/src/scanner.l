%{
#include <stdint.h>

#define LIST                concatenateString(yytext)
#define TOKEN(t)            { LIST; if (opt_tok) printf("<%s>\n", #t); }
#define TOKEN_CHAR(t)       { LIST; if (opt_tok) printf("<%c>\n", (t)); }
#define TOKEN_STRING(t, s)  { LIST; if (opt_tok) printf("<%s: %s>\n", #t, (s)); }
#define MAX_LINE_LENG       512

uint32_t line_num = 1;
static char buffer[MAX_LINE_LENG];
static char string_buffer_temp[MAX_LINE_LENG];
static char string_buffer[MAX_LINE_LENG];
static uint32_t opt_src = 1;
static uint32_t opt_tok = 1;
static char *buffer_ptr = buffer;

static void concatenateString(const char *yytext_ptr);

%}

%Start COMMENT COMMENT2

%%
<INITIAL>"//&S+".* { LIST; opt_src=1; }
<INITIAL>"//&S-".* { LIST; opt_src=0;}
<INITIAL>"//&T+".* { LIST; opt_tok=1;}
<INITIAL>"//&T-".* { LIST; opt_tok=0;}
<COMMENT>.  {LIST; } 
<INITIAL>"//".* { LIST; }
<INITIAL>"/*" { LIST; BEGIN COMMENT;}
<COMMENT>"*/" { LIST; BEGIN INITIAL; }




<INITIAL>"*" { TOKEN_CHAR('*'); }
<INITIAL>"/" { TOKEN_CHAR('/'); }
<INITIAL>"mod" { TOKEN(mod); }
<INITIAL>":=" { TOKEN(:=); }
<INITIAL>"<" { TOKEN_CHAR('<'); }
<INITIAL>"<=" { TOKEN(<=); }
<INITIAL>"<>" { TOKEN(<>); }
<INITIAL>">=" { TOKEN(>=); }
<INITIAL>">" { TOKEN_CHAR('>'); }
<INITIAL>"=" { TOKEN_CHAR('='); }
<INITIAL>"," { TOKEN_CHAR(','); }
<INITIAL>";" { TOKEN_CHAR(';'); }
<INITIAL>":" { TOKEN_CHAR(':'); }
<INITIAL>"[" { TOKEN_CHAR('['); }
<INITIAL>"]" { TOKEN_CHAR(']'); }
<INITIAL>")" { TOKEN_CHAR(')'); }
<INITIAL>"(" { TOKEN_CHAR('('); }
<INITIAL>"and" { TOKEN(and); }
<INITIAL>"or" { TOKEN(or); }
<INITIAL>"not" { TOKEN(not); }
<INITIAL>"var" { TOKEN(KWvar); }
<INITIAL>"def" { TOKEN(KWdef); }
<INITIAL>"array" { TOKEN(KWarray); }
<INITIAL>"of" { TOKEN(KWof); }
<INITIAL>"boolean" { TOKEN(KWboolean); }
<INITIAL>"integer" { TOKEN(KWinteger); }
<INITIAL>"real" { TOKEN(KWreal); }
<INITIAL>"string" { TOKEN(KWstring); }
<INITIAL>"true" { TOKEN(KWtrue); }
<INITIAL>"false" { TOKEN(KWfalse); }
<INITIAL>"while" { TOKEN(KWwhile); }
<INITIAL>"do" { TOKEN(KWdo); }
<INITIAL>"if" { TOKEN(KWif); }
<INITIAL>"then" { TOKEN(KWthen); }
<INITIAL>"else" { TOKEN(KWelse); }
<INITIAL>"for" { TOKEN(KWfor); }
<INITIAL>"to" { TOKEN(KWto); }
<INITIAL>"begin" { TOKEN(KWbegin); }
<INITIAL>"end" { TOKEN(KWend); }
<INITIAL>"print" { TOKEN(KWprint); }
<INITIAL>"read" { TOKEN(KWread); }
<INITIAL>"return" { TOKEN(KWreturn); }
<INITIAL>[a-zA-Z][0-9a-zA-Z]* { TOKEN_STRING(id,yytext); }
<INITIAL>((((0)|([1-9][0-9]*))|((((0)|([1-9][0-9]*))\.[0-9]*[1-9])|((0)|([1-9][0-9]*)\.0)))[Ee][+|-]?((0)|([1-9][0-9]*))) { TOKEN_STRING(scientific,yytext); }
<INITIAL>((((0)|([1-9][0-9]*))\.[0-9]*[1-9])|(((0)|([1-9][0-9]*))\.0)) { TOKEN_STRING(float,yytext); }

<INITIAL>((0)|([1-9][0-9]*)) { TOKEN_STRING(integer,yytext); }
<INITIAL>0[0-7][0-7]* { TOKEN_STRING(oct_integer,yytext); }


<INITIAL>"+" { TOKEN_CHAR('+'); }
<INITIAL>"-" { TOKEN_CHAR('-'); }

<INITIAL>\"[^\n\"]*((\"\")([^\n\"]*))*\" {
            int yytext_len = strlen(yytext)-2;
            strncat(string_buffer_temp,yytext+1,yytext_len);
            int len = strlen(string_buffer_temp);
            for(int i=0;i<len;){
                strncat(string_buffer,string_buffer_temp+i,1);
                if(*(string_buffer_temp+i)=='\"'){
                    i+=2;
                }else{
                    i++;
                }
            }                                         
            TOKEN_STRING(string,string_buffer);
            *string_buffer='\0';
            *string_buffer_temp='\0';
        }






<INITIAL>\n |
<COMMENT>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}



[ \t]+ {LIST;}
    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%
static void concatenateString(const char *yytext_ptr) {
    while (*yytext_ptr) {
        *buffer_ptr = *yytext_ptr;
        ++buffer_ptr;
        ++yytext_ptr;
    }
    *buffer_ptr = '\0';
}


int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./scanner [filename]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    return 0;
}
