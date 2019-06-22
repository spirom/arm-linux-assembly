 .thumb

.cpu        cortex-a53
.section    .text

.thumb_func
.global     rb_init
rb_init:
            push  {r2, r3, r4, r5, r6, r7}
            @ set up
            @ full = false
            movs   r7, #0
            ldr   r3, =full
            str   r7, [r3]
            @ [head] = entries ; [tail] = entries
            ldr   r7, =entries
            ldr   r3, =head
            ldr   r4, =tail
            str   r7, [r3]
            str   r7, [r4]
            @ max = entries + 4 * size
            ldr   r3, =max
            mov   r4, #4
            mul   r4, r4, r1
            add   r4, r7, r4
            str   r4, [r3]
            @ return values
            ldr   r2, =head
            str   r2, [r0]      @ first arg is buffer pointer TODO: wrong
            mov   r0, #0        @ return RB_OK
            pop   {r2, r3, r4, r5, r6, r7}
            bx    lr

.thumb_func
.global     rb_get
rb_get:
            push  {r2, r3, r4, r5, r6, r7}
            ldr   r3, =head
            ldr   r5, [r3]
            ldr   r4, =tail
            ldr   r6, [r4]
            @ check for empty
            cmp   r5, r6
            bne   rb_getnotempty   @ head != tail
            ldr   r7, =full
            ldr   r2, [r7]
            mov   r7, #0
            cmp   r2, r7
            bne   rb_getnotempty   @ full == true
            @ it is empty
            mov   r0, #1           @ RB_ERROR
            pop   {r2, r3, r4, r5, r6, r7}
            bx    lr
rb_getnotempty:
            @ put data in output location
            ldr   r0, [r5]
            str   r0, [r1]
            @ increment head
			mov   r0, #4
            add   r5, r5, r0
            str   r5, [r3]
            @ definitely not full once we finish this
            ldr   r7, =full
            mov   r2, #0
            str   r2, [r7]
            @ check for wrap: head == max
            ldr   r7, =max
            ldr   r7, [r7]
            cmp   r5, r7
            bne   rb_getnowrap
            @ wrap: reset head to entries
            ldr   r2, =entries
            str   r2, [r3]
rb_getnowrap:
            mov   r0, #0            @ RB_OK
            pop   {r2, r3, r4, r5, r6, r7}
            bx    lr

.thumb_func
.global     rb_put
rb_put:
            push  {r2, r3, r4, r5, r6, r7}
            @ check for full (1)
            ldr   r7, =full
            ldr   r2, [r7]
            mov   r3, #0
            cmp   r2, r3
            bne   rb_putfull
            @ set up head and tail
            ldr   r3, =head
            ldr   r4, =tail
            ldr   r5, [r3]
            ldr   r6, [r4]
            str   r1, [r6]                        @ store the data at [tail]
            @ increment tail
			mov   r0, #4
            add   r6, r6, r0
            str   r6, [r4]
            @ check for wrap: tail == max
            ldr   r2, =max
            ldr   r2, [r2]
            cmp   r6, r2
            bne   rb_putnowrap
            @ wrap: reset tail to entries
            ldr   r6, =entries
            str   r6, [R4]
rb_putnowrap:
            @ full = true, if appropriate
            cmp   r5, r6
            bne   rb_putok
            mov   r2, #1                     @ full is 1
            str   r2, [r7]
rb_putok:
            @ done
            mov   r0, #0      @ RB_OK
            pop   {r2, r3, r4, r5, r6, r7}
            bx    lr
rb_putfull:
            mov   r0, #1      @ RB_ERROR
            pop   {r2, r3, r4, r5, r6, r7}
            bx    lr

.section    .data
            .balign 4
head:       .skip 4
tail:       .skip 4
max:        .skip 4
full:       .skip 4
entries:    .skip 100

