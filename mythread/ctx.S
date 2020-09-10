/*
    ctx.S:  x86-64
    ctx_buf[0]: rbx
    ctx_buf[1]: rsp
    ctx_buf[2]: rbp
    ctx_buf[3]: r12
    ctx_buf[4]: r13
    ctx_buf[5]: r14
    ctx_buf[6]: r15
    ctx_buf[7]: Program Counter
*/
#if defined(__amd64__) || defined(__x86_64__)
    
/*save_context*/
    .text
    .align 4
    .globl save_context
    .type save_context, @function

/*rdi保存参数：from线程上下文buffer*/
save_context:
    pop %rsi                /*这里为什么要pop rsi？在x86-64的函数调用与栈帧原理中，
                            调用者call指令会将返回地址压栈，这里我们想要保存的是调用者原本的栈信息，
                            即esp应该是将返回地址压栈前的esp。这里先把返回地址pop出即可得到原本的esp*/
    xorl %eax, %eax         /*设置返回值为0*/
    movq %rbx, (%rdi)
    movq %rsp, 8(%rdi)
    push %rsi               /*上一条指令已经将调用者原esp保存，现将rsi入栈，使得函数能够正常返回*/
    movq %rbp, 16(%rdi)
    movq %r12, 24(%rdi)
    movq %r13, 32(%rdi)
    movq %r14, 40(%rdi)
    movq %r15, 48(%rdi)
    movq %rsi, 56(%rdi)     /*线程恢复执行的地址，即save_context调用的下一条地址*/
    ret                     /*ret指令会跳转到返回地址处，也是save_context调用的下一条地址*/

/*restore_context*/
    .text
    .align 4
    .globl restore_context
    .type restore_context, @function

restore_context:
    movl $1, %eax           /*设置返回值为1*/
    movq (%rdi), %rbx       /*恢复栈帧*/
    movq 8(%rdi), %rsp
    movq 16(%rdi), %rbp
    movq 24(%rdi), %r12
    movq 32(%rdi), %r13
    movq 40(%rdi), %r14
    movq 48(%rdi), %r15
    jmp *56(%rdi)           /*恢复执行*/

/*start_context*/
    .text
    .align 4
    .globl start_context
    .type start_context, @function

start_context:
    movq (%r10), %rbx       /*恢复栈帧*/
    movq 8(%r10), %rsp
    movq 16(%r10), %rbp
    movq 24(%r10), %r12
    movq 32(%r10), %r13
    movq 40(%r10), %r14
    movq 48(%r10), %r15
    jmp *56(%r10)           /*恢复执行*/
    

#else
#error "Linux cpu arch not supported"
#endif