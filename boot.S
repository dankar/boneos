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

.comm stack, 0x10000
reset:
	ldr 	sp, =stack+0x10000 	@ Setup stack

	// Call kernel_main
	ldr	r0, =0x12345678
	ldr	r1, =kernel_main
	ldr	r3, =kernel_main
	blx	r3
halt:
	b halt


.comm irq_stack, 0x10000

undefined_handler:
	stmfd   sp!, {r0-r12, lr}
	ldmfd   sp!, {r0-r12, pc}^

software_interrupt_handler:
	stmfd	sp!, {r0-r12, lr}
	ldr	r0, [lr, #-4]

	bic 	r0, r0, #0xff000000

	ldmfd	sp!, {r0-r12, pc}^


irq_handler:
	sub lr, lr, #4
        stmfd   sp!, {r0-r12, lr}
        ldmfd   sp!, {r0-r12, pc}

fiq_handler:
        sub lr, lr, #4
        stmfd   sp!, {r0-r12, lr}
        ldmfd   sp!, {r0-r12, pc}

prefetch_abort_handler:
	sub lr, lr, #4
        stmfd   sp!, {r0-r12, lr}
        ldmfd   sp!, {r0-r12, pc}^

data_abort_handler:
	sub lr, lr, #8
	stmfd   sp!, {r0-r12, lr}
        ldmfd   sp!, {r0-r12, pc}

not_used_handler:

