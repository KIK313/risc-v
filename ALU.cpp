#include"ALU.h"
int alunit::add(int x, int y) {
    return x + y;
}
int alunit::sub(int x, int y) {
    return x - y;
}
int alunit::AND(int x, int y) {
    return x & y;
}
int alunit::OR(int x, int y) {
    return x | y;
}
int alunit::XOR(int x, int y) {
    return x ^ y;
}
int alunit::LT(int x, int y) {
    return x < y;
}
int alunit::LTU(int x, int y) {
    unsigned int u = x; unsigned int v = y;
    return u < v;
}
int alunit::SRL(int x, int y) {
    // return (x >> y) & ((1 << (32 - y)) - 1);
    return static_cast<unsigned>(x) >> (y & 0b11111);
}
int alunit::eq(int x, int y) {
    return (x == y);
}
int alunit::SRA(int x, int y) {
    y = y & 31;
    return (x >> y);
}
int alunit::SLL(int x, int y) {
    y = y&31;
    return (x << y);
}