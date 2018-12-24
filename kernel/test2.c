#define MAX_FILENAME_LENGTH 51

void list_file(){
    char filename[MAX_FILENAME_LENGTH];
    stat_t st;
    file_iterator_t it = file_iterator();
    
    while (file_has_next(&it)) {
        file_next(filename, &it);
        file_stat(filename, &st);
        term_printf("%s (%d bytes)\n", st.name, st.size);
    }
}

int test2 (){
	stat_t stat;
	
	sleep(100);
	if(file_stat("artscii.txt", &stat) >= 0){
		term_printf("name : %s \n", stat.name);
		term_printf("size : %d \n", stat.size);
	}else{
		term_printf("file artscii.txt not found \n");
	}
	
	sleep(100);
	if(file_stat("notfound.txt", &stat)  >= 0){
	    term_printf("file notfound.txt found \n");
	}else{
        term_printf("file notfound.txt not found \n");
	}
	
	sleep(100);
	if(file_exists("artscii.txt")){
	    term_printf("file artscii exist \n");
	}else{
	    term_printf("file artscii doesn't exist \n");
	}
	
	sleep(100);
	if(file_exists("notfound.txt")){
	    term_printf("file notfound.txt exist \n");
	}else{
	    term_printf("file notfound.txt doesn't exist \n");
	}
    
	sleep(100);
    list_file();
	sleep(100);
    term_printf("Remove fs.c \n");
	sleep(100);
    file_remove("fs.c");
    list_file();
	sleep(100);
    
	return 0;
}