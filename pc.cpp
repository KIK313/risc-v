#include"pc.h" 
#include<iostream>
program_counter::program_counter() {
    pc = new_pc = 0;
    is_clear = new_is_clear = 0;
    new_is_stop = is_stop = 0;
    is_restart = new_is_restart = 0;
    rest = new_rest = 0;
}       
bool program_counter::get_stop() {
    return is_stop;
}
int program_counter::get_pc() const {
    return pc;
}
void program_counter::move_pc(int x) {
    new_pc = pc + x;
}
void program_counter::stop_pc() {
    new_pc = pc; 
    new_is_stop = 1;
}
bool program_counter::is_rest() {
    return is_restart;
}
void program_counter::work_clk() {
    if (is_clear) {
        new_is_clear = 0;
        new_clpos = 0;
        new_is_stop = 0; new_is_restart = 0;
        new_pc = clpos;
        return;
    }
    if (is_restart) {
        new_pc = rest; new_rest = 0;
        new_is_restart = 0;
        new_is_stop = 0;
        return;
    }
    if (is_stop) {
        new_pc = pc;
        new_is_stop = 1;
    }
}
void program_counter::work_again(int pos) {
    new_is_restart = 1; new_rest = pos;
  //  std::cerr<< pos <<" )){{}}"<<std::endl;
}
void program_counter::init(control* cc) {
    co = cc;
}
void program_counter::get_clear(int x) {
    new_clpos = x; new_is_clear = 1;
}
void program_counter::neg_edge() {
    pc = new_pc;
    is_clear = new_is_clear;
    is_restart = new_is_restart;
    is_stop = new_is_stop;
    rest = new_rest;
    clpos = new_clpos;    
}