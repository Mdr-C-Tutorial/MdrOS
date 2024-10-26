ENTRY(_start)

SECTIONS
{

	. = 2M;

	.text :
	{
	    code = .; _code = .; __code = .;
		*(.text)
	}

	.rodata ALIGN(0x1000):
	{
		*(.rodata)
	}

	.data ALIGN(0x1000):
	{
	    data = .; _data = .; __data = .;
		*(.data)
	}

	.bss :
	{
	    bss = .; _bss = .; __bss = .;
		*(COMMON)
		*(.bss)
	}

	end = .; _end = .; __end = .;
}