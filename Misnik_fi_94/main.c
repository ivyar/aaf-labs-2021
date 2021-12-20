#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include <stdio.h>

int main(int argc, char * argv[]){
dlist * tree = dlist_init();//дерево для стека команды
dbase * tables=dbase_init("test");//создаем базу данных
if (argc==2) {//если введен аргумент
FILE * datafile;
datafile = fopen(argv[1],"r");
if (!datafile) {puts("cant open file"); return -2;}//и это было имя сущевствующего файла
char command[0xff];
do{//читем команды из него
if(fgets(command,0xff,datafile)){
	//printf("------------Command acheive - %s !!!",command);
	parse_string(tree,command);//парсим команду в последовательность двусвязного списка
	parse_comm(tree,tables);}//разбираем последовательность как команду языка SQL
//puts("i home!!");
fflush(datafile);
dlist_clear(tree); //чистим стек команды
}while(!feof(datafile));//повторяем со следуюзей стркоой файла
fclose(datafile);//закрываем файл
} //после чего и/или если файла не было
puts("Input commands...");
char cmnd[80];
do {//принимаем команды с стандартного ввода/вывода
dlist_clear(tree); //сначла чистим стек команд
printf("\n>>");//печатаем приглашение
//fflush(stdin);
fgets(cmnd,80,stdin);//считываем из стандартного фала ввода/вывода
//fflush(stdin);
//printf("cmnd-%s\n",cmnd);//печатем что получили для контроля
parse_string(tree,cmnd);//парсим команду
//dlist_list(tree);//вывод стека команды для контроля
parse_comm(tree,tables);//разбор команды
//printf("cmnd=%s",cmnd);
//puts("i home!!");
//printf("%s\n",cmnd);
} while (cmnd[0]!='q');//повторяем пока не получим quit
return 0;//всё
}
