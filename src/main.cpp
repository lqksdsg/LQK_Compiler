#include "lex.h"
int main()
{
    const char *path = "F:\\123.lqk";
    auto k = tokenScanner(path);
    for(auto i : k->lines){
        cout << i->type <<" -> " << i->value << endl;
    }
    cout << endl;
    return 0;
}
