#include"control.h"
#include"mem.h"
#include<iostream>
control::control() {
    new_state = state = 0;
    cte = new_cte = 0;
    is_end = new_is_end = 0;    
}
bool control::get_state() {
    return state;
}
void control::init(memory* M) {
    memo = M;
}
bool control::ed() {return is_end;}
void control::work_clk() {
    if (state) new_state = 0;
    if (cte) {
       // std::cerr<<"OoO"<<std::endl; exit(0);
        new_cte = 1;
        if (memo->is_empty()) new_is_end = 1;
    }
    if (is_end) new_is_end = 1;
}
void control::neg_edge() {
    state = new_state;
    cte = new_cte;
    is_end = new_is_end;
}
void control::come_to_end() {
  //  std::cerr<<" OKOKO"<<std::endl;
    new_cte = 1;
}
void control::pre_wrong() {
    new_state = 1;
}