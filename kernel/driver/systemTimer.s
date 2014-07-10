.globl wait
.globl getTimerAddr
.globl getTimeStamp

/*
 * r0 contains 32 lower bits of waiting time
 * r1 contains 32 Higher bits of waiting time
 */

getTimerAddr:
ldr r0, =0x20003000
mov pc, lr


getTimeStamp:
push {lr}
bl getTimerAddr
ldrd r0, r1, [r0, #4]
pop {pc}


wait:
value .req r0
delay .req r2
mov delay, r0
push {lr}

bl getTimeStamp
start .req r3
mov start, value


waitLoop$:
	bl getTimeStamp
	elapsed .req r1
	sub elapsed, value, start
	cmp elapsed, delay
	.unreq elapsed
	bls waitLoop$

.unreq value
.unreq delay
.unreq start

pop {pc}

