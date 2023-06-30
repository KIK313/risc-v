#include"insu.h"
#include"control.h"
#include "opcode.h"
#include"pc.h"
#include"ROB.h"
#include"RS.h"
#include"regf.h"
#include"predict.h"
#include<iostream>
instruction_unit::instruction_unit() {
    l = r = 0; L = R = 0; stp = new_stp = 0;
}
void instruction_unit::init(bool* me, reorder_buffer* u, 
    reservation_station* uu, register_file* rr, control* cc, program_counter* pp, predictor* rp) {
    M = me; rob = u; rss = uu; rgf = rr; co = cc; pc = pp; pr = rp;
}
void instruction_unit::check_src(int &V, int &Q, int rs) {
    std::pair<bool, int> t; std::pair<bool, int> tt;
    t = rgf->get_src(rs);
    if (t.first == 1) {
        Q = 0; V = t.second;
    } else { 
        tt = rob->ask(t.second);
        if (tt.first) {
            Q = 0; V = tt.second;            
        } else Q = t.second;
    }  
    
}
int sext(int x, int k) {
    if ((x >> k) & 1) {
        x = x | (-(1<<k));
    }
    return x;
}
void instruction_unit::stop_ins() {
    stp = 1;
}
void instruction_unit::work_r_clk() {
    if (co->get_state()) return;
    if (pc->is_rest()) return;
    if (pc->get_stop()) return;
    if (((r+1)&31) == l) {pc->move_pc(0); return;}
    int pos = pc->get_pc()*8;
    int yy = 0;
    for (int i = 0; i < 32; ++i) yy |= (M[pos+i] <<i);
    if (yy == 267388179) {
        inst o; o.op = END;
        new_a[r] = o; R = (r + 1) & 31;
        pc->stop_pc(); 
    } else {
    int x = 0;
    for (int i = 0; i < 7; ++i) x |= (M[pos+i] << i);    
    int rs1 = 0; int rs2 = 0; int dest = 0;
    for (int i = 0; i < 5; ++i) dest |= (M[pos+7+i] << i);
    for (int i = 0; i < 5; ++i) rs1 |= (M[pos+15+i] << i);
    for (int i = 0; i < 5; ++i) rs2 |= (M[pos+20+i] << i);
    inst o; int im = 0; o.hs = 0;
    if (x == 55) { // lui
        o.op = LUI;
        for (int i = 12; i < 32; ++i) im |= (M[pos+i] << i);
    }
    if (x == 23) { //auipc
        o.op = AUIPC;
        for (int i = 12; i < 32; ++i) im |= (M[pos+i] << i);
    }
    if (x == 111) { // jal 
        o.op = JAL;
        for (int i = 12; i <= 19; ++i) im |= (M[pos+i] << i);
        im |= (M[pos+20] << 11);
        for (int i = 1; i <= 10; ++i) im |= (M[pos+20+i] <<i);
        im |= (M[pos+31] << 20);
        im = sext(im, 20);
    }
    if (x == 103) { // jalr
        o.op = JALR;
        for (int i = 0; i <= 11; ++i) im |= (M[pos+20+i] <<i);
        im = sext(im, 11);
    }
    if (x == 99) {
        for (int i = 1; i <= 4; ++i) im |= (M[pos+7+i] << i);
        for (int i = 5; i <= 10; ++i) im |= (M[pos+20+i] << i);
        im |= (M[pos+7] << 11);
        im |= (M[pos+31] << 12);
        for (int i = 0; i < 3; ++i) o.hs |= (M[pos+12+i] << i);
        o.hs |= (M[pos+15] << 3); o.hs |= (M[pos+20] << 4);
        o.hs |= (M[pos+7] << 5); o.hs |= (M[pos+8] << 6);  
        int r = 0;
        for (int i = 0 ; i < 3; ++i) r |= M[pos+12+i] << i;
        switch(r) {
            case 0: o.op = BEQ; break;
            case 1: o.op = BNE; break;
            case 4: o.op = BLT; break;
            case 5: o.op = BGE; break;
            case 6: o.op = BLTU; break;
            case 7: o.op = BGEU; break;
        }
        im = sext(im, 12);
    }
    if (x == 3) { // lb lw  
        for (int i = 0; i <= 11; ++i) im |= (M[pos+20+i] << i);
        int r = 0;
        for (int i = 0; i < 3; ++i) r |= M[pos+12+i] << i;
        switch(r) {
            case 0: o.op = LB; break;
            case 1: o.op = LH; break;
            case 2: o.op = LW; break;
            case 4: o.op = LBU; break;
            case 5: o.op = LHU; break;
        }
        im = sext(im, 11);
    }
    if (x == 35) { //  
        for (int i = 0; i <= 4; ++i) im |= (M[pos+7+i] << i);
        for (int i = 5; i <= 11; ++i) im |= (M[pos+20+i] << i);
        int r = 0;
        for (int i = 0; i < 3; ++i) r |= M[pos+12+i] << i;
        switch(r) {
            case 0: o.op = SB; break;
            case 1: o.op = SH; break;
            case 2: o.op = SW; break;
        }
        im = sext(im, 11);
    }
    if (x == 19) {
        for (int i = 0; i <= 11; ++i) im |= (M[pos+i+20] << i);
        int r = 0;
        for (int i = 0; i < 3; ++i) r |= M[pos+12+i] << i;
        switch(r) {
            case 0: o.op = ADDI; break;
            case 1: o.op = SLLI; break;
            case 2: o.op = SLTI; break;
            case 3: o.op = SLTIU; break;
            case 4: o.op = XORI; break;
            case 6: o.op = ORI; break;
            case 7: o.op = ANDI; break;
            case 5: if (M[pos+30]) {im = im & 31; o.op = SRAI;} else o.op = SRLI;  break;
        }
        im = sext(im, 11);
    }
    if (x == 51) {
        int r = 0;
        for (int i = 0; i < 3; ++i) r |= M[pos+12+i] << i;
        switch(r) {
            case 0: if (M[pos+30]) o.op = SUB; else o.op = ADD; break;
            case 1: o.op = SLL; break;
            case 2: o.op = SLT; break;
            case 3: o.op = SLTU; break;
            case 4: o.op = XOR; break;
            case 5: if (M[pos+30]) o.op = SRA; else o.op = SRL; break;
            case 6: o.op = OR; break;
            case 7: o.op = AND; break;
        }
    }
    o.A = im; o.val = pc->get_pc(); o.rs1 = rs1; o.rs2 = rs2; o.dest = dest;
    if (o.op == JAL) pc->move_pc(o.A);
    else if (o.op == JALR) pc->stop_pc();
    else if (o.op >= 4 && o.op <= 9) { 
        if (pr->get_state(o.hs)) {
            o.hs = (o.hs << 1) | 1;
            pc->move_pc(o.A);
            o.val = o.val + 4;
        } else {
            o.hs = o.hs << 1;
            pc->move_pc(4);
            o.val = o.A + o.val;
        }
    }
    else pc->move_pc(4);
    new_a[r] = o; R = (r + 1) & 31; 
    } 
}
void instruction_unit::work_w_clk() {
    if (co->get_state()) {
        L = R = 0;
        return;
    }
    if (l == r) return;
    if (rob->is_full() || rss->is_full()) return;
    int pos = rob->getpl();
    inst o = a[l]; L = (l + 1) & 31;
    rob_entry u; res_entry v;
    v.is_busy = 1; v.is_prepared = 0; v.is_waiting = 0; u.is_ready = 0;
    u.op = o.op; v.op = o.op; v.dest = pos;
    u.an_pc = 0; u.dest = 1; u.val = 0; // u.dest may be linked with ben
    u.rg_id = o.dest; u.hs = (o.hs >> 1); 
    std::pair<bool, int> w;
    switch (o.op) {
        case LUI: v.A = o.A; v.Qj = v.Qk = 0; rgf->add_tag(o.dest, pos); break;
        case AUIPC: v.A = o.A; v.Qj = v.Qk = 0; v.Vj = o.val; rgf->add_tag(o.dest, pos); break;
        case JAL: v.Qj = v.Qk = 0; v.Vj = o.val; rgf->add_tag(o.dest, pos); break;
        case JALR: v.A = o.A; v.Qj = 0; v.Vj = o.val; rgf->add_tag(o.dest, pos);
            check_src(v.Vk, v.Qk, o.rs1);
            break; 
        case BEQ: // this needs to be changed
        case BNE:
        case BLT:
        case BGE:
        case BLTU:
        case BGEU: u.an_pc = o.val; u.dest = o.hs & 1; check_src(v.Vj, v.Qj, o.rs1); check_src(v.Vk, v.Qk, o.rs2); break;
        case LB: 
        case LH: 
        case LW: 
        case LBU:
        case LHU: rgf->add_tag(o.dest, pos); v.A = o.A; v.Qk = rgf->get_ld(); 
            if (v.Qk != 0) {
                if (v.Qk == rob->st_ask()) v.Qk = 0;
            }
         check_src(v.Vj, v.Qj, o.rs1);
             break; 
        case SB:
        case SH: 
        case SW: rgf->modify_st(pos); v.A = o.A; 
            check_src(v.Vj, v.Qj, o.rs1); check_src(v.Vk, v.Qk, o.rs2); break;
        case ADDI:
        case SLTI: 
        case SLTIU: 
        case XORI:
        case ORI:
        case ANDI:
        case SLLI:
        case SRLI:  
        case SRAI: rgf->add_tag(o.dest, pos); v.A = o.A; v.Qk = 0; check_src(v.Vj, v.Qj, o.rs1); break;
        case ADD: 
        case SUB: 
        case SLL: 
        case SLT: 
        case SLTU:  
        case XOR: 
        case SRL:  
        case SRA: 
        case OR:  
        case AND: rgf->add_tag(o.dest, pos); check_src(v.Vj,v.Qj,o.rs1); check_src(v.Vk,v.Qk,o.rs2); break;
        case END: v.Qj = v.Qk = 0; break;
    }
    rob->insert(u); rss->insert(v);
}
void instruction_unit::neg_edge() {
    l = L; r = R; stp = new_stp;
    int x = l;
    while (x != r) {
        a[x] = new_a[x];
        x = (x + 1) & 31;
    }    
}