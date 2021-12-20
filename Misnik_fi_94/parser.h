#include "dlist.h"
#include "dbase.h"

void parse_string(dlist * list,char * str);// parsing string whit expression

int parse_comm(dlist * list,dbase * base);

bool isSpacer(char * ch);

bool isDivider(char * ch);

bool isDigit(char * ch);

bool isCharacter(char * ch);

bool isSymbol(char * ch);
