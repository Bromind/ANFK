.section .data

.align 1
foreColour:
.hword 0xFFFF

.align 2
graphicsAddress:
.int 0

.section .text
.globl setForeColour
setForeColour:
cmp r0, #0x10000
movhs pc, lr

ldr r1, =foreColour
strh r0, [r1]
mov pc, lr

.globl setGraphicsAddress
setGraphicsAddress:
ldr r1, =graphicsAddress
str r0, [r1]
mov pc, lr



.globl drawPixel
drawPixel:
px .req r0
py .req r1
address .req r2

ldr address,=graphicsAddress
ldr address, [address]

height .req r3
ldr height, [address,#4]
sub height, #1
cmp py, height
movhi pc, lr
.unreq height

width .req r3
ldr width, [address,#0]
sub width, #1
cmp px, width
movhi pc, lr

pixelAddr .req r3
add width, #1
mla pixelAddr, py, width, px
.unreq width
.unreq px
.unreq py
ldr address, [address,#32]
add address, pixelAddr,lsl #1
.unreq pixelAddr

fore .req r3
ldr fore, =foreColour
ldrh fore, [fore]

strh fore, [address]
.unreq fore
.unreq address
mov pc, lr

.globl drawLine
drawLine:
push {r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}

x0 .req r4
y0 .req r5
x1 .req r6
y1 .req r7

deltaX .req r8
deltaYNeg .req r9

stepX .req r10
stepY .req r11

error .req r12

mov x0, r0
mov y0, r1
mov x1, r2
mov y1, r3

cmp x0, x1
subgt deltaX, x0, x1
movgt stepX, #-1
suble deltaX, x1, x0
movle stepX, #1

cmp y0, y1
subgt deltaYNeg, y1, y0
movgt stepY, #-1
suble deltaYNeg, y0, y1
movgt stepY, #1

add error, deltaX, deltaYNeg
add x1, x1, stepX
add y1, y1, stepY

pixelLoop$:

teq x0, x1
teqne y0, y1
popeq {r4, r5, r6, r7, r8, r9, r10, r11, r12, pc}

mov r0, x0
mov r1, y0
bl drawPixel

add r0, error, error
cmp r0, deltaYNeg
addge x0, x0, stepX
addge error, error, deltaYNeg

cmp r0, deltaX
addle y0, y0, stepY
addle error, error, deltaX

bl pixelLoop$
