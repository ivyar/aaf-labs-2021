#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
//printf("-stp-s=%s-\n",p->str);puts("unexpected end");
#define  _step if (p->next) {p=p->next;} else {return -10;}
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
int count=0;
char * tmst=NULL;
//устанавливаем укзатель в начало списка			        
if (!list->tail) return 0;
 dnode * p = list->tail;
_step
_stback
if (!list->count) return 0;
//-------------------------------------CREATE----------------------	
	C_CREATE //если команда создать создаем бесконечный цикл для удобства
	{ puts("Create table recognized");  
        
        _step;//смещаемся на следующий элемент команду уже обработали
       
        
        
        if (!isDivider(_s)) {printf("Table name %s is specifyed \n",_s);}//смотрим имя таблицы
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
                		puts(" Open bracket ( found - adding columns");
                			
                		while(1){//заводим цикл по скобкам
	 _step
	    if (_s[0]==')') {puts("Bracket closed, Success");break;}
	    if (_s[0]==',') {puts(", found");continue;}
	    if (_s[0]=='"') {puts(" string found");  continue;}
	    if (!isCharacter(_s)) {printf("unexpected input%s\n",_s);return -2;}
            if (!strcmp(_s,"INDEXED"))//и проверяем индексируемый ли столбец
            {puts("INDEXED found");//если да создаем столбец как индексируемый
	      		_step;//шагаем
	      			_step;//шагаем
	      		dtable_add(t,dcol_create(_s,true));
	                		}else//если нет создаем как неиндексируемый
	                			{puts("col is not INDEXED");
						dtable_add(t,dcol_create(_s,false));}
                			}
                		}
                		_step
                		if (_s[0]==';') {
                			printf("Table %s succesfully created",t->name);
                			printf("with %i columns added",t->count);
                			return 1;}
              puts ("Unsuspected end of command!");
              return -1;
	    }// CREATE END---------------------------
//===============================================INSERT=================================	 
	 C_INSERT
	  
	 {puts("insert found");
	 _step
	 if (!isSymbol(_s)) {printf("unexpected input%s\n",_s);return -2;}
	 if (!strcmp(_s,"INTO")) {puts ("INTO acheived"); _step }
	 dtable * tmptable = dbase_find(base,_s);//
	 if (!tmptable) {printf("There isn`t table %s",_s);return -2;} else 
	 		{printf("Table %s found\n",_s);
	 		printf("%i columns there\n",tmptable->count);
	 		}
	 // дальше у нас может быть либо ; либо скобки с именами и типами столбцов для таблицы
         _step ; 
                if (_s[0]==';') {return 2;}//если находим ; то уходим без ошибки
                	if (_s[0]=='(') {//находим открытые скобки
                		puts(" Open bracket ( found - adding cells");	 
                dcol * tcol = tmptable->tail;
	
	 while(1){//-------------------------------------------
	 _step
	    if (_s[0]==')') {puts("Bracket closed, Success");break;}
	    if (_s[0]==',') {puts(", found");continue;}
	    if (_s[0]=='"') {printf(" string %i found\n",count); count++; continue;}
	    if (!isCharacter(_s)) {printf("unexpected input%s\n",_s);return -2;}
            if (count>(tmptable->count*2)) 
            		{printf("trying to add more cols there in table%li\n",tmptable->count);break;}
	    dcol_add(tcol,create_cell(_s));//
	    tcol=tcol->next;//
	    if (!tcol) {printf("There isn`t col named %s\n",tcol->name);  continue;}	    
	 	}//---------------------------------------------
	 	                }
	 _step
	 if (_s[0]==';') {printf("Inserting %i cols succesfull\n",count/2);return 2;}
	 return 2;}
//=========================================SELECT=======================================	 
	 C_SELECT
	 {puts("select found");
	 dptodo * todo=NULL;
	 _step
	 if (_s[0]=='*') {	puts("Print whole table");
	 		_step;
	 		 if (!strcmp(_s,"FROM")) {puts ("FROM acheived");} else 
	 		 	{puts("FROM expected but not founf");return -3;}
	 		_step;
	 		printf("Searching tabl %s\n",_s);
	 		 dtable * tmptable = dbase_find(base,_s);//
			 if (!tmptable) {printf("There isn`t table %s",_s);return -3;} else 
		 		printf("Table %s found\n",_s);
		 	dtable_print_all(tmptable);//
		 	_step;
		 	 if (_s[0]==';') {puts("Select succesfull");return 3;}
		 	 puts("Unexpected end of SELECT");
		 	return -3;	
	 		}
	 if (!isCharacter(_s)) {printf("unexpected input%s\n",_s);return -3;}
	 if (!strcmp(_s,"FROM")) {puts ("FROM acheived"); _step }
	 dtable * tmptable = dbase_find(base,_s);//

	 if (!tmptable) {printf("There isn`t table %s",_s);return -3;} else 
	 		printf("Table %s found\n",_s);
	 _step
	 if (!strcmp(_s,"WHERE")) {puts ("WHERE acheived");
while(1){
	 _step
	 if (_s[0]=='=') continue;
	 	 if (_s[0]=='<') continue;
	 	 	 if (_s[0]=='>') continue;
	 	 	 	 if (_s[0]=='|') continue;
	 	 	 	 if (_s[0]==')') break;
	 	 	 	 if (_s[0]==',') continue;
	 	 	 	 if (_s[0]=='"'){continue;}
	 if (isCharacter(_s)) 	 
	if (!todo) todo = dtable_create_todo(tmptable,_s); else{
	printf("Trying %s in todo add\n",_s);
	todo=dtable_todo_add(todo,_s);	 
         }
         
         }
         dtable_print_todo(todo);
	}
	
          if (_s[0]==';') {puts("Select succesfull");return 3;}
	 
	 
	 return 3;}
//=======================================DELETE==========================================
	 C_DELETE
	 {puts("delete found");
	 dcol * tmpcol;
	 
	 _step
	 if (!isCharacter(_s)) {printf("unexpected input%s\n",_s);return -3;}
	 if (!strcmp(_s,"FROM")) {puts ("FROM acheived"); _step }
	 dtable * tmptable = dbase_find(base,_s);//

	 if (!tmptable) {printf("There isn`t table %s",_s);return -3;} else 
	 		printf("Table %s found\n",_s);
	 _step
	 if (!strcmp(_s,"WHERE")) {puts ("WHERE acheived");
while(1){
	 _step
	 if (_s[0]=='=') continue;
	 	 if (_s[0]=='<') continue;
	 	 	 if (_s[0]=='>') continue;
	 	 	 	 if (_s[0]=='|') continue;
	 	 	 	 if (_s[0]==')') break;
	 	 	 	 if (_s[0]==',') continue;
	 	 	 	 if (_s[0]=='"'){continue;}
	 if (isCharacter(_s)) 	 {
	
	printf("Trying %s in todo add\n",_s);
	tmpcol = dtable_find(tmptable,_s);
	if (!tmpcol) {printf("Column named %s in table %s not found\n",_s,tmptable->name);} else
	if (!dtable_remove(tmpcol)) {printf("Column %s succesfully removed\n",tmpcol->name);}
         }
         
         }
         
	}
	
          if (_s[0]==';') {puts("Delet succesfull");return 3;}
	 	 
	 
	 return 4;}
	 
//=======================================DESCRIBE========================================
          C_DESCRIBE 
          { puts("describe found");
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


	 
	 
puts("Unrekognized command. Skipping...");	 
	 
	    
//все прошло успешно очищаем команду
return 0;
}
