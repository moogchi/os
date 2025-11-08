/*
tells linker where to start
*/

ENTRY(_start)

/*
Tells the section of the object files that will be in the final kernel image
*/

SECTIONS
{
    /*
    Where u start yaaa
    */

    . = 2M;

    /*
    Multiboot header
    */
    .text BLOCK(4K): ALIGN(4K){
        *(.multiboot)
        *(.text)
    }
    /*
    Read only data
    */
    .rodata BLOCK(4K): ALIGN(4K){
        *(.rodata)
    }

    /*
    Read and Write data
    */
    .data BLOCK(4K) : ALIGN(4K){
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K){
        *(COMMON)
        *(.bss)
    } 

}