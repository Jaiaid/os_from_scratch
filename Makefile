CC="/home/laptop/Documents/programming/OS/crosscompiler/bin/i686-elf-gcc"
LNK="/home/laptop/Documents/programming/OS/crosscompiler/bin/i686-elf-ld"
FASM="/home/laptop/application/fasm/fasm"

CC_FLAG=-ffreestanding -m32 -fno-pie
LNK_FLAG=-melf_i386

MACRO=macro.h
BOOT=bootloader
KERNEL=kernel
GDT=gdt
INT=interrupt
SCH=scheduler
DD=displaydriver
PORT=portIO
PCI=pci
KD=keyboarddriver
LIB=lib
MEM=memory
LINK=link.ld

#
harddisk.bin  :  bootloader.bin  kernel.bin
		cat bootloader.bin kernel.bin > harddisk.bin 
		truncate -s %4096 harddisk.bin 
#
bootloader.bin  :  $(BOOT)/bootloader.s
		$(FASM) $(BOOT)/bootloader.s
		mv $(BOOT)/bootloader.bin bootloader.bin
# 
kernel.bin  :  $(KERNEL)/kernel_entry.o  $(KERNEL)/kernel.o  $(INT)/idt.o  $(DD)/displaydriver_entry.o  $(DD)/displaydriver.o  $(PCI)/pci.o   $(SCH)/timer_entry.o   $(MEM)/memory.o   $(MEM)/memory_asm.o   $(KERNEL)/misc.o    $(LINK)
		$(LNK) $(LNK_FLAG) -T link.ld
#
$(KERNEL)/kernel.o  :  $(KERNEL)/kernel.c  $(LIB)/IO.h  $(MACRO)
		$(CC) $(CC_FLAG) -c $(KERNEL)/kernel.c -o $(KERNEL)/kernel.o
#
$(KERNEL)/kernel_entry.o  : $(KERNEL)/kernel_entry.s
		$(FASM) $(KERNEL)/kernel_entry.s  
#
$(KERNEL)/misc.o :  $(KERNEL)/misc.s
		$(FASM) $(KERNEL)/misc.s
#
$(INT)/idt.o  :  $(INT)/idt.c   $(MACRO)
		$(CC) $(CC_FLAG) -c $(INT)/idt.c -o $(INT)/idt.o
#
$(DD)/displaydriver.o  :  $(DD)/displaydriver.c   $(MACRO)  $(PORT)/port_IO.h  $(DD)/displaydriver_entry.o
		$(CC) $(CC_FLAG) -c $(DD)/displaydriver.c -o $(DD)/displaydriver.o
#
$(DD)/displaydriver_entry.o  :  $(DD)/displaydriver_entry.s
		$(FASM) $(DD)/displaydriver_entry.s
#
#$(PORT)/port_IO.o  :  $(PORT)/port_IO.s
#		$(FASM) $(PORT)/port_IO.s
#
$(KD)/keyboarddriver_entry.o : $(KD)/keyboarddriver_entry.s
		$(FASM) $(KD)/keyboarddriver_entry.s
#
$(KD)/keyboarddriver.o : $(KD)/keyboarddriver.c  $(PORT)/port_IO.h
		$(CC) $(CC_FLAG) -c $(KD)/keyboarddriver.c -o $(KD)/keyboarddriver.o
#
$(SCH)/timer_entry.o : $(SCH)/timer_entry.s
		$(FASM) $(SCH)/timer_entry.s
#
$(PCI)/pci.o : $(PCI)/pci.c
		$(CC) $(CC_FLAG) -c $(PCI)/pci.c -o $(PCI)/pci.o
#
$(MEM)/memory.o : $(MEM)/memory.c
		$(CC) $(CC_FLAG) -c $(MEM)/memory.c -o $(MEM)/memory.o
#
$(MEM)/memory_asm.o : $(MEM)/memory_asm.s
		$(FASM) $(MEM)/memory_asm.s
#
clean:
	./clear.sh
