#include"RS.h"
#include "predict.h"
#include"regf.h"
#include"control.h"
#include"lstype.h"
#include"opcode.h"
#include"mem.h"
#include"pc.h"
#include"ROB.h"
reorder_buffer::reorder_buffer() {
    l = r = 1; L = R = 1;
}
void reorder_buffer::get_ready(int id, int val) {
    new_a[id].is_ready = 1; new_a[id].val = val;    
}
void reorder_buffer::getst(int id, int val) {
    new_a[id].dest = val;
}
void reorder_buffer::init(reservation_station* u, control* cc, 
    register_file* w, memory* mm, program_counter* pp, predictor* rp) {
    rss = u; co = cc; rgf = w; memo = mm; pc = pp; pr = rp;
}
bool reorder_buffer::is_full() {
    return (((r & 31) + 1) == l);
}
void reorder_buffer::insert(const rob_entry& o) {
    new_a[r] = o; R = (r & 31) + 1;
}
int reorder_buffer::getpl() {
    return r;
}
void reorder_buffer::clk_work(int cl) {
    if (co->get_state()) {
        clear();
        return;
    }
    // if (cl > 9999500) {
    //     std::cerr<<l<<' '<<r<<" KIKAKAK"<<std::endl;
    //     std::cerr<<a[l].op <<' '<< l << " work it? "<<a[l].is_ready <<std::endl;
    // }
    if (l == r) return;
    if (!a[l].is_ready) return;
    if (a[l].op >= 15 && a[l].op <= 17) {
        if (memo->is_full()) return;
    }
    L = (l & 31) + 1;
    ls_type o;
    switch(a[l].op) {
        case LUI: rgf->commit_tag(a[l].rg_id, l, a[l].val); break;
        case AUIPC: rgf->commit_tag(a[l].rg_id,  l, a[l].val); break;  //wait ALU;
        case JAL: rgf->commit_tag(a[l].rg_id, l, a[l].val); break;
        case JALR: rgf->commit_tag(a[l].rg_id, l, a[l].val); break; //hard
        case BEQ: 
        case BNE:
        case BLT: 
        case BGE: 
        case BLTU: 
        case BGEU:  if (a[l].val) pr->increase(a[l].hs); else pr->decrease(a[l].hs); 
            if (a[l].val != a[l].dest) {pc->get_clear(a[l].an_pc); co->pre_wrong();} break;
        case LB: rgf->commit_tag(a[l].rg_id, l, a[l].val); break;
        case LH: rgf->commit_tag(a[l].rg_id, l, a[l].val); break;
        case LW: rgf->commit_tag(a[l].rg_id, l, a[l].val); break;
        case LBU: rgf->commit_tag(a[l].rg_id, l, a[l].val); break;
        case LHU: rgf->commit_tag(a[l].rg_id, l, a[l].val); break; // a bit hard
        case SB: o.clk = 0; o.pos = a[l].val; o.val = a[l].dest; o.type = 5; memo->push(o); rgf->commit_st(l); break;
        case SH: o.clk = 0; o.pos = a[l].val; o.val = a[l].dest; o.type = 6; memo->push(o); rgf->commit_st(l); break;
        case SW: o.clk = 0; o.pos = a[l].val; o.val = a[l].dest; o.type = 7; memo->push(o); rgf->commit_st(l); break;
        case ADDI: 
        case SLTI:
        case SLTIU:
        case XORI: 
        case ORI: 
        case ANDI: 
        case SLLI: 
        case SRLI: 
        case SRAI: 
        case ADD: 
        case SUB: 
        case SLL: 
        case SLT: 
        case SLTU: 
        case XOR: 
        case SRL: 
        case SRA: 
        case OR: 
        case AND: rgf->commit_tag(a[l].rg_id, l, a[l].val); break;
        case END: co->come_to_end(); break;
    }
   // std:: cerr<< l <<' '<<a[l].val <<" remove it"<<std::endl;
   //if ()
    rss -> remove_tag(l, a[l].val);      
}
void reorder_buffer::clear() {
    L = R = 1;
}
std::pair<bool, int> reorder_buffer::ask(int id) {
    if (a[id].is_ready) return std::make_pair(1,a[id].val);
    return std::make_pair(0,a[id].val);
}
int reorder_buffer::st_ask() {
    if (l == r || (!a[l].is_ready)) return 0;
    return l; 
}
void reorder_buffer::neg_edge() {
    l = L; r = R; int x = l;
    while(x != r) {
        a[x] = new_a[x];
        x = (x & 31) + 1; 
    }
}