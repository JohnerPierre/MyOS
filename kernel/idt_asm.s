%include "const.inc"
extern exception_handler
extern interruption_handler

section .text   ; start of the text (code) section
align 4         ; the code must be 4 byte aligned

global idt_load
idt_load:
    mov     eax,[esp+4]  ; Get the pointer to the IDT, passed as a parameter.
    lidt    [eax]        ; Load the IDT pointer
    ret

;------------------------------------------------
; CPU exceptions

global _divide_error
_divide_error:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0    ; exception number
    jmp     exception_wrapper

global _reserved
_reserved:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    1    ; exception number
    jmp     exception_wrapper

global _nmi_interrupt
_nmi_interrupt:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    2    ; exception number
    jmp     exception_wrapper

global _breakpoint
_breakpoint:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    3    ; exception number
    jmp     exception_wrapper

global _overflow
_overflow:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    4    ; exception number
    jmp     exception_wrapper

global _bound_range_exceeded
_bound_range_exceeded:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    5    ; exception number
    jmp     exception_wrapper

global _invalid_opcode
_invalid_opcode:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    6    ; exception number
    jmp     exception_wrapper

global _device_not_available
_device_not_available:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    7    ; exception number
    jmp     exception_wrapper

global _double_fault
_double_fault:
    cli          ; disable interrupts
    push    8    ; exception number
    jmp     exception_wrapper

global _coprocessor_segment_overrun
_coprocessor_segment_overrun:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    9    ; exception number
    jmp     exception_wrapper

global _invalid_tss
_invalid_tss:
    cli           ; disable interrupts
    push    10    ; exception number
    jmp     exception_wrapper

global _segment_not_present
_segment_not_present:
    cli           ; disable interrupts
    push    11    ; exception number
    jmp     exception_wrapper

global _stack_segment_fault
_stack_segment_fault:
    cli           ; disable interrupts
    push    12    ; exception number
    jmp     exception_wrapper

global _general_protection
_general_protection:
    cli           ; disable interrupts
    push    13    ; exception number
    jmp     exception_wrapper

global _page_fault
_page_fault:
    cli           ; disable interrupts
    push    14    ; exception number
    jmp     exception_wrapper

global _intel_reserved
_intel_reserved:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    15    ; exception number
    jmp     exception_wrapper

global _math_fault
_math_fault:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    16    ; exception number
    jmp     exception_wrapper

global _alignment_check
_alignment_check:
    cli           ; disable interrupts
    push    17    ; exception number
    jmp     exception_wrapper

global _machine_check
_machine_check:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    18   ; exception number
    jmp     exception_wrapper

global _simd_floating_point_exception
_simd_floating_point_exception:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    19   ; exception number
    jmp     exception_wrapper

global _virtualization_exception
_virtualization_exception:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    20   ; exception number
    jmp     exception_wrapper

;------------------------------------------------
; IRQ

global _irq_system_timer
_irq_system_timer:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    0    ; irq number
    jmp     irq_wrapper

global _irq_keyboard
_irq_keyboard:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    1    ; irq number
    jmp     irq_wrapper

global _redirected_slave
_redirected_slave:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    2    ; irq number
    jmp     irq_wrapper

global _serial_port_2_4
_serial_port_2_4:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    3    ; irq number
    jmp     irq_wrapper

global _serial_port_1_3
_serial_port_1_3:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    4    ; irq number
    jmp     irq_wrapper

global _sound_card
_sound_card:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    5    ; irq number
    jmp     irq_wrapper

global _floppy_disk_controller
_floppy_disk_controller:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    6    ; irq number
    jmp     irq_wrapper

global _parallel_port
_parallel_port:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    7    ; irq number
    jmp     irq_wrapper

global _real_time_clock
_real_time_clock:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    8    ; irq number
    jmp     irq_wrapper

global _redirected_irq2
_redirected_irq2:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    9    ; irq number
    jmp     irq_wrapper

global _reserved_1
_reserved_1:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    10   ; irq number
    jmp     irq_wrapper

global _reserved_2
_reserved_2:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    11   ; irq number
    jmp     irq_wrapper

global _ps2_mouse
_ps2_mouse:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    12   ; irq number
    jmp     irq_wrapper

global _math_coprocessor
_math_coprocessor:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    13   ; irq number
    jmp     irq_wrapper

global _hard_disk_controller
_hard_disk_controller:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    14   ; irq number
    jmp     irq_wrapper

global _reserved_3
_reserved_3:
    cli          ; disable interrupts
    push    0    ; dummy error code
    push    15   ; irq number
    jmp     irq_wrapper
;------------------------------------------------
; Wrapper for exceptions

exception_wrapper:
    ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    exception_handler  ; implemented in idt.c
    pop     eax  ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
    
	; Fix the stack pointer due to the 2 push done before the call to
	; exception_wrapper: error code and exception/irq number
    add     esp,8
    iret

;------------------------------------------------
; Wrapper for IRQ
irq_wrapper:
   ; Save all registers
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi
    push    edi
    push    ebp
    push    ds
    push    es
    push    fs
    push    gs

    ; Load kernel data descriptor into all segments
    mov     ax,GDT_KERNEL_DATA_SELECTOR
    mov     ds,ax
    mov     es,ax
    mov     fs,ax
    mov     gs,ax

    ; Pass the stack pointer (which gives the CPU context) to the C function
    mov     eax,esp
    push    eax
    call    interruption_handler  ; implemented in idt.c
    pop     eax  ; only here to balance the "push eax" done before the call

    ; Restore all registers
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     ebp
    pop     edi
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax
    
	; Fix the stack pointer due to the 2 push done before the call to
	; exception_wrapper: error code and exception/irq number
    add     esp,8
    iret
