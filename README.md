# Overview

This is a collection of ARM assembly example projects developed on a Raspberry Pi 3 Model B (based on the Arm Cortex A53) running 
the Raspbian 9 (Stretch) operating system.

It is important to understand that, while this is a 64 bit CPU, conforming to the ARMv8-A architecture spec, 
Raspbian is a 32 bit operating system, making the CPU _effectively_ conform to the ARMv7-A architecture spec. Since the A 
family of ARM Cortex cores supports both the ARM and Thumb instruction sets, examples of both are provided.

# Examples

| Example | Location |
|-----------| -------- |
| Ring Buffer | [ring-buffer](ring-buffer) |

# Reading Material
[A Guide to ARM64 / AArch64 Assembly on Linux with Shellcodes and Cryptography](https://modexp.wordpress.com/2018/10/30/arm64-assembly/)


