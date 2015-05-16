Kernel for raspberry
====================

As a student in CS, I try to apply the content of the courses in real projects. 
Thus, the aim of this project is :
 1. To create a concurrent kernel (without interrupt first, with them after).
 2. To think of an event-driven language, and try to develop it.

Nevertheless, the aim is *not* :
 1. To do a full operating system for raspberry

Directories
===========

| Directory | Content |
| ----------| --------|
| /kernel/  | base of kernel, mainly startup |
| /kernel/processManager | all that concern process managing, i.e. transfer functions, etc...|
| /kernel/driver/ | sources for driver (gpio, framebuffer, mailbox, etc...) | 
| /kernel/mem/ | stuff concerning memory allocation|
| /utils/   | maths, dynamic lists, dynamic arrays, etc... | 
