#ifndef RS
#define RS
#include"opcode.h"
class alunit;
class reorder_buffer;
class memory;
class control;
class program_counter;
class reservation_station {
private:
    res_entry a[32], new_a[32];
    int cnt, new_cnt;
    reorder_buffer* RoB;
    alunit* Alu;
    memory* Memo;
    control* co;
    program_counter* pc;
public:
    reservation_station();
    bool is_full();
    void remove_tag(int id, int val);
    void insert(const res_entry& o);
    void init(reorder_buffer* o, alunit* u, memory* w, control* cc, program_counter* pp);
    void ld_ready(int id, int val);
    void commit_st(int tag);
    void work_clk(int cl);
    void clear();
    void neg_edge();
};
#endif 