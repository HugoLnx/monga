.data
.text
.globl main
main:
pushl %ebp
movl %esp, %ebp
subl $8, %esp
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $5, %eax
imull $4,%eax
pushl %ebx
pushl %ecx
pushl %edx
pushl %eax
call malloc
addl $4, %esp
popl %edx
popl %ecx
popl %ebx
popl %ecx
movl %eax, (%ecx)
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $0, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
pushl %eax
movl $10, %eax
popl %ecx
movl %eax, (%ecx)
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $1, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
pushl %eax
movl $15, %eax
popl %ecx
movl %eax, (%ecx)
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $2, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
pushl %eax
movl $20, %eax
popl %ecx
movl %eax, (%ecx)
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $3, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
pushl %eax
movl $25, %eax
popl %ecx
movl %eax, (%ecx)
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $4, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
pushl %eax
movl $30, %eax
popl %ecx
movl %eax, (%ecx)
pushl %ebx
pushl %ecx
pushl %edx
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $0, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
movl (%eax), %eax
pushl %eax
.data
l0: .string "array1: %d\n"
.text
movl $l0, %eax
pushl %eax
call printf
addl $8, %esp
popl %edx
popl %ecx
popl %ebx
pushl %ebx
pushl %ecx
pushl %edx
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $1, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
movl (%eax), %eax
pushl %eax
.data
l1: .string "array2: %d\n"
.text
movl $l1, %eax
pushl %eax
call printf
addl $8, %esp
popl %edx
popl %ecx
popl %ebx
pushl %ebx
pushl %ecx
pushl %edx
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $2, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
movl (%eax), %eax
pushl %eax
.data
l2: .string "array3: %d\n"
.text
movl $l2, %eax
pushl %eax
call printf
addl $8, %esp
popl %edx
popl %ecx
popl %ebx
pushl %ebx
pushl %ecx
pushl %edx
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $3, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
movl (%eax), %eax
pushl %eax
.data
l3: .string "array4: %d\n"
.text
movl $l3, %eax
pushl %eax
call printf
addl $8, %esp
popl %edx
popl %ecx
popl %ebx
pushl %ebx
pushl %ecx
pushl %edx
movl %ebp, %eax
addl $-4, %eax
pushl %eax
movl $4, %eax
imull $4,%eax
movl %eax,%ecx
popl %eax
addl %ecx,%eax
movl (%eax), %eax
pushl %eax
.data
l4: .string "array5: %d\n"
.text
movl $l4, %eax
pushl %eax
call printf
addl $8, %esp
popl %edx
popl %ecx
popl %ebx
movl %ebp, %esp
popl %ebp
ret
.end

