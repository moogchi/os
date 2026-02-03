/* Multiboot2 header with framebuffer support */
.set MB2_MAGIC,     0xe85250d6       /* Multiboot2 magic number */
.set MB2_ARCH,      0                /* i386 protected mode */
.set MB2_HEADER_LENGTH, multiboot2_header_end - multiboot2_header_start
.set MB2_CHECKSUM,  -(MB2_MAGIC + MB2_ARCH + MB2_HEADER_LENGTH)

.section .multiboot
.align 8
multiboot2_header_start:
    .long MB2_MAGIC
    .long MB2_ARCH
    .long MB2_HEADER_LENGTH
    .long MB2_CHECKSUM

    /* Framebuffer tag */
    .align 8
    .short 5                         /* type = framebuffer */
    .short 0                         /* flags */
    .long 20                         /* size */
    .long 1024                       /* width */
    .long 768                        /* height */
    .long 32                         /* depth */

    /* End tag */
    .align 8
    .short 0                         /* type = end */
    .short 0                         /* flags */
    .long 8                          /* size */
multiboot2_header_end: 

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB for stack
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp             /* Set up the stack */
    
    /* Push multiboot info structure pointer */
    push %ebx                        /* EBX contains pointer to multiboot info */
    push %eax                        /* EAX contains multiboot magic */
    
    call kernel_main                 /* Call kernel with multiboot info */

    cli                              /* Disable interrupts */

1:  hlt                              /* Halt CPU */
    jmp 1b                           /* Infinite loop */

.size _start, . - _start
