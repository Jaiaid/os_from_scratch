INPUT
(
	kernel/kernel_entry.o
	kernel/kernel.o
	interrupt/idt.o
	kernel/devmgr_entry.o
	kernel/devmgr.o
	pci/pci.o
	scheduler/timer_entry.o
	memory/memory.o
	memory/memory_asm.o
)

OUTPUT_FORMAT("binary")

OUTPUT(kernel.bin)

SECTIONS
{
    . = 0x8000;

	.kernel  : {
		kernel/kernel_entry.o( .text )
        kernel/misc.o( .text )
		kernel/kernel.o( .text )
	    memory/memory.o( .text )
		memory/memory_asm.o( .text )
        interrupt/idt.o( .text )
        kernel/devmgr_entry.o( .text )	
		kernel/devmgr.o( .text )
        pci/pci.o( .text )
        scheduler/timer_entry.o( .text )
        displaydriver/displaydriver.o( .text )
	}

    . = 0xa000;

    .memorydata  :  {
		memory/memory.o( .data )
		interrupt/idt.o( .data )
		interrupt/idt.o( .rodata )
	}

	. = 0xb000;

	.kerneldata  : {
		kernel/kernel.o( .data )
		kernel/kernel.o( .rodata )
        kernel/devmgr.o( .data )        
	}
}
