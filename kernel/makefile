mode=$(mode)
flag=-std=gnu99 -m32 -fno-builtin -nostdlib -ffreestanding -Wall -Wextra

kernel: kernel.o bootloader_asm.o gdt.o gdt_asm.o library.o screen.o cursor.o port_asm.o keyboard.o timer.o idt.o idt_asm.o pic.o ide.o fs.o syscall.o syscall_asm.o task_asm.o task.o test1.c test2.c
	ld -Tkernel.ld -melf_i386 kernel.o ../common/term.o bootloader_asm.o gdt.o gdt_asm.o library.o screen.o cursor.o port_asm.o keyboard.o timer.o idt.o idt_asm.o pic.o ide.o fs.o syscall.o syscall_asm.o task_asm.o task.o -o mykernel.elf 
kernel.o: kernel.c multiboot.h gdt.h library.h
	gcc $(flag) $(mode) $< -c 
bootloader_asm.o: bootloader_asm.s
	nasm -f elf32 $<
gdt.o: gdt.c ../common/types.h gdt.h x86.h library.h task.h
	gcc $(flag) $(mode) $< -c
gdt_asm.o: gdt_asm.s
	nasm -f elf32 $<
library.o: library.c library.h
	gcc $(flag) $(mode) $< -c
screen.o: screen.c screen.h
	gcc $(flag) $(mode) $< -c
cursor.o: cursor.c cursor.h
	gcc $(flag) $(mode) $< -c
port_asm.o: port_asm.s
	nasm -f elf32 $<
keyboard.o: keyboard.c keyboard.h port.h
	gcc $(flag) $(mode) $< -c
timer.o: timer.c timer.h
	gcc $(flag) $(mode) $< -c
idt.o: idt.c idt.h 
	gcc $(flag) $(mode) $< -c
idt_asm.o: idt_asm.s
	nasm -f elf32 $<
pic.o: pic.c pic.h 
	gcc $(flag) $(mode) $< -c
ide.o: ide.c ide.h 
	gcc $(flag) $(mode) $< -c
fs.o: fs.c fs.h 
	gcc $(flag) $(mode) $< -c
syscall.o: syscall.c ../common/types.h ../common/syscall_nb.h ../common/term.h
	gcc $(flag) $(mode) $< -c
syscall_asm.o: syscall_asm.s
	nasm -f elf32 $<
task_asm.o: task_asm.s
	nasm -f elf32 $<
task.o: task.c task.h gdt.h x86.h library.h
	gcc $(flag) $(mode) $< -c
clean:
	rm -f *.o