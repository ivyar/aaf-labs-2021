



unsigned int dtable_print_todo (dptodo * todo){
// функция печати таблицы согласно задания todo
dcell * c;
unsigned int ind = 0;
unsigned int max_row=todo->col->table->count;//количество строк для вывода
char * s;//

//выводим заговолок - названия столбцов
printf("+");//рисуем таблицу
for (dptodo * prn_col = todo;i!=NULL;i=i->next) printf("--------+");
printf("\n|");//печатаем псевдографику
for (dptodo * prn_col = todo;i!=NULL;i=i->next) printf("%8s|",strncpy(s, prn_col, 8));
printf("\n+");
for (dptodo * prn_col = todo;i!=NULL;i=i->next) printf("--------+");
//печатаем ячейки
  for(unsigned int  i = 1;i!=max_row+1;i++)  {//побегаем по строкам
  	printf("\n%3i|",ind);//псевдографика
  	ind++;
	for (dptodo * prn_col = todo;i!=NULL;i=i->next){//пробегаем по колонкам
           c = dcol_find_cell_index(todo->col,i);//ищем строку в колонке по текущему индексу строки
           if (str) {printf ("%8s|",c->string);} else //печатаем если ячейка в колонке с таким индексом есть
           		puts("  nAn   |");//если нет то данные не укзны при добавлении колонки
	}
	}
printf("\n+");
for (dptodo * prn_col = todo;i!=NULL;i=i->next) printf("--------+");

return ind;}



