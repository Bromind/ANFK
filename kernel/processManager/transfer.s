@
@  +----------+  <- process Address
@  |    PC    |
@  +----------+  <- address + #4
@  |    SP    |
@  +----------+  <- address + #8
@  | LR TMP   |
@  +----------+  <- address + #12
@  | GP regs  |
@  |   ...    |
@  |    LR    |  <- address + #48
@  +----------+  <- process stack : address + #52
@  |   ...    |




.text

@ r0 : process to transfer, i.e. pointer to head of stack of new process 
@ r1 : running process, i.e  pointer to head of stack of running process 
.globl transfer
transfer:
@ set next instruction be the link return
	str lr, [r1, #0]
	str lr, [r1, #48]

@ save process State
	mov r2, r0
	mov r0, r1
	bl saveProcessState

@ At this point, we just have to restart the process. We can jump directlty to 
@ that point if we don't need to save the state of the previous process (i.e. if
@ it is deleted.
	mov r0, r2

@ r0 : process to restart : restore configuration & jump
.globl restartProcess
restartProcess:
@restore process State
	ldr sp, [r0, #4]
@ restore GP registers
	ldr r4, [r0, #12]
	ldr r5, [r0, #16]
	ldr r6, [r0, #20]
	ldr r7, [r0, #24]
	ldr r8, [r0, #28]
	ldr r9, [r0, #32]
	ldr r10, [r0, #36]
	ldr r11, [r0, #40]
	ldr r12, [r0, #44]
	ldr r1, [r0, #0]
	ldr lr, [r0, #48]
@return 
	mov pc, r1

@ globl symbol such that we can save processor state from the outside 
@ (for instance before forking).
.globl saveProcessState
@ r0 : process to save
saveProcessState:
	str sp, [r0, #4]
@ save GP Registers
	str r4, [r0, #12]
	str r5, [r0, #16]
	str r6, [r0, #20]
	str r7, [r0, #24]
	str r8, [r0, #28]
	str r9, [r0, #32]
	str r10, [r0, #36]
	str r11, [r0, #40]
	str r12, [r0, #44]
@ goto label return
	mov pc, lr

@ r0 contains processState to save to. lr contains the address to return, i.e.:
@ pc value of where the function is called.
.globl savePC
savePC:
	str lr, [r0, #0]
	str lr, [r0, #48]
	mov pc, lr


@r0 : process to start : 
@ set sp & lr (to deleteProcess, in  lr_tmp at processState initialization)
@ both located at [r0, #4] & [r0, #8]
.globl startProcess
startProcess:
	ldr lr, [r0, #8]
	ldr sp, [r0, #4]
	ldr pc, [r0, #0]
