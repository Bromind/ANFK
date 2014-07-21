.section .init
.globl _start


_start:
bl main

.section .text
.globl main
main:

mov sp, #0x8000


mov r0,#1024
mov r1,#768
mov r2,#16
bl initialiseFrameBuffer

teq r0, #0
beq blinkFast

mov r4, r0
bl setGraphicsAddress

mov r0, #0
mov r1, #0
mov r2, #10
mov r3, #10
bl drawLine

bl kernelStart
