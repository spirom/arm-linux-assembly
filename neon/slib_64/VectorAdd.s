.arch       armv8-a
.cpu        cortex-a53
.section    .text

.global     add_asm
add_asm:
            // x0 is size, x1 is in1, x2 is in2, x3 is out
            mov x5, #0       // counter
loop:
            cmp x0, x5       // comapre counter maybe exit
            beq done
            ldr q16, [x1]    // load vector registers
            ldr q17, [x2]
            add v18.16B, v16.16B, v17.16B
            str q18, [x3]    // store after vector add
            add x1, x1, #16
            add x2, x2, #16
            add x3, x3, #16
            add x5, x5, #1   // increment coutner
            b loop 
done:
            ret

