.section .init
.globl _start


_start:
b main

.section .text
.globl main
main:

mov sp, #0x8000

mov r0,#1024
mov r1,#768
mov r2,#16
bl initialiseFrameBuffer


fbInfoAddr .req r4
mov fbInfoAddr,r0

bl setGraphicsAddress

mov r0, #0
mov r1, #0
mov r2, #10
mov r3, #10
bl drawLine

loop$:
b loop$
