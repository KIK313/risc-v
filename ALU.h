#ifndef ALU
#define ALU
class alunit{
private:

public:
    int add(int x, int y);
    int sub(int x, int y);
    int AND(int x, int y);
    int OR(int x, int y);
    int XOR(int x, int y);
    int LT(int x, int y);
    int LTU(int x, int y);
    int SRL(int x, int y);
    int eq(int x, int y);
    int SRA(int x, int y);
    int SLL(int x, int y);
};
#endif