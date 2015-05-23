#!/bin/sh

ld --no-undefined main.o  driver/frameBuffer/frameBuffer.o driver/drawing/drawing.o driver/mailbox/mailbox.o kernelStart.o driver/gpio/gpio.o driver/timer/systemTimer.o initFrameBuffer.o test.o initSubsystems.o processManager/processManager.o processManager/transfer.o mem/allocationTable.o syscall/*.o fs/fs.o ../utils/linkedList.o ../utils/maths/div.o ../utils/string/string.o mem/freeMemory.o -Map kernel.map -o kernel.elf -T kernel.ld
objcopy kernel.elf -O binary kernel.img
objdump -d kernel.elf > kernel.list 
