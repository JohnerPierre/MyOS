#include "../common/types.h"
#include "library.h"
#include "x86.h"
#include "pic.h"
#include "timer.h"
#include "keyboard.h"
#include "screen.h"
#ifndef _IDT_H_
#define _IDT_H_

// Structure of an IDT descriptor. There are 3 types of descriptors:
// a task-gate, an interrupt-gate, and a trap-gate.
// See 5.11 of Intel 64 & IA32 architectures software developer's manual for more details.
// For task gates, offset must be 0.
typedef struct idt_entry_st {
	uint16_t offset15_0;   // only used by trap and interrupt gates
	uint16_t selector;     // segment selector for trap and interrupt gates; TSS segment selector for task gates
	uint16_t reserved : 8;
	uint16_t type : 5;
	uint16_t dpl : 2;
	uint16_t p : 1;

	uint16_t offset31_16;  // only used by trap and interrupt gates
} __attribute__((packed)) idt_entry_t;

// Structure describing a pointer to the IDT gate table.
// This format is required by the lgdt instruction.
typedef struct idt_ptr_st {
	uint16_t limit;   // Limit of the table (ie. its size)
	uint32_t base;    // Address of the first entry
} __attribute__((packed)) idt_ptr_t;

extern void idt_init();
extern void idt_load(idt_ptr_t *idt_ptr);

extern void _divide_error();
extern void _reserved();
extern void _nmi_interrupt();
extern void _breakpoint();
extern void _overflow();
extern void _bound_range_exceeded();
extern void _invalid_opcode();
extern void _device_not_available();
extern void _double_fault();
extern void _coprocessor_segment_overrun();
extern void _invalid_tss();
extern void _segment_not_present();
extern void _stack_segment_fault();
extern void _general_protection();
extern void _page_fault();
extern void _intel_reserved();
extern void _math_fault();
extern void _alignment_check();
extern void _machine_check();
extern void _simd_floating_point_exception();
extern void _virtualization_exception();

extern void _irq_system_timer();
extern void _irq_keyboard();
extern void _redirected_slave();
extern void _serial_port_2_4();
extern void _serial_port_1_3();
extern void _sound_card();
extern void _floppy_disk_controller();
extern void _parallel_port();
extern void _real_time_clock();
extern void _redirected_irq2();
extern void _reserved_1();
extern void _reserved_2();
extern void _ps2_mouse();
extern void _math_coprocessor();
extern void _hard_disk_controller();
extern void _reserved_3();

extern void _syscall_handler(); 

#endif
