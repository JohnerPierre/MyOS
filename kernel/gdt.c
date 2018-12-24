#include "gdt.h"
#include "task.h"
#include "x86.h"
#include "library.h"

#define GDT_INDEX_TO_SELECTOR(idx) ((idx) << 3)
#define GDT_SIZE 19

// declare the GDT table
static gdt_entry_t gdt_table[GDT_SIZE];
// Pointeur sur la table GDT
static gdt_ptr_t   gdt_ptr;

// Build and return a GDT entry.
// base is the base of the segment
// limit is the limit of the segment (NOTE: it's a 20-bit value)
// type is the type of segment
// s indicates whether it's a system segment
// db indicates whether it's a code/data segment or TSS, LDT or gate
// granularity indicates 1 byte or 4KB granularity
// dpl is the privilege level
static gdt_entry_t build_entry(uint32_t base, uint32_t limit, uint8_t type, uint8_t s, uint8_t db,
                               uint8_t granularity, uint8_t dpl) {
	gdt_entry_t entry;
    // For a TSS and LDT, base is the addresse of the TSS/LDT structure
    // and limit is the size of the structure.
    entry.lim15_0 = limit & 0xffff;
    entry.base15_0 = base & 0xffff;
    entry.base23_16 = (base >> 16) & 0xff;
    entry.type = type;  // See TYPE_xxx flags
    entry.s = s;        // 1 for segments; 0 for system (TSS, LDT, gates)
    entry.dpl = dpl;    // privilege level
    entry.present = 1;  // present in memory
    entry.lim19_16 = (limit >> 16) & 0xf;
    entry.avl = 0;      // available for use
    entry.l = 0;        // should be 0 (64-bit code segment)
    entry.db = db;      // 1 for 32-bit code and data segments; 0 for system (TSS, LDT, gate)
    entry.granularity = granularity;  // granularity of the limit value: 0 = 1 byte; 1 = 4096 bytes
    entry.base31_24 = (base >> 24) & 0xff;
	return entry;
}

// Create a NULL "segment".
static gdt_entry_t gdt_make_null_segment() {
	gdt_entry_t entry;
	memset(&entry, 0, sizeof(gdt_entry_t));
	return entry;
}

// Create a code segment specified by the base, limit and privilege level passed in arguments.
gdt_entry_t gdt_make_code_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_CODE_EXECREAD, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

// Return a data segment specified by the base, limit and privilege level passed in arguments.
gdt_entry_t gdt_make_data_segment(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_DATA_READWRITE, S_CODE_OR_DATA, DB_SEG, 1, dpl);
}

// Return a TSS entry  specified by the TSS structure and privilege level passed in arguments.
// NOTE: a TSS entry can only reside in the GDT!
gdt_entry_t gdt_make_tss(void *tss, uint8_t dpl) {
	return build_entry((uint32_t)tss, sizeof(tss_t)-1, TYPE_TSS, S_SYSTEM, DB_SYS, 0, dpl);
}

// Can only be set in the GDT!
gdt_entry_t gdt_make_ldt(uint32_t base, uint32_t limit, uint8_t dpl) {
	return build_entry(base, limit, TYPE_LDT, S_SYSTEM, DB_SYS, 0, dpl);
}

//Return selector from entry
uint gdt_entry_to_selector(gdt_entry_t *entry) {
	return GDT_INDEX_TO_SELECTOR(entry - gdt_table);
}

//Return the selector from index
uint gdt_index_to_selector(int idx){
    return GDT_INDEX_TO_SELECTOR(idx);
}

//Setup the task with the gdt
void gdt_setup_task(int i){
    setup_task(gdt_table,i);
}


// Initialize the GDT
void gdt_init() {
	// setup gdt_ptr so it points to the GDT and ensure it has the right limit.
	gdt_ptr.base= (uint32_t)gdt_table;
	gdt_ptr.limit =  (sizeof(gdt_entry_t) * GDT_SIZE) - 1;
	//initialize 3 segment descriptors: NULL, code segment, data segment.
	// Code and data segments must have a privilege level of 0.
	gdt_table[0] = gdt_make_null_segment();
	gdt_table[1] = gdt_make_code_segment(0,0x100000-1,DPL_KERNEL);
	gdt_table[2] = gdt_make_data_segment(0,0x100000-1,DPL_KERNEL);

    // Load the GDT
    gdt_load(&gdt_ptr);
    
	// Setup GDT index 3 to store the initial kernel TSS (where CPU state of first task is saved)
	static uint8_t initial_tss_kernel_stack[65536];  // 64KB of stack
	static tss_t initial_tss;
	memset(&initial_tss, 0, sizeof(tss_t));
	gdt_table[3] = gdt_make_tss(&initial_tss, DPL_KERNEL);
	initial_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
	initial_tss.esp0 = ((uint32_t)initial_tss_kernel_stack) + sizeof(initial_tss_kernel_stack);

	// Load the task register to point to the initial TSS selector.
	// IMPORTANT: The GDT MUST be loaded prior to setting up the task register!
	task_ltr(gdt_entry_to_selector(&gdt_table[3]));
}

