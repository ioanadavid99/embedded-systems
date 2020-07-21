ENTRY(Reset_Handler)

MEMORY
{
	FLASH(rx):ORIGIN =0x08000000,LENGTH =1024K
	SRAM(rwx):ORIGIN =0x20000000,LENGTH =128K
}

__max_heap_size = 0x400;	/* not a variable - symbol declaration */ 
__max_stack_size = 0x200;

SECTIONS
{ 
	.text :
	{ 
		*(.isr_vector)
		*(.text)
		*(.rodata)
		_etext = .;	/* this symbol will hold the address of the end of the text section - can be accessed in C */ 
	}> FLASH
	
	.data : 
	{ 
		_sdata = .;
		*(.data)
		. = ALIGN(4);
		_edata = .; 	
	}> SRAM AT> FLASH

	.bss :
	{
		_sbss = .; 	/* location counter */ 
		*(.bss)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .; 
	}> SRAM
} 

