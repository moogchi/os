/*multiboot header */
.set MBALIGN,   1 << 0 /*aligns loaded modules*/
/*1<<0 value 1 left shifted by 0*/
.set MEMINFO,   1 << 1 /* memory map starting from 0010 */
/*1<<0 value 1 left shifted by 0*/
.set MBFLAGS,   MBALIGN | MEMINFO /*flag made by or gate*/
.set MAGIC,     0x1BADB002 /* "Magic" Number like it guides bootloader to find the header */
.set CHECKSUM,   -(MAGIC + MBFLAGS) /* checking so that flag + magic should be 0000*/

.section .multiboot
.align 4 # align the 4 byte values
    .long MAGIC
    .long MBFLAGS
    .long CHECKSUM 

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB for stack
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp # move top of the stack to esp

    call kernel_main # call kernel

    cli

1:  hlt
    jmp 1b

.size _start, . - _start
