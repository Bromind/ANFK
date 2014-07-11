.globl initSP


initSP:

/*
* Set the stack point to the value in r0.
*/
	mov sp, r0
	mov pc, lr
