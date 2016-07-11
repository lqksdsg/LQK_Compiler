#include "lex.h"
using namespace std;

/* 本动态语言采用最简单的控制流语句，包括选择语句if..else..和循环语句while..break..continue
 * 数据类型包括浮点型
 * bool类型
 * 字符串类型
 * 函数类型def
 */
char *cp;
static char* get_fd_ptr(const char *path)           //read file
{
    FILE *fd = fopen(path,"rb");
    fseek(fd,0,SEEK_END);
    size_t len = ftell(fd);
    char *file_buf = new char[len+1];
    rewind(fd);
    fread(file_buf,1,len,fd);
    file_buf[len] = '\0';
    fclose(fd);
    cout << file_buf <<endl;
    return file_buf;
}

const char* line = 0;
int lineno=1;
Coordinate* place = new Coordinate();       //位置

Token* getToken()
{
    CodeFile *codeFile = new CodeFile();
    for(;;)
    {
        if(*cp==' ')
            cp++;
        place->x = line==0?0:(char *)cp-line;
        place->y = lineno;

        switch(*cp++)
        {
        case '\0':
            return new Token(END,"\\0");
        case '{':
            return (new Token(LC,"{"));
        case ' ': case '\t' :case '\r':
            continue;
        case '}':
            return (new Token(RC,"}"));
        case '>':
            return (new Token(BT,">"));
        case '<':
            return (new Token(LT,"<"));
        case '(':
            return (new Token(LEBRAKET,"("));
        case ')':
            return (new Token(RIBRAKET,")"));
        case '=':
            if(*cp=='=')
            {
                cp++;
                return (new Token(EQ,"=="));
            }
            else
                return (new Token(ASSIGN,"="));
        case '*':
            return (new Token(MUL,"*"));
        case '+':
            return (new Token(ADD,"+"));
        case '/':
            return (new Token(DIV,"/"));
        case '%':
            return (new Token(MOD,"%"));
        case ';':
            return (new Token(SEMICOLON,";"));
        case '#':
            while(*cp!='\n'||*cp!='\r')
                cp++;
            cp++;
            break;
        case '&':
            if(*cp=='&'){
                cp++;
                return (new Token(AND,"&&"));
            }
            else{
                cout << "token error at "<< place->y <<" line "<<place->x << " column";
                exit(-1);
            }
        case '|':
            if(*cp=='|'){
                cp++;
                return (new Token(OR,"||"));
            }
            else{
                cout << "token error at "<< place->y <<" line "<<place->x << " column";
                exit(-1);
            }
        case '!':
            return (new Token(NOT,"|"));
        case '\n':
            line = cp;
            lineno++;
            continue;
        case 'i':
            if(*cp=='f'){
                cp++;
                return (new Token(IF,"if"));
            }
            else
                goto id;
        case 'w':
            if(*cp=='h'&&*(cp+1)=='i'&&*(cp+2)=='l'&&*(cp+3)=='e'){
                cp+=4;
                return (new Token(WHILE,"while"));
            }
            else
                goto id;
        case 'b':
            if(*cp=='r'&&*(cp+1)=='e'&&*(cp+2)=='a'&&*(cp+3)=='k'){
                cp+=4;
                return (new Token(BREAK,"break"));
            }
            else
                goto id;
        case 'c':
            if(*cp=='o'&&*(cp+1)=='n'&&*(cp+2)=='t'&&*(cp+3)=='i'&&*(cp+4)=='n'&&*(cp+5)=='u'&&*(cp+6)=='e'){
                cp+=7;
                return (new Token(CONTINUE,"continue"));
            }
            else
                goto id;
        case 'r':
            if(*cp=='e'&&*(cp+1)=='t'&&*(cp+2)=='u'&&*(cp+3)=='r'&&*(cp+4)=='n'){
                cp+=5;
                return (new Token(RETURN,"return"));
            }
            else
                goto id;
        case 't':
            if(*cp=='r'&&*(cp+1)=='u'&&*(cp+2)=='e'){
                cp+=3;
                return (new Token(TRUE,"true"));
            }
            else
                goto id;
        case 'f':
            if(*cp=='a'&&*(cp+1)=='l'&&*(cp+2)=='s'&&*(cp+3)=='e'){
                cp+=4;
                return (new Token(FALSE,"false"));
            }
            else
                goto id;
        case 'e':
            if(*cp=='l'&&*(cp+1)=='s'&&*(cp+2)=='e'){
                cp+=3;
                return (new Token(FALSE,"else"));
            }
            else
                goto id;
        case 'd':
            if(*cp=='e'&&*(cp+1)=='f'){
                cp+=2;
                return (new Token(DEF,"def"));
            }
            else
                goto id;
        case '"':
        {
            const char *rcp = cp;
            while(*cp!='"'){
                if(*cp=='\n')
                {
                    cout << "string should be in one line at "<<place->y <<" line " << place->x << " column"<< endl;
                    exit(-1);
                }
                cp++;
            }
            cp++;
            char *str = new char[cp-rcp];
            memcpy(str,rcp,cp-rcp-1);
            str[cp-rcp-1]='\0';
            return new Token(STRING,str);
        }
        case '0': case '1':case '2':case '3':case '4':case '5':case '6':case '7': case '8': case '9':
        {
            const char *rcp = cp;
            if(*(cp-1)=='0')
            {
                if(isdigit(*cp)){
                    cout << "wrong number at " << place->y << " line " << endl;
                    exit(-1);
                }
                else if(*cp=='.'){
                    cp++;
                    if(isdigit(*cp)){
                        do
                            cp++;
                        while(isdigit(*cp));
                    }
                    else{
                        cout << "wrong number at "<< place->y <<" line " <<endl;
                        exit(-1);
                    }
                }
                char *number = new char[cp-rcp+2];
                memcpy(number,rcp-1,cp-rcp+1);
                number[cp-rcp+1]='\0';
                return new Token(NUM,number);
            }
            while(isdigit(*cp))
            {
                cp++;
            }
            if(*cp=='.'){
                cp++;
                if(isdigit(*cp)){
                    do
                        cp++;
                    while(isdigit(*cp));
                }
                else{
                    cout << "wrong number at "<< place->y <<" line " <<endl;
                    exit(-1);
                }

            }
            char *number = new char[cp-rcp+2];
            memcpy(number,rcp-1,cp-rcp+1);
            number[cp-rcp+1]='\0';
            return new Token(NUM,number);               //extract number
        }
id:
        case 'a': case 'g': case 'h': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 's': case 'u':
        case 'v': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':case 'Y': case 'Z':{
            const char *rcp = cp;
            while(1){
                if(*cp<='z'&&*cp>='a'||*cp<='Z'&&*cp>='A'){
                    cp++;
                }
                else
                {
                    char *var = new char[cp-rcp+2];
                    memcpy(var,rcp-1,cp-rcp+1);
                    var[cp-rcp+1]='\0';
                    return new Token(IDENTIFIER,var);
                }
            }
        }

        default:
            cout << "got wrong token " << "at "<<  place->y << " line " << place->x << " column " <<endl;
            exit(-1);
        }
    }
}


CodeFile* tokenScanner(const char *path)
{
    cp = get_fd_ptr(path);
    CodeFile* codeFile = new CodeFile();
    do
    {
        codeFile->lines.push_back(getToken());
    }while(*cp!='\0');
    return codeFile;
}

