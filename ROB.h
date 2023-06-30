#ifndef ROB
#define ROB
#include"opcode.h"
#include<iostream>
class reservation_station;
class control;
class register_file;
class memory;
class predictor;
class program_counter;
class reorder_buffer {
private:
    int l,r,L,R;
    rob_entry a[33], new_a[33]; 
    reservation_station* rss;
    control* co;
    register_file* rgf;
    program_counter* pc;
    memory* memo;
    predictor* pr;
public:
    reorder_buffer();
    int st_ask();
    void get_ready(int id, int val);
    void init(reservation_station* u, control* cc, 
    register_file* w, memory* mm, program_counter* pp, predictor* rp);
    void getst(int id, int val);
    std::pair<bool, int> ask(int id);
    bool is_full();
    void insert(const rob_entry& o);
    void clear();
    void clk_work();
    void neg_edge();
    int getpl();
};
#endif