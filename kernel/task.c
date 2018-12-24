#include "gdt.h"
#include "task.h"
#include "x86.h"
#include "library.h"
#include "fs.h"
#include "../common/term.h"

#define TASK_SIZE 8
#define SPACE_SIZE 0x100000-1

int counter = 0;
//All static array for task
static tss_t task_tss_table[TASK_SIZE];
static gdt_entry_t task_ldt_table[TASK_SIZE][2];
static uint8_t addr_space_table[TASK_SIZE][SPACE_SIZE] __attribute__((aligned(4096)));
static uint8_t task_kernel_stack_table[TASK_SIZE][8192];
static uint8_t gdt_tss_sel_table[TASK_SIZE];

//Setup a new task in task array
void setup_task(gdt_entry_t* gdt_table, int id_task){
    //id_array
	int id = id_task/2-2;

    memset(&task_tss_table[id], 0, sizeof(tss_t));
    
    gdt_table[id_task]= gdt_make_tss(&task_tss_table[id], DPL_KERNEL);
    gdt_table[id_task+1]= gdt_make_ldt((uint32_t)task_ldt_table[id], sizeof(task_ldt_table[id])-1, DPL_KERNEL);
    int gdt_tss_sel = gdt_entry_to_selector(&gdt_table[id_task]);
    int gdt_ldt_sel = gdt_entry_to_selector(&gdt_table[id_task+1]);
    gdt_tss_sel_table[id] = gdt_tss_sel;
    
	int ldt_code_idx = 0;  // Index of code segment descriptor in the LDT
	int ldt_data_idx = 1;  // Index of data segment descriptor in the LDT
	uint limit = sizeof(addr_space_table[id]);  // Limit for both code and data segments
	task_ldt_table[id][ldt_code_idx] = gdt_make_code_segment((uint32_t)addr_space_table[id], limit / 4096, DPL_USER);
	task_ldt_table[id][ldt_data_idx] = gdt_make_data_segment((uint32_t)addr_space_table[id], limit / 4096, DPL_USER);
	
	task_tss_table[id].ldt_selector = gdt_ldt_sel;
	task_tss_table[id].eip = 0;
    task_tss_table[id].esp = task_tss_table[id].ebp = limit;  // stack pointers
    
    task_tss_table[id].cs = gdt_index_to_selector(ldt_code_idx) | DPL_USER | LDT_SELECTOR;
    task_tss_table[id].ds = task_tss_table[id].es = task_tss_table[id].fs = task_tss_table[id].gs = task_tss_table[id].ss = gdt_index_to_selector(ldt_data_idx) | DPL_USER | LDT_SELECTOR;
    task_tss_table[id].eflags = 512;  // Activate hardware interrupts (bit 9)

	task_tss_table[id].ss0 = GDT_KERNEL_DATA_SELECTOR;
	task_tss_table[id].esp0 = (uint32_t)(task_kernel_stack_table[id]) + sizeof(task_kernel_stack_table[id]);
		
	return;
}

//Return the base in function of id
gdt_entry_t get_entry(int id){
    return task_ldt_table[id][1];
}

//Add a new task
int add_task(){
    int id = -1;
    if(counter < (TASK_SIZE-1))
    {
        id = counter;
        counter++;
    }
    return id;
}

//Free a task 
void free_task(){
    if(counter > 0){
        counter--;
    }  
}

//Run a new task
int run_task(char *filename){
    int id_task = add_task();
    if(id_task != -1)
    {
        int id =  (2*id_task)+4;
        //Setup id task
        gdt_setup_task(id);
        //Get the selector for switch
        uint16_t gdt_tss_sel = gdt_tss_sel_table[id_task];
    	int file = file_open(filename);

        if(file != -1){
            //Load code in memory
        	file_read(file, addr_space_table[id_task], SPACE_SIZE);
            file_close(file);
            task_switch(gdt_tss_sel);
            free_task();
            return 0;
        }else{
            free_task();
            return -1;
        }
    }else{
        return -1;
    }
}