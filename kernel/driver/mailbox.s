.globl getMailboxAddr

getMailboxAddr:
	ldr r0, =0x2000B880
	mov pc, lr


/*
 * r0 : message to send
 * r1 : mailbox to send
*/
.globl mailboxWrite
mailboxWrite:
	msg .req r0
	mlbx .req r1

	tst msg, #0b1111
	movne pc, lr
	cmp mlbx, #7
	movhi pc, lr

	push {lr}

	mov r2, msg
	.unreq msg
	msg .req r2

	mlad .req r0
	bl getMailboxAddr

	statusWrite$:
		status .req r3
		ldr status, [mlad, #0x18]
		tst status, #0x80000000
		bne statusWrite$
		.unreq status

	/* Status ok */
	orr msg, mlbx
	.unreq msg
	.unreq mlbx
	fullmsg .req r2

	str fullmsg, [mlad, #0x20]
	.unreq fullmsg
	.unreq mlad

	pop {pc}

.globl mailboxRead
mailboxRead:
	cmp r0, #15
	movhi pc, lr

	push {lr}

	mlbx .req r2
	mov mlbx, r0
	mlad .req r0
	bl getMailboxAddr

	statusRead$:
	status .req r1
	ldr status, [mlad, #0x18]
	tst status, #0x40000000
	bne statusRead$
	.unreq status

	/* Status ok */
	fullmsg .req r1
	ldr fullmsg, [mlad]
	and r3, fullmsg, #0b1111
	teq r3, mlbx
	bne statusRead$
	.unreq mlbx
	.unreq mlad

	and r0, fullmsg, #0xfffffff0
	.unreq fullmsg
	pop {pc}
