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

bl kernelStart

loop$:
b loop$
