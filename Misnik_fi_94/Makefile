all:
	gcc main.c dlist.c parse_str.c parse_comm.c dbase.c -o mydb -lm
zip:
	zip -r mydb.zip * *.* *.t
run:
	./mydb comm.t

