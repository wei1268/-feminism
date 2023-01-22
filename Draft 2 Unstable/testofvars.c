#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "String.c"
int a;String c;int main(){printf("%s", append(string_like("a"),(String)(String)string_like("b"))->str);printf("\n");a=9;printf("%s", append(string_like(""),its((int)(String)a))->str);printf("\n");c=string_like("z");c=append(string_like("z"),(String)(String)string_like("a"));printf("%s", c->str);printf("\n");}