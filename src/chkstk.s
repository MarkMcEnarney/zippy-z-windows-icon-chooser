.text
.globl __chkstk
.def __chkstk; .scl 2; .type 32; .endef
__chkstk:
    pushq %rcx
    pushq %rax
    leaq 24(%rsp), %rcx
    cmpq $4096, %rax
    jb .Llast
.Lloop:
    subq $4096, %rcx
    testb %al, (%rcx)
    subq $4096, %rax
    cmpq $4096, %rax
    ja .Lloop
.Llast:
    subq %rax, %rcx
    testb %al, (%rcx)
    popq %rax
    popq %rcx
    retq
