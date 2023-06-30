#ifndef insu
#define insu
#include"opcode.h"
class program_counter;
class reorder_buffer;
class reservation_station;
class register_file;
class control;
class predictor;
class instruction_unit {
private:
    bool stp, new_stp;
    bool* M;
    inst a[32], new_a[32];
    int l, r, L, R;
    reorder_buffer* rob;
    reservation_station* rss;
    register_file* rgf;
    program_counter* pc;
    control* co;
    predictor* pr;
public:
    instruction_unit();
    void init(bool * me, reorder_buffer* u, reservation_station* uu, 
        register_file* rr, control* cc, program_counter* pc, predictor* pr);
    void stop_ins();
    void work_r_clk(int cl);
    void check_src(int& V, int &Q, int rs);
    void work_w_clk(int cl);
    void work_again(int pos);
    void neg_edge();
};
#endif