# Overview

This is a very simple use of the NEON instructions to add large arrays of uint8_t. There are three implementations
of vector addition:
* Scalar loop in C
* Function calls from C to the SIMD runtime functions defined in arm_neon.h
* Aarch64 assembly code calling the NEON instruction using vector registers

The C code is built with optimziation disabled because otherwise the scalar code is optimzied to use the 
SIMD instructions. This is a crude approach but it seems to show the performance benefits quite well.

# Prerequisites

This requires Aarch64. 

# Build

```
$ mkdir build
$ cd build
$ cmake ..
-- The C compiler identification is GNU 7.4.0
-- The CXX compiler identification is GNU 7.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- The ASM compiler identification is GNU
-- Found assembler: /usr/bin/cc
-- Configuring done
-- Generating done
-- Build files have been written to: /home/spiro/GitRepos/arm-linux-assembly/neon/build
$ make
Scanning dependencies of target libs_64
[ 25%] Building ASM object slib_64/CMakeFiles/libs_64.dir/VectorAdd.s.o
[ 50%] Linking ASM static library liblibs_64.a
[ 50%] Built target libs_64
Scanning dependencies of target neon_driver
[ 75%] Building C object driver/CMakeFiles/neon_driver.dir/Main.c.o
[100%] Linking C executable neon_driver
[100%] Built target neon_driver
```

# Run

```
$ driver/neon_driver 
mean elapsed time (scalar) = 40.7212 msec
mean elapsed time (vector) = 6.7207 msec
mean elapsed time (asm) = 4.6842 msec
```


