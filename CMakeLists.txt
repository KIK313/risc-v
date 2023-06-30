cmake_minimum_required(VERSION 3.9)
project(risc-v)

add_executable(code ALU.cpp ALU.h control.cpp control.h cpu-sim.cpp insu.cpp insu.h lstype.h mem.cpp mem.h opcode.h pc.cpp pc.h regf.cpp regf.h ROB.cpp ROB.h RS.cpp RS.h predict.h)
