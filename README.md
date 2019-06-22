# Overview

This is a collection of ARM assembly self-education projects for Cortex-A based single board computers running 
various forms of Linux. They all make use of gcc and the associated assembler, and tend to use a C main that calls 
the assembly routines, as well as unit tests written in C.

# Platforms 

## Raspberry Pi 3 Model B

Based on a quad-core Arm Cortex-A53 and running the Raspbian 9 (Stretch) operating system.

It is important to understand that, while this is a 64 bit CPU, conforming to the ARMv8-A architecture spec, 
Raspbian is a 32 bit operating system, making the CPU _effectively_ conform to the ARMv7-A architecture spec. 
Supports both the ARM and Thumb instruction sets.

## PINE64 ROCKPro64

The [Rockip RK3399](ihttp://rockchip.wikidot.com/rk3399)-based [ROCKPro64](https://www.pine64.org/rockpro64/)
running [Armbian Bionic](https://www.armbian.com/rockpro64/). This is a rather odd 
[big.LITTLE](https://www.arm.com/why-arm/technologies/big-little) configuration with two Cortex-A72 and 
four Cortex-A53 cores as well as a [Neon](https://developer.arm.com/architectures/instruction-sets/simd-isas/neon) 
co-processor.

Features from `/proc/cpuinfo'
```
fp asimd evtstrm aes pmull sha1 sha2 crc32
```

# Examples

| Example | Location |
|-----------| -------- |
| Ring Buffer | [ring-buffer](ring-buffer) |

# Reading Material
[A Guide to ARM64 / AArch64 Assembly on Linux with Shellcodes and Cryptography](https://modexp.wordpress.com/2018/10/30/arm64-assembly/)


