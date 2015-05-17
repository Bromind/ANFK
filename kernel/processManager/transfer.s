@
@  +----------+  <- process Address
@  |    PC    |
@  +----------+  <- address + #4
@  |    SP    |
@  +----------+  <- address + #8
@  |    LR    |
@  +----------+  <- address + #12
@  | GP regs  |
@  |   ...    |
@  +----------+  <- process stack : address + #0x30
@  |   ...    |




.text

@ r0 : process to transfer, i.e. pointer to head of stack of new process 
@ r1 : running process, i.e  pointer to head of stack of running process 
.globl transfer
transfer:
@ set next instruction be the link return
	str lr, [r1, #0]

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
	bl restoreProcessState
	ldr lr, [r0, #0]
	mov pc, lr

@ globl symbol such that we can save processor state from the outside 
@ (for instance before forking).
.globl saveProcessState
@ r0 : process to save
saveProcessState:
	str sp, [r0, #4]
	str lr, [r0, #8] @ space for tmp lr storage
	bl saveGPRegister
	ldr lr, [r0, #8]
	mov pc, lr @ goto label return

@ r0 : process to Save
saveGPRegister:
	str r4, [r0, #12]
	str r5, [r0, #16]
	str r6, [r0, #20]
	str r7, [r0, #24]
	str r8, [r0, #28]
	str r9, [r0, #32]
	str r10, [r0, #36]
	str r11, [r0, #40]
	str r12, [r0, #44]
	mov pc, lr

@ r0 : process to restore
restoreProcessState:
	ldr sp, [r0, #4]
	str lr, [r0, #8]
	bl restoreGPRegister
	ldr pc, [r0, #8]

@ r0 : process to restore
restoreGPRegister:
	ldr r4, [r0, #12]
	ldr r5, [r0, #16]
	ldr r6, [r0, #20]
	ldr r7, [r0, #24]
	ldr r8, [r0, #28]
	ldr r9, [r0, #32]
	ldr r10, [r0, #36]
	ldr r11, [r0, #40]
	ldr r12, [r0, #44]
	mov pc, lr

@ r0 : process to stop
.globl stopRunningProcess
stopRunningProcess:
	push {lr}
	str lr, [r0, #0]
	bl saveProcessState
	pop {pc}

@r0 : process to start : 
@ set sp & lr (to deleteProcess, in  lr_tmp at processState initialization)
@ both located at [r0, #4] & [r0, #8]
.globl startProcess
startProcess:
	ldr lr, [r0, #8]
	ldr sp, [r0, #4]
	ldr pc, [r0, #0]
