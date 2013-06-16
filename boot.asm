
boot.bin:     file format elf32-littlearm


Disassembly of section .text:

80200000 <_start>:
80200000:	ea00000d 	b	8020003c <reset>
80200004:	e59ff014 	ldr	pc, [pc, #20]	; 80200020 <undefined_vector>
80200008:	e59ff014 	ldr	pc, [pc, #20]	; 80200024 <software_interrupt_vector>
8020000c:	e59ff014 	ldr	pc, [pc, #20]	; 80200028 <prefetch_abort_vector>
80200010:	e59ff014 	ldr	pc, [pc, #20]	; 8020002c <data_abort_vector>
80200014:	e59ff014 	ldr	pc, [pc, #20]	; 80200030 <not_used_vector>
80200018:	e59ff014 	ldr	pc, [pc, #20]	; 80200034 <irq_vector>
8020001c:	e59ff014 	ldr	pc, [pc, #20]	; 80200038 <fiq_vector>

80200020 <undefined_vector>:
80200020:	80200074 	.word	0x80200074

80200024 <software_interrupt_vector>:
80200024:	8020007c 	.word	0x8020007c

80200028 <prefetch_abort_vector>:
80200028:	802000a4 	.word	0x802000a4

8020002c <data_abort_vector>:
8020002c:	802000b0 	.word	0x802000b0

80200030 <not_used_vector>:
80200030:	802000bc 	.word	0x802000bc

80200034 <irq_vector>:
80200034:	8020008c 	.word	0x8020008c

80200038 <fiq_vector>:
80200038:	80200098 	.word	0x80200098

8020003c <reset>:
8020003c:	e59fd078 	ldr	sp, [pc, #120]	; 802000bc <not_used_handler>
80200040:	e59f4078 	ldr	r4, [pc, #120]	; 802000c0 <not_used_handler+0x4>
80200044:	e59f9078 	ldr	r9, [pc, #120]	; 802000c4 <not_used_handler+0x8>
80200048:	e3a05000 	mov	r5, #0
8020004c:	e3a06000 	mov	r6, #0
80200050:	e3a07000 	mov	r7, #0
80200054:	e3a08000 	mov	r8, #0
80200058:	ea00001c 	b	802000d0 <__*ABS*0x2_veneer>
8020005c:	e8a401e0 	stmia	r4!, {r5, r6, r7, r8}
80200060:	e1540009 	cmp	r4, r9
80200064:	3afffffc 	bcc	8020005c <reset+0x20>
80200068:	e59f3058 	ldr	r3, [pc, #88]	; 802000c8 <not_used_handler+0xc>
8020006c:	e12fff33 	blx	r3

80200070 <halt>:
80200070:	eafffffe 	b	80200070 <halt>

80200074 <undefined_handler>:
80200074:	e92d5fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
80200078:	e8fd9fff 	ldm	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, pc}^

8020007c <software_interrupt_handler>:
8020007c:	e92d5fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
80200080:	e51e0004 	ldr	r0, [lr, #-4]
80200084:	e3c004ff 	bic	r0, r0, #-16777216	; 0xff000000
80200088:	e8fd9fff 	ldm	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, pc}^

8020008c <irq_handler>:
8020008c:	e24ee004 	sub	lr, lr, #4
80200090:	e92d5fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
80200094:	e8bd9fff 	pop	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, pc}

80200098 <fiq_handler>:
80200098:	e24ee004 	sub	lr, lr, #4
8020009c:	e92d5fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
802000a0:	e8bd9fff 	pop	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, pc}

802000a4 <prefetch_abort_handler>:
802000a4:	e24ee004 	sub	lr, lr, #4
802000a8:	e92d5fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
802000ac:	e8fd9fff 	ldm	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, pc}^

802000b0 <data_abort_handler>:
802000b0:	e24ee008 	sub	lr, lr, #8
802000b4:	e92d5fff 	push	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
802000b8:	e8bd9fff 	pop	{r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, pc}

802000bc <not_used_handler>:
802000bc:	80221000 	.word	0x80221000
802000c0:	80201000 	.word	0x80201000
802000c4:	80221000 	.word	0x80221000
802000c8:	802000cc 	.word	0x802000cc

802000cc <kernel_main>:
802000cc:	e12fff1e 	bx	lr

802000d0 <__*ABS*0x2_veneer>:
802000d0:	e51ff004 	ldr	pc, [pc, #-4]	; 802000d4 <__*ABS*0x2_veneer+0x4>
802000d4:	00000002 	.word	0x00000002
