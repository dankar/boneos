.section ".text.boot"
_start:
_vector_table:
	b 	reset
	ldr 	pc, undefined_vector
	ldr 	pc, software_interrupt_vector
	ldr	pc, prefetch_abort_vector
	ldr	pc, data_abort_vector
	ldr	pc, not_used_vector
	ldr	pc, irq_vector
	ldr	pc, fiq_vector
undefined_vector:		.word undefined_handler
software_interrupt_vector:	.word software_interrupt_handler
prefetch_abort_vector:		.word prefetch_abort_handler
data_abort_vector:		.word data_abort_handler
not_used_vector:		.word not_used_handler
irq_vector:			.word irq_handler
fiq_vector:			.word fiq_handler
.globl vector_table
vector_table: 			.word _vector_table
.globl kernel_size
kernel_size:			.word _kernel_size
.globl kernel_base
kernel_base:			.word _start

.comm kernel_stack, 0x10000
reset:

	// Disable interrupts for init and set SVC mode
	mrs     r0, cpsr
        and     r1, r0, #0x1f           @ mask mode bits
        teq     r1, #0x1a               @ test for HYP mode
        bicne   r0, r0, #0x1f           @ clear all mode bits
        orrne   r0, r0, #0x13           @ set SVC mode
        orr     r0, r0, #0xc0           @ disable FIQ and IRQ
        msr     cpsr,r0
	ldr 	sp, =kernel_stack+0x10000 	@ Setup stack

	// Call kernel_main
	ldr	r0, =0x12345678
	ldr	r1, =kernel_main
	ldr	r3, =kernel_main
	blx	r3
halt:
	b halt


.comm irq_stack, 0x10000
.comm abort_stack, 0x10000

undefined_handler:
	stmfd   sp!, {r0-r12, lr}
	mov	r0, lr
	ldr	r3, =cundefined_interrupt_handler
	blx	r3
	ldmfd   sp!, {r0-r12, pc}^

software_interrupt_handler:
	stmfd	sp!, {r0-r12, lr}
	ldr	r0, [lr, #-4]

	bic 	r0, r0, #0xff000000

	ldr 	r3, =csoftware_interrupt_handler
	blx 	r3

	ldmfd	sp!, {r0-r12, pc}^


irq_handler:
	ldr sp, =irq_stack+0x10000
        stmfd   sp!, {r0-r12, lr}
	mov	r0, lr
	ldr 	r3, =cirq_handler
	blx 	r3
        ldmfd   sp!, {r0-r12, lr}
	subs	pc, lr, #4

fiq_handler:
        sub lr, lr, #4
        stmfd   sp!, {r0-r12, lr}
	ldr	r3, =cfiq_handler
	blx	r3
        ldmfd   sp!, {r0-r12, pc}

prefetch_abort_handler:
	sub lr, lr, #4
        stmfd   sp!, {r0-r12, lr}
	ldr	r3, =cprefetch_abort_handler
        ldmfd   sp!, {r0-r12, pc}^

data_abort_handler:
	ldr sp, =abort_stack+0x10000
	stmfd   sp!, {r0-r12, lr}
	mov	r0, sp
	ldr	r3, =cdata_abort_handler
	blx	r3
        ldmfd   sp!, {r0-r12, lr}
	subs	pc, lr, #8

not_used_handler:

