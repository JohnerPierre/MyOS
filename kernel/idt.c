#include "idt.h"

#define SIZE_IDT 256

idt_entry_t idt_table[SIZE_IDT];
static idt_ptr_t idt_ptr;

// CPU context used when saving/restoring context from an interrupt
typedef struct regs_st {
	uint32_t gs, fs, es, ds;
	uint32_t ebp, edi, esi;
	uint32_t edx, ecx, ebx, eax;
	uint32_t number, error_code;
	uint32_t eip, cs, eflags, esp, ss;
} regs_t;

// Build and return an IDT entry.
// selector is the code segment selector to access the ISR
// offset is the address of the ISR (for task gates, offset must be 0)
// type indicates the IDT entry type
// dpl is the privilege level required to call the associated ISR
static idt_entry_t idt_build_entry(uint16_t selector, uint32_t offset, uint8_t type, uint8_t dpl) {
	idt_entry_t entry;
	entry.offset15_0 = offset & 0xffff;
	entry.selector = selector;
	entry.reserved = 0;
	entry.type = type;
	entry.dpl = dpl;
	entry.p = 1;
	entry.offset31_16 = (offset >> 16) & 0xffff;
	return entry;
}

// Interruption handler
void interruption_handler(regs_t *regs) {
    switch(regs->number){
        case 0: 
            timer_handler();
            break;
        case 1:
            keyboard_handler();
            break;
    }
    pic_eoi(regs->number);
}

// Exception handler
void exception_handler(regs_t *regs) {
    setColorText(12);
    print("Exception code %d", regs->number);
    halt();
}

void idt_init() {
	idt_ptr.base = (uint32_t)idt_table;
	idt_ptr.limit =  (sizeof(idt_entry_t) * SIZE_IDT) - 1;
	
	//Init all IDT to 0
	memset(idt_table,0, sizeof(idt_entry_t)* SIZE_IDT);
	 
	//Exeptions 0-20
	idt_table[0] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_divide_error,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[1] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_reserved,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[2] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_nmi_interrupt,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[3] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_breakpoint,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[4] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_overflow,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[5] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_bound_range_exceeded,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[6] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_invalid_opcode,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[7] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_device_not_available,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[8] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_double_fault,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[9] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_coprocessor_segment_overrun,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[10] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_invalid_tss,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[11] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_segment_not_present,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[12] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_stack_segment_fault,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[13] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_general_protection,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[14] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_page_fault,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[15] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_intel_reserved,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[16] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_math_fault,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[17] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_alignment_check,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[18] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_machine_check,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[19] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_simd_floating_point_exception,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[20] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_virtualization_exception,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	
	//IRQ 32-47
	idt_table[32] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_irq_system_timer,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[33] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_irq_keyboard,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[34] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_redirected_slave,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[35] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_serial_port_2_4,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[36] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_serial_port_1_3,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[37] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_sound_card,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[38] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_floppy_disk_controller,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[39] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_parallel_port,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[40] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_real_time_clock,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[41] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_redirected_irq2,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[42] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_reserved_1,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[43] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_reserved_2,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[44] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_ps2_mouse,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[45] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_math_coprocessor,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[46] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_hard_disk_controller,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	idt_table[47] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR,(uint32_t)&_reserved_3,TYPE_INTERRUPT_GATE,DPL_KERNEL);
	
	//System
	idt_table[48] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)&_syscall_handler, TYPE_TRAP_GATE, DPL_USER);
	
	idt_load(&idt_ptr);
}

