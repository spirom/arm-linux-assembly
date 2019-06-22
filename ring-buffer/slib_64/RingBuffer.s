.arch       armv8-a
.cpu        cortex-a53
.section    .text

.global     rb_init
rb_init:
            // set up
            // full = false
            mov   w2, #0
            adr   x3, full
            str   w2, [x3]
            // [head]  entries ; [tail] = entries
            adr   x2, entries
            adr   x3, head
            adr   x4, tail
            str   x2, [x3]
            str   x2, [x4]
            // max = entries + 4 * size
            adr   x3, max
            mov   x4, #4
            mul   x4, x1, x4        // size arg
            add   x4, x2, x4        // entries set above
            str   x4, [x3]
            // return values
            adr   x2, head
            str   x2, [x0]          // first arg is buffer pointer TODO: wrong
            mov   w0, #0            // return RB_OK
            ret

.global     rb_get
rb_get:
            // set up head and tail
            adr   x3, head
            ldr   x5, [x3]
            adr   x4, tail
            ldr   x6, [x4]
            // check for empty
            cmp   x5, x6
            bne   rb_getnotempty    // head != tail
            adr   x7, full
            ldr   w2, [x7]
            mov   w8, #0
            cmp   w2, w8
            bne   rb_getnotempty    // full == true
            // it is empty
            mov   w0, #1            // RB_ERROR
            ret
rb_getnotempty:
            // put data in output location
            ldr   w0, [x5]
            str   w0, [x1]
            // increment head
            add   x5, x5, #4
            str   x5, [x3]
            // definitely not full after this is done
            adr   x7, full
            mov   w2, #0
            str   w2, [x7]
            // check for wrap: head == max
            adr   x8, max
            ldr   x8, [x8]
            cmp   x5, x8
            bne   rb_getnowrap
            // wrap: reset head to entries
            adr   x2, entries
            str   x2, [x3]
rb_getnowrap:
            mov   w0, #0            // RB_OK
            ret

.global     rb_put
rb_put:
            // check for full (-1)
            adr   x7, full
            ldr   w2, [x7]
            mov   w3, #0
            cmp   w2, w3
            bne   rb_putfull
            // set up head and tail
            adr   x3, head
            adr   x4, tail
            ldr   x5, [x3]
            ldr   x6, [x4]
            str   w1, [x6]          // store the data at [tail]
            // increment tail
            add   x6, x6, #4
            str   x6, [x4]
            // check for wrap: tail == max
            adr   x8, max
            ldr   x8, [x8]
            cmp   x6, x8
            bne   rb_putnowrap
            // wrap: reset tail to entries
            adr   x6, entries
            str   x6, [x4]
rb_putnowrap:
            // full = true, if appropriate
            cmp   x5, x6
            bne   rb_putok
            mov   w2, #-1           // full is -1
            str   w2, [x7]
rb_putok:
            // done
            mov   w0, #0            // RB_OK
            ret
rb_putfull:
            mov   w0, #1            // RB_ERROR
            ret

.section    .data
            .balign 4
head:       .skip 8
tail:       .skip 8
max:        .skip 8 // points just past the end of entries array
full:       .skip 4
entries:    .skip 100

