#define UNUSED(x) ((void)(x))
#include "../common/types.h"
#include "../common/syscall_nb.h"
#include "../common/term.h"
#include "gdt.h"
#include "keyboard.h"
#include "timer.h"
#include "fs.h"
#include "task.h"
#include "x86.h"

/*
 Display String
*/
int syscall_puts(char* c){
    term_puts(c);
    return 0;
}

/*
 Execute File 
*/
int syscall_exec(char* filename){
    return run_task(filename);
}

/*
 Wait a char of user (Blocking)
*/
int syscall_getc(char* c){
    *c = (char)getc();
    return 0;
}

/*
 Get file Stat
*/
int syscall_file_stat(char *filename, stat_t *stat){
    return file_stat(filename, stat);
}

/*
 Read count char of a File
*/
int syscall_file_read(int fd, void *buf, uint count){
    return file_read(fd, buf, count);
}

/*
 Remove File 
*/
int syscall_file_remove(char *filename){
    return file_remove(filename);
}

/*
 Create File Iterator
*/
int syscall_file_iterator(file_iterator_t *it){
    *it = file_iterator();
    return 0;
}

/*
 Go to next File 
*/
int syscall_file_next(char *filename, file_iterator_t *it){
    file_next(filename, it);
    return 0;
}

/*
 Get ticks of system
*/
int syscall_get_ticks(int* t){
    *t = (int)get_ticks();
    return 0;
}

/*
 Sleep Passive
*/
int syscall_sleep(uint ms){
    sleep(ms);
    return 0;
}

/*
 If iterator hase next
*/
int syscall_file_has_next(file_iterator_t *it, bool *result){
    *result = file_has_next(it);
    return 0;
}

/*
 Display char
*/
int syscall_putc(char c){
    term_putc(c);
    return 0;
}

/*
 Close File
*/
int syscall_file_close(int fd){
    file_close(fd);
    return 0;
}

/*
 Open File
*/
int syscall_file_open(char *filename){
    return file_open(filename);
}

// System call handler: call the appropriate system call according to the nb argument.
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t caller_tss_selector) {
    
    //Compute base with caller_tss_selector
    gdt_entry_t ldt = get_entry((caller_tss_selector / 16) - 2); 
    uint32_t base = (ldt.base15_0 | (ldt.base23_16 << 16) | (ldt.base31_24 << 24));
    
    // For not display Warning 
    UNUSED(arg1);
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);
    
    int code_error = 0;
    // Add base to pointer value to access to absolute adress 
    switch(nb){
        case SYSCALL_PUTS:
            arg1+=base;
            code_error = syscall_puts((char*)arg1);
            break;
            
        case SYSCALL_EXEC: 
            arg1+=base;
            code_error = syscall_exec((char*)arg1);
            break;
            
        case SYSCALL_GETC:
            arg1+=base;
            code_error =  syscall_getc((char*)arg1);
            break;
            
        case SYSCALL_FILE_STAT:
            arg1+=base;
            arg2+=base;
            code_error =  syscall_file_stat((char*)arg1, (stat_t*)arg2);
            break;
            
        case SYSCALL_FILE_READ:
            arg2+=base;
            code_error = syscall_file_read((int)arg1, (void*)arg2, (uint)arg3);
            break;
            
        case SYSCALL_FILE_REMOVE:
            arg1+=base;
            code_error =  syscall_file_remove((char*)arg1);
            break;
            
        case SYSCALL_FILE_ITERATOR:
            arg1+=base;
            code_error = syscall_file_iterator((file_iterator_t*)arg1);
            break;
        
        case SYSCALL_FILE_NEXT:
            arg1+=base;
            arg2+=base;
            code_error = syscall_file_next((char*)arg1,(file_iterator_t*)arg2);
            break;
        
        case SYSCALL_GET_TICKS:
            arg1+=base;
            code_error =  syscall_get_ticks((int*)arg1);
            break;
        
        case SYSCALL_SLEEP:
            code_error =  syscall_sleep((uint)arg1);
            break;
       
        case SYSCALL_FILE_HAS_NEXT:
            arg1+=base;
            arg2+=base;
            code_error = syscall_file_has_next((file_iterator_t*)arg1,(bool *)arg2);
            break;
            
        case SYSCALL_PUTC:
            code_error = syscall_putc((char)arg1);
            break;
            
        case SYSCALL_FILE_CLOSE:
            code_error = syscall_file_close((int)arg1);
            break;
            
        case SYSCALL_FILE_OPEN:
            arg1+=base;
            code_error = syscall_file_open((char*)arg1);
            break;
            
        default: 
            code_error = 1;
    }
    return code_error;
}
