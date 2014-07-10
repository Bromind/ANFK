.globl getGpioAddress
.globl setGpioFunction
.globl setGpio

getGpioAddress:
	gpioAddr .req r0
	ldr gpioAddr, =0x20200000
	mov pc, lr
	.unreq gpioAddr


/*
 * r0 contains the pin number
 * r1 contains the function number
*/
setGpioFunction:
	pinNum .req r0
	pinFunc .req r1
	cmp pinNum, #53
	cmpls pinFunc, #7
	
	movhi pc, lr
	push {lr}

	mov r2, pinNum
	.unreq pinNum
	pinNum .req r2

	bl getGpioAddress
	gpioAddr .req r0

	functionLoop$:
	cmp pinNum, #9
	subhi pinNum, #10
	addhi gpioAddr, #4
	bhi functionLoop$

	add pinNum, pinNum, lsl #1
	lsl pinFunc, pinNum
	
	mask .req r3
	mov mask, #7
	lsl mask, pinNum
	mvn mask, mask
	.unreq pinNum

	oldFunc .req r2

	ldr oldFunc, [gpioAddr]
	and oldFunc, mask
	.unreq mask
	orr pinFunc, oldFunc
	.unreq oldFunc
	str pinFunc, [gpioAddr]
	.unreq pinFunc
	.unreq gpioAddr

	pop {pc}

/*
 * r0 contains pin Number
 * r1 contains value to put on pin r0
*/
setGpio:
	pinNum .req r0
	pinVal .req r1

	cmp pinNum, #53
	movhi pc, lr
	push {lr}

	mov r2, pinNum
	.unreq pinNum
	pinNum .req r2

	bl getGpioAddress
	gpioAddr .req r0

	pinBank .req r3
	lsr pinBank, pinNum, #5
	lsl pinBank, pinBank, #2
	add gpioAddr, pinBank
	.unreq pinBank

	and pinNum, #31
	setBit .req r3
	mov setBit, #1
	lsl setBit, pinNum
	.unreq pinNum

	teq pinVal, #0
	.unreq pinVal
	streq setBit, [gpioAddr, #40]
	strne setBit, [gpioAddr, #28]
	.unreq setBit
	.unreq gpioAddr
	pop {pc}
