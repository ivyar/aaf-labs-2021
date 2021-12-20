#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
//printf("-stp-s=%s-\n",p->str);
#define  _step if (p->next) {p=p->next;} else {puts("unexpected end");return -1;}
#define  _stback p=p->prev;
#define  _s p->str
//printf("!p->str=%s!",p->str);
#define  _tst 
#define  _isok if (p)  if (p->str)
#define  _isnok else {puts("Unespected NULL");return -1;}
#define  _t if (p) 
#define  C_CREATE if (!strcmp(p->str,"CREATE")) while(1) 
#define  C_INSERT if (!strcmp(p->str,"INSERT")) while(1)  
#define  C_SELECT if (!strcmp(p->str,"SELECT")) while(1) 
#define  C_DELETE if (!strcmp(p->str,"DELETE")) while(1) 
#define  C_DESCRIBE if (!strcmp(p->str,"DESCRIBE")) while(1) 




int parse_comm( dlist * list, dbase * base){
//функция получает ссылку на список dlist с командами  
//и ссылку на базу данных dbase
//-------------------------------------------------------

char * tmst=NULL;
//устанавливаем укзатель в начало списка			        
if (!list->tail) return 0;
 dnode * p = list->tail;
_step
_stback
if (!list->count) return 0;
//-------------------------------------CREATE----------------------	
	C_CREATE //если команда создать создаем бесконечный цикл для удобства
	{ //puts("Create table recognized");  
        
        _step;//смещаемся на следующий элемент команду уже обработали
       
        //printf("Table name %s is specifyed \n",_s);
        
        if (!isDivider(_s)) {}//смотрим имя таблицы
           else {puts("Incorrect table name");return -1;}//если начинается с разделителя то ошибка
              
              dtable * t;
         //какое-то соответствующее имя получено - можно создавать таблицу
         //создаем таблицу без столбцов	 и крепим ее к базе   	
         if (!dbase_find(base,_s))  { t = dtable_create(_s );}
         	else {printf("Table %s is already present\n"); return -1;}
         dbase_add(base,t);
	// дальше у нас может быть либо ; либо скобки с именами и типами столбцов для таблицы
        _step ; 
        
                if (_s[0]==';') {return 1;}//если находим ; то уходим без ошибки
                	if (_s[0]=='(') {//находим открытые скобки
                		//puts(" Open bracket ( found - adding columns");
                			
                		while(1){//заводим цикл по скобкам
	 _step
	    if (_s[0]==')') {break;}
	    if (_s[0]==',') {continue;}
	    if (_s[0]=='"') {  continue;}
	    if (!isSymbol(_s)) {printf("unexpected input%s\n",_s);return -2;}
            if (!strcmp(_s,"INDEXED"))//и проверяем индексируемый ли столбец
            {//если да создаем столбец как индексируемый
	      		_step;//шагаем
	      		dtable_add(t,dcol_create(_s,true));
	                		}else//если нет создаем как неиндексируемый
	                			{//puts("col is not INDEXED");
						dtable_add(t,dcol_create(_s,false));}
                			}
                		}
                		_step
                		if (_s[0]==';') {
                			printf("Table %s succesfully created",t->name);
                			return 1;}
              puts ("Unsuspected end of command!");
              return -1;
	    }// CREATE END---------------------------
//===============================================INSERT=================================	 
	 C_INSERT
	 {//puts("insert found");
	 _step
	 if (!isSymbol(_s)) {printf("unexpected input%s\n",_s);return -2;}
	 if (!strcmp(_s,"INTO")) {_step }
	 dtable * tmptable = dbase_find(base,_s);//
	 if (!tmptable) {printf("There isn`t table %s",_s);return -2;} else 
	 		printf("Table %s found\n",_s);
	 // дальше у нас может быть либо ; либо скобки с именами и типами столбцов для таблицы
         _step ; 
                if (_s[0]==';') {return 2;}//если находим ; то уходим без ошибки
                	if (_s[0]=='(') {//находим открытые скобки
                		//puts(" Open bracket ( found - adding cells");	 
                dcol * tcol = tmptable->tail;
	 int count=0;
	 while(1){//-------------------------------------------
	 _step
	    if (_s[0]==')') {break;}
	    if (_s[0]==',') {continue;}
	    if (_s[0]=='"') { count++; continue;}
	    if (!isSymbol(_s)) {printf("unexpected input%s\n",_s);return -2;}
            if (count>tmptable->count) 
            		{printf("trying to add more cols there in table%li\n",tmptable->count);break;}
	    dcol_add(tcol,create_cell(_s));//
	    tcol=tcol->next;//
	    if (!tcol) {printf("There isn`t col named %s\n",tcol->name);  continue;}	    
	 	}//---------------------------------------------
	 	                }
	 _step
	 if (_s[0]==';') {puts("Inserting succesfull");return 2;}
	 return -2;}
//=========================================SELECT=======================================	 
	 C_SELECT
	 {//puts("select found");
	 dptodo * todo=NULL;
	 _step
	 if (!isSymbol(_s)) {printf("unexpected input%s\n",_s);return -3;}
	 if (!strcmp(_s,"FROM")) { _step }
	 dtable * tmptable = dbase_find(base,_s);//

	 if (!tmptable) {printf("There isn`t table %s",_s);return -3;} else 
	 		printf("Table %s found\n",_s);
	 _step
	 if (!strcmp(_s,"WHERE")) {//puts ("WHERE acheived");
while(1){
	 _step
	 if (_s[0]=='=') continue;
	 	 if (_s[0]=='<') continue;
	 	 	 if (_s[0]=='>') continue;
	 	 	 	 if (_s[0]=='|') continue;
	 	 	 	 if (_s[0]==';') break;
	 	 	 	 if (_s[0]=='"'){_step _step }
	 if (isSymbol(_s)) {
	todo = dtable_create_todo(tmptable,_s);}
         
         }
         dtable_print_todo(todo);
	}
	
          if (_s[0]==';') {puts("Inserting succesfull");return 3;}
	 
	 
	 return 3;}
//=======================================DELETE==========================================
	 C_DELETE
	{//puts("delete found");
	 return 4;}
	 
//=======================================DESCRIBE========================================
          C_DESCRIBE 
          {// puts("describe found");
          _step
            if(!isSymbol(_s))  {printf("unexpected input%s\n",_s);return -4;}
          	 dtable * tmptable = dbase_find(base,_s);//
             if (!tmptable) {printf("There isn`t table %s",_s);return -4;} else 
	 		{printf("Table %s found\n",_s);
	 	for (dcol * c=tmptable->tail;c!=NULL;c=c->next)
	 		printf("Column name-|%s|\n",c->name);
	 		}
	 		_step
	             if (_s[0]==';') {puts("Describyng succesfull");return 4;}
          return -4;}


	 
	 
//puts("Unrekognized command. Skipping...");	 
	 
	    
//все прошло успешно очищаем команду
return 0;
}
