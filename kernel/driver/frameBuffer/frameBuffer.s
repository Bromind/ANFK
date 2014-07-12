	.arch armv6
	.eabi_attribute 27, 3
	.eabi_attribute 28, 1
	.fpu vfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"frameBuffer.c"
	.global	fbInfo
	.data
	.align	2
	.type	fbInfo, %object
	.size	fbInfo, 40
fbInfo:
	.word	1024
	.word	768
	.word	1024
	.word	768
	.word	0
	.word	24
	.word	0
	.word	0
	.word	0
	.word	0
	.text
	.align	2
	.global	initialiseFrameBuffer
	.type	initialiseFrameBuffer, %function
initialiseFrameBuffer:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	str	r2, [fp, #-16]
	ldr	r3, [fp, #-8]
	cmp	r3, #4096
	bgt	.L2
	ldr	r3, [fp, #-12]
	cmp	r3, #4096
	bgt	.L2
	ldr	r3, [fp, #-16]
	cmp	r3, #32
	ble	.L3
.L2:
	mov	r3, #0
	b	.L4
.L3:
	ldr	r3, .L6
	ldr	r2, [fp, #-8]
	str	r2, [r3, #0]
	ldr	r3, .L6
	ldr	r2, [fp, #-8]
	str	r2, [r3, #8]
	ldr	r3, .L6
	ldr	r2, [fp, #-12]
	str	r2, [r3, #4]
	ldr	r3, .L6
	ldr	r2, [fp, #-12]
	str	r2, [r3, #12]
	ldr	r3, .L6
	ldr	r2, [fp, #-16]
	str	r2, [r3, #20]
	ldr	r3, .L6
	mov	r0, r3
	mov	r1, #1
	bl	mailboxWrite
	mov	r0, #1
	bl	mailboxRead
	mov	r3, r0
	cmp	r3, #0
	bne	.L5
	ldr	r3, .L6
	b	.L4
.L5:
	mov	r3, #0
.L4:
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L7:
	.align	2
.L6:
	.word	fbInfo
	.size	initialiseFrameBuffer, .-initialiseFrameBuffer
	.ident	"GCC: (Debian 4.6.3-14+rpi1) 4.6.3"
	.section	.note.GNU-stack,"",%progbits
