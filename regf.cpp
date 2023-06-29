#include <iostream>
#include <cstdio>
#include "regf.h"
#include"control.h"
register_file::register_file() {
    for (int i = 0; i < 32; ++i) reg[i] = 0, tag[i] = 0; 
    for (int i = 0; i < 32; ++i) new_reg[i] = new_tag[i] = 0;
    stt = new_stt = 0;
}
std::pair<bool,int> register_file::get_src(int id) {
    if (tag[id] == 0) return std::make_pair(1, reg[id]);
    return std::make_pair(0, tag[id]);
}
int register_file::get_ld() const {
    return stt;
} 
void register_file::clear_rf() {
    for (int i = 0; i < 32; ++i) new_tag[i] = 0;
    new_stt = 0;
}
void register_file::add_tag(int id, int tg) {
    if (id == 0) return;
    new_tag[id] = tg;
}
void register_file::modify_st(int id) {
    new_stt = id;
} 
void register_file::commit_st(int sttag) {
    if (stt == sttag) new_stt = 0;
}
void register_file::commit_tag(int rg_id, int tg, int val) {
    if (rg_id == 0) return;
    new_reg[rg_id] = val; if (tag[rg_id] == tg) new_tag[rg_id] = 0;
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
    for (int i = 0; i < 32; ++i) tag[i] = new_tag[i], reg[i] = new_reg[i];
    reg[0] = 0; tag[0] = 0;
    stt = new_stt;
}
int register_file::getans() {
    return reg[10];
}