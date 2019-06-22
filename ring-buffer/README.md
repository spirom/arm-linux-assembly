
# Overview

This project provides multiple implementations of a ring buffer containing 32 bit values (integers.) Insertion fails when 
the buffer is full. There is a C implementation (in [clib](clib)) in addition to the versions using ARM 
assembly (A32 or ARM in [slib](slib) and Thumb in [slib_thumb](slib_thumb).) 

Unit tests (in [test](test)) are written in C, and they are linked with all 
three implementations to create the binaries `testrbc`, `testrbs` and `testrbs_thumb`. The tests use a common interface 
definition specified in [inc/RingBuffer.h](inc/RingBuffer.h).

# Building and Running the Unit Tests

```
~/rpi-assembly/ring-buffer $ mkdir build ; cd build
~/rpi-assembly/ring-buffer/build $ cmake ..
-- The C compiler identification is GNU 6.3.0
-- The CXX compiler identification is GNU 6.3.0
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
-- Build files have been written to: /home/pi/rpi-assembly/ring-buffer/build
~/rpi-assembly/ring-buffer/build $ make
Scanning dependencies of target librbc
[  8%] Building C object clib/CMakeFiles/librbc.dir/RingBuffer.c.o
[ 16%] Linking C static library liblibrbc.a
[ 16%] Built target librbc
Scanning dependencies of target librbs
[ 25%] Building ASM object slib/CMakeFiles/librbs.dir/RingBuffer.s.o
[ 33%] Linking ASM static library liblibrbs.a
[ 33%] Built target librbs
Scanning dependencies of target librbs_thumb
[ 41%] Building ASM object slib_thumb/CMakeFiles/librbs_thumb.dir/RingBuffer.s.o
[ 50%] Linking ASM static library liblibrbs_thumb.a
[ 50%] Built target librbs_thumb
Scanning dependencies of target testrbc
[ 58%] Building C object test/CMakeFiles/testrbc.dir/TestRB.c.o
[ 66%] Linking C executable testrbc
[ 66%] Built target testrbc
Scanning dependencies of target testrbs
[ 75%] Building C object test/CMakeFiles/testrbs.dir/TestRB.c.o
[ 83%] Linking C executable testrbs
[ 83%] Built target testrbs
Scanning dependencies of target testrbs_thumb
[ 91%] Building C object test/CMakeFiles/testrbs_thumb.dir/TestRB.c.o
[100%] Linking C executable testrbs_thumb
[100%] Built target testrbs_thumb
~/rpi-assembly/ring-buffer/build $ test/testrbc
[1] Empty                                    PASS 
[2] Basic                                    PASS 
[3] Wrap                                     PASS 
[4] Full                                     PASS 
[5] Fill, consume all and refill             PASS 
[6] Fill, consume some and refill            PASS 
[7] Lag                                      PASS 
All 7 tests passed
No tests were skipped
~/rpi-assembly/ring-buffer/build $ test/testrbs
[1] Empty                                    PASS 
[2] Basic                                    PASS 
[3] Wrap                                     PASS 
[4] Full                                     PASS 
[5] Fill, consume all and refill             PASS 
[6] Fill, consume some and refill            PASS 
[7] Lag                                      PASS 
All 7 tests passed
No tests were skipped
~/rpi-assembly/ring-buffer/build $ test/testrbs_thumb
[1] Empty                                    PASS 
[2] Basic                                    PASS 
[3] Wrap                                     PASS 
[4] Full                                     PASS 
[5] Fill, consume all and refill             PASS 
[6] Fill, consume some and refill            PASS 
[7] Lag                                      PASS 
All 7 tests passed
No tests were skipped
~/rpi-assembly/ring-buffer/build $ 

```


# TODO

* improve comments
* make the C API properly opaque
* reject too-large buffer size
* document properly

