flag=-Wall -Wextra

tools: fs_add fs_create fs_del fs_info fs_list

fs_add: fs_add.o fs.o
	gcc $(flag) fs_add.o fs.o -o fs_add

fs_create: fs_create.o fs.o
	gcc $(flag) fs_create.o fs.o -o fs_create

fs_del: fs_del.o fs.o
	gcc $(flag) fs_del.o fs.o -o fs_del
	
fs_list: fs_list.o fs.o
	gcc $(flag) fs_list.o fs.o -o fs_list
	
fs_info: fs_info.o fs.o
	gcc $(flag) fs_info.o fs.o -o fs_info
	
fs_add.o: fs_add.c 
	gcc $(flag) $< -c 

fs_create.o: fs_create.c
	gcc $(flag) $< -c 

fs_del.o: fs_del.c 
	gcc $(flag) $< -c 

fs_info.o: fs_info.c 
	gcc $(flag) $< -c 

fs_list.o: fs_list.c 
	gcc $(flag) $< -c 

fs.o: fs.c fs.h ../common/types.h
	gcc $(flag) $< -c 

clean:
	rm -f *.o