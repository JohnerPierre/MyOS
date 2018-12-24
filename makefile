default:
	cd kernel && $(MAKE) mode=
	cd tools && $(MAKE)
	cd user && $(MAKE)
	$(MAKE) build
build: fs.img
	mkdir -p myos/boot/grub
	cp grub/menu.lst myos/boot/grub/menu.lst
	cp grub/stage2_eltorito myos/boot/grub/stage2_eltorito
	cp kernel/mykernel.elf myos/boot/mykernel.elf
	genisoimage -R -b boot/grub/stage2_eltorito -input-charset utf8 -no-emul-boot -boot-info-table -o myos.iso myos
run: 
	$(MAKE)
	qemu-system-i386 -cdrom myos.iso -hda fs.img 
debug:
	cd kernel && $(MAKE) mode=-Ddebug
	cd tools && $(MAKE)
	$(MAKE) build
clean:
	cd kernel && $(MAKE) clean
	cd tools && $(MAKE) clean
	cd user && $(MAKE) clean
	rm -f -r myos 
	rm -f myos.iso
	rm -f fs.img
fs.img: tools/fs.c tools/fs.h
	cd tools && ./fs_create fs.img data 512 1024
	cp user/test tools/test
	cp user/shell tools/shell
	cp user/game tools/game
	cd tools && ./fs_add artscii.txt fs.img
	cd tools && ./fs_add test fs.img
	cd tools && ./fs_add shell fs.img
	cd tools && ./fs_add game fs.img
	cp tools/fs.img fs.img