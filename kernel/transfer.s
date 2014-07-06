
.text

@ r0 : process to transfer, i.e. pointer to head of stack of new process 
@ r1 : running process, i.e  pointer to head of stack of running process 
.globl transfer
transfer:
@ save registers to stack
	push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr, sp}

@ set next instruction be the link return
	str lr, [r1]
@ store the stack pointer at the second word of the stack
	str sp, [r1,#4]

@ load new stack pointer
	ldr sp, [r0,#4]
	pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr, sp}

@ load next instruction to be the saved link return
	ldr pc, [r0]


@ r0 : process to launch
.globl launch
launch:
	ldr pc, [r0]
