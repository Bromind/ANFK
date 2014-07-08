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

@ restart process 
	mov r0, r2
	bl restartProcess

@ r0 : process to save
.globl saveProcessState
saveProcessState:
	str sp, [r0, #4]
	str lr, [r0, #8] @ space for tmp lr storage
	bl saveGPRegister
	ldr pc, [r0, #8] @ goto label return

@ r0 : process to Save
.globl saveGPRegister
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
.globl restoreProcessState
restoreProcessState:
	ldr sp, [r0, #4]
	str lr, [r0, #8]
	bl restoreGPRegister
	ldr pc, [r0, #8]

@ r0 : process to restore
.globl restoreGPRegister
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

@ r0 : process to restart
.globl restartProcess
restartProcess:
	bl restoreProcessState
	ldr pc, [r0, #0]

@r0 : process to start
.globl startProcess
startProcess:
	ldr pc, [r0, #0]
