#include <iostream>
#include <cstdio>
#include "regf.h"
#include"control.h"
register_file::register_file() {
    for (int i = 0; i < 32; ++i) {
        reg[i] = new_reg[i] = 0;
        pos[i] = new_pos[i] = 0;
        tag[i][0] = tag[i][1] = new_tag[i][0] = new_tag[i][1] = 1;
    }
    stt[0] = stt[1] = new_stt[0] = new_stt[1] = 0; ps = new_ps = 0;
}
std::pair<bool,int> register_file::get_src(int id) {
    if (tag[id][pos[id]] == 0) return std::make_pair(1, reg[id]);
    return std::make_pair(0, tag[id][pos[id]]);
}
int register_file::get_ld() const {
    return stt[ps];
} 
void register_file::clear_rf() {
    for (int i = 0; i < 32; ++i) new_tag[i][0] = new_tag[i][1] = 0, new_pos[i] = 0;
    new_stt[0] = new_stt[1] = 0; new_ps = 0;
}
void register_file::add_tag(int id, int tg) {
    if (id == 0) return;
    new_tag[id][pos[id] ^ 1] = tg;
    new_pos[id] = pos[id] ^ 1;
}
void register_file::modify_st(int id) {
    new_stt[ps^1] = id;
    new_ps = ps ^ 1;
} 
void register_file::commit_st(int sttag) {
    if (stt[ps] == sttag) new_stt[ps] = 0;
}
void register_file::commit_tag(int rg_id, int tg, int val) {
    if (rg_id == 0) return;
    new_reg[rg_id] = val; 
    if (tag[rg_id][pos[rg_id]] == tg) new_tag[rg_id][pos[rg_id]] = 0;
}
void register_file::init(control *cc) {
    co = cc;
}
void register_file::work_clk() {
    if (co->get_state()) {
        clear_rf();
    }
}
void register_file::neg_edge() {
    for (int i = 0; i < 32; ++i) {
        tag[i][0] = new_tag[i][0]; 
        tag[i][1] = new_tag[i][1];
        reg[i] = new_reg[i];
        pos[i] = new_pos[i];
    } 
    reg[0] = 0; tag[0][0] = tag[0][1] = 0;
    stt[0] = new_stt[0]; stt[1] = new_stt[1];
    ps = new_ps;
}
int register_file::getans() {
    return reg[10];
}