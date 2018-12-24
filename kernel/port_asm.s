global outw
global outb
global inb
global inw

;void outb(uint16_t port, uint8_t data)
outb:
    ;Push arguments 
    push	ebp
    mov		ebp,esp

    mov dx,[ebp+8]
    mov al,[ebp+12]
    
    out dx,al

	leave
    ret

;void outw(uint16_t port, uint16_t data)
outw:
    ;Push arguments 
    push	ebp
    mov		ebp,esp
    
    mov dx,[ebp+8]
    mov ax,[ebp+12]
    
    out dx,ax
    
	leave
    ret
    
;
inb:
    ;Push arguments 
    push	ebp
    mov		ebp,esp
	
    ;Read 
    mov     dx,[ebp+8]
    in      al,dx
    
	leave
    ret

;uint16_t inb(uint16_t port)
inw:
    ;Push arguments 
    push	ebp
    mov		ebp,esp
	
    ;Read 
    mov     dx,[ebp+8]
    in      ax,dx
    
	leave
	ret


