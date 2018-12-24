extern main
global exit

section .entrypoint
align 4

    mov     [stack_ptr],esp
    call    main
    mov     esp,[stack_ptr]
    iret

section .text
align 4

exit:
    mov     esp,[stack_ptr]
    iret

section .bss
stack_ptr:
    resd 1    ; uninitialized dword (32 bits)


