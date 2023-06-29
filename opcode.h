#ifndef Opcode
#define Opcode
enum optype {
    LUI, AUIPC, JAL, JALR,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LB, LH, LW, LBU, LHU, SB, SH, SW, 
    ADDI, SLTI, SLTIU, 
    XORI, ORI, ANDI,
    SLLI, SRLI, SRAI,
    ADD, SUB, 
    SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, END
};
struct res_entry {
    bool is_busy; bool is_prepared; bool is_waiting;
    optype op;
    int Qj, Qk, Vj, Vk;
    int dest;
    int A;
};
struct rob_entry {
    int rg_id;
    bool is_ready;
    optype op;
    int dest;
    int val;
    int an_pc;    
};
struct inst {
    optype op;
    int rs1, rs2, dest, val;
    int A;
};
#endif