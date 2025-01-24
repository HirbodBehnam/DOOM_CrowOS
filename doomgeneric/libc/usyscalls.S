#include "syscall.h"
.section .text
.intel_syntax noprefix
.globl read
.type read, @function
read:
  mov rax, SYSCALL_READ
  syscall
  ret
.globl write
.type write, @function
write:
  mov rax, SYSCALL_WRITE
  syscall
  ret
.globl open
.type open, @function
open:
  mov rax, SYSCALL_OPEN
  syscall
  ret
.globl close
.type close, @function
close:
  mov rax, SYSCALL_CLOSE
  syscall
  ret
.globl sbrk
.type sbrk, @function
sbrk:
  mov rax, SYSCALL_SBRK
  syscall
  ret
.globl exec
.type exec, @function
exec:
  mov rax, SYSCALL_EXEC
  syscall
  ret
.globl exit
.type exit, @function
exit:
  mov rax, SYSCALL_EXIT
  syscall
  ret
.globl wait
.type wait, @function
wait:
  mov rax, SYSCALL_WAIT
  syscall
  ret
.globl lseek
.type lseek, @function
lseek:
  mov rax, SYSCALL_LSEEK
  syscall
  ret
.globl time
.type time, @function
time:
  mov rax, SYSCALL_TIME
  syscall
  ret
.globl sleep
.type sleep, @function
sleep:
  mov rax, SYSCALL_SLEEP
  syscall
  ret
.globl ioctl
.type ioctl, @function
ioctl:
  mov rax, SYSCALL_IOCTL
  syscall
  ret
.globl rename
.type rename, @function
rename:
  mov rax, SYSCALL_RENAME
  syscall
  ret
.globl unlink
.type unlink, @function
unlink:
  mov rax, SYSCALL_UNLINK
  syscall
  ret
.globl mkdir
.type mkdir, @function
mkdir:
  mov rax, SYSCALL_MKDIR
  syscall
  ret
.section .note.GNU-stack
