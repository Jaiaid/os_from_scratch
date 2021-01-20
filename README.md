# Experimental coding of OS from scratch

These codes are written following different tutorial on devloping OS from scratch for an intel x86 based IBM PC architecture. By IBM PC architecture, it is meant that what chips are present in the system (e.g. timer, PIC, keyboard chip), I/O port addresses (e.g.  timer, PIC, PCI controller, keyboard controller) are assumed according to this architecture. For processor intel80386 is assumed.

Written code boots pc at 16bit real mode and then switches to 32bit protected mode. In 32bit mode paged memory addressing activation is done but it is not complete (seg. fault handler code, using disk as memory is not done. Actually, disk driver is not implemented at all). Any intel x86_64 based system following IBM PC architecture should be backward compatible with this experiment code. Currently bootloader and kernel is tightly coupled, means it is not attempted to load with grub or other bootloader.

Following sites and books are heavily followed
* [Programming from the Ground Up By Jonathan Bartlett](http://download-mirror.savannah.gnu.org/releases/pgubook/ProgrammingGroundUp-1-0-booksize.pdf) 
* [Writing a Simple Operating System from Scratch By Nick Blundell](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
* [osdev tutorial](https://wiki.osdev.org/Tutorials)

## Build
To build raw harddisk image `harddisk.bin`, run
```
make
```
#### Build Prerequisites
* [fasm](https://flatassembler.net/)
* gcc (tested with 7.5.0 and [built crosscompiler](https://wiki.osdev.org/GCC_Cross-Compiler))
* ld (tested with 2.30)

To clean the build,
```
make clean
```

## Simulation
Simulation is done with [bochs-2.6](http://bochs.sourceforge.net/). To simulate, run `simulate.sh` script
```
./simulate.sh
```
As machine configuration, `machineconf.txt` is used. Currently memory management code is incomplete and RAM amount is hardcoded. So, code may break if ram amount is reduced.

Virtualbox may also be used to simulate, if host environment is compatible. To turn built disk image into virtualbox compatible harddisk image, run
```
VBoxManage convertfromraw harddisk.bin virtual_disk.vhd --format vhd
```
Create a device according to `machineconf.txt` with storage set as `virtual_disk.vhd`. Then run the created virtual machine.
