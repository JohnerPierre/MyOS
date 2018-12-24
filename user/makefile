flag=-std=gnu99 -m32 -fno-builtin -nostdlib -ffreestanding -Wall -Wextra

main: shell game test

game: game.o ulibc.o entrypoint_asm.o syscall_asm.o app.ld
	ld -Tapp.ld -melf_i386 game.o ulibc.o entrypoint_asm.o syscall_asm.o app.ld -o game	

shell: shell.o ulibc.o entrypoint_asm.o syscall_asm.o app.ld
	ld -Tapp.ld -melf_i386 shell.o ulibc.o entrypoint_asm.o syscall_asm.o -o shell	
	
test: test.o ulibc.o entrypoint_asm.o syscall_asm.o app.ld
	ld -Tapp.ld -melf_i386 test.o ulibc.o entrypoint_asm.o syscall_asm.o -o test	

test.o: test.c ulibc.h
	gcc $(flag) $(mode) $< -c 	

game.o: game.c ulibc.h
	gcc $(flag) $(mode) $< -c 
	
shell.o: shell.c ulibc.h
	gcc $(flag) $(mode) $< -c 

ulibc.o: ulibc.c ulibc.h
	gcc $(flag) $(mode) $< -c 	
	
entrypoint_asm.o: entrypoint_asm.s
	nasm -f elf32 $<
	
syscall_asm.o: syscall_asm.s
	nasm -f elf32 $<
	
clean:
	rm -f *.o