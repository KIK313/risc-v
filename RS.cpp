#include"RS.h"
#include"ALU.h"
#include"ROB.h"
#include"mem.h"
#include"lstype.h"
#include"opcode.h"
#include"pc.h"
#include"control.h"
reservation_station::reservation_station() { 
    cnt = new_cnt = 0;
    for (int i = 0; i < 32; ++i) {   
        a[i].is_busy = new_a[i].is_busy = 0;
        a[i].Qj = a[i].Qk = new_a[i].Qj = new_a[i].Qk = 0;
    }
}
bool reservation_station::is_full() {
    return (cnt == 32);
}
void reservation_station::insert(const res_entry& o) {
    for (int i = 0; i < 32; ++i) { 
        if (a[i].is_busy) continue;
        new_a[i] = o; new_cnt++;
        break;
    }
}
void reservation_station::init(reorder_buffer* o, alunit* u, memory* w, control* cc, program_counter* pp) {
    RoB = o; Alu = u; Memo = w; co = cc; pc = pp;
}
void reservation_station::ld_ready(int id, int val) {
    new_a[id].A = val; new_a[id].is_prepared = 1; new_a[id].is_waiting = 0;    
}
void reservation_station::commit_st(int tag) {
    for (int i = 0; i < 32; ++i) {
        if (!a[i].is_busy) continue;
        if (a[i].Qk == tag) new_a[i].Qk = 0;
    } 
}
void reservation_station::remove_tag(int id, int val) {
    for (int i = 0; i < 32; ++i) {
        if (!a[i].is_busy) continue;
     //   if (i == 0 && id ==3) {std::cerr<<" KKKKK"<<std::endl;}
        if (a[i].Qj == id) {new_a[i].Qj = 0; new_a[i].Vj = val;}
        if (a[i].Qk == id) {new_a[i].Qk = 0; new_a[i].Vk = val;}
    }
}
void reservation_station::work_clk(int cl) {
    if (co->get_state()) {
        clear();
        return;
    }
   // if (cl>9999500) std::cerr<<cnt<<" RS"<<std::endl;
    for (int i = 0; i < 32; ++i) {
        if (!a[i].is_busy) continue;
        if (a[i].is_prepared) {
          //  std::cerr<<i<<" OoO1 "<<a[i].op<<std::endl;
            RoB->get_ready(a[i].dest, a[i].A);
            new_cnt--; new_a[i].is_busy = 0;
            if (a[i].op == END) return;
            if (a[i].op == SB || a[i].op == SH || a[i].op == SW) {RoB->getst(a[i].dest, a[i].Vk); return;}
            for (int j = 0; j < 32; ++j) {
                if (!a[j].is_busy) continue;
                if (a[j].Qj == a[i].dest) {
                    new_a[j].Qj = 0; new_a[j].Vj = a[i].A;
                }
                if (a[j].Qk == a[i].dest) {
                    new_a[j].Qk = 0; new_a[j].Vk = a[i].A;
                }
            }
        } else if (a[i].Qj == 0 && a[i].Qk == 0 && !a[i].is_waiting) {
          //  std::cerr<<i<<" OoO2 "<<a[i].op<<std::endl;
            ls_type o;
            switch (a[i].op) {
                case LUI: new_a[i].is_prepared = 1; break;
                case AUIPC: new_a[i].A = Alu->add(a[i].A, a[i].Vj);  new_a[i].is_prepared = 1; break;  //wait ALU;
                case JAL: new_a[i].A = Alu->add(a[i].Vj, 4); new_a[i].is_prepared = 1; break;
                case JALR: new_a[i].A = a[i].Vj + 4; pc->work_again((a[i].Vk + a[i].A) & (-2)); new_a[i].is_prepared = 1; break; //hard
                case BEQ: new_a[i].A = Alu->eq(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case BNE: new_a[i].A = !Alu->eq(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case BLT: new_a[i].A = Alu->LT(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case BGE: new_a[i].A = !Alu->LT(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case BLTU: new_a[i].A = Alu->LTU(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case BGEU: new_a[i].A = !Alu->LTU(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case LB:if (Memo->is_full()) break; new_a[i].is_waiting = 1;  o.type = 0;  // Qk -> earlier store
                        o.clk = 0; o.pos = a[i].Vj + a[i].A; o.Rs_id = i; Memo->push(o);  break;
                case LH:if (Memo->is_full()) break; new_a[i].is_waiting = 1; o.type = 1; 
                        o.clk = 0; o.pos = a[i].Vj + a[i].A; o.Rs_id = i; Memo->push(o);  break;
                case LW:if (Memo->is_full()) break; new_a[i].is_waiting = 1; o.type = 2; 
                        o.clk = 0; o.pos = a[i].Vj + a[i].A; o.Rs_id = i; Memo->push(o);  break;
                case LBU:if (Memo->is_full()) break; new_a[i].is_waiting = 1; o.type = 3; 
                        o.clk = 0; o.pos = a[i].Vj + a[i].A; o.Rs_id = i; Memo->push(o);  break;
                case LHU:if (Memo->is_full()) break; new_a[i].is_waiting = 1; o.type = 4; 
                        o.clk = 0; o.pos = a[i].Vj + a[i].A; o.Rs_id = i; Memo->push(o); break; // a bit hard
                case SB:
                case SH:
                case SW: new_a[i].A = Alu->add(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case ADDI: new_a[i].A = Alu->add(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case SLTI: new_a[i].A = Alu->LT(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case SLTIU: new_a[i].A = Alu->LTU(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case XORI: new_a[i].A = Alu->XOR(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case ORI: new_a[i].A = Alu->OR(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case ANDI: new_a[i].A = Alu->AND(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case SLLI: new_a[i].A = Alu->SLL(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case SRLI: new_a[i].A = Alu->SRL(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case SRAI: new_a[i].A = Alu->SRA(a[i].Vj, a[i].A); new_a[i].is_prepared = 1; break;
                case ADD: new_a[i].A = Alu->add(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case SUB: new_a[i].A = Alu->sub(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case SLL: new_a[i].A = Alu->SLL(a[i].Vj, a[i].Vk&31); new_a[i].is_prepared = 1; break;
                case SLT: new_a[i].A = Alu->LT(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case SLTU: new_a[i].A = Alu->LTU(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case XOR: new_a[i].A = Alu->XOR(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case SRL: new_a[i].A = Alu->SRL(a[i].Vj, a[i].Vk&31); new_a[i].is_prepared = 1; break;
                case SRA: new_a[i].A = Alu->SRA(a[i].Vj, a[i].Vk&31); new_a[i].is_prepared = 1; break;
                case OR: new_a[i].A = Alu->OR(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case AND: new_a[i].A = Alu->AND(a[i].Vj, a[i].Vk); new_a[i].is_prepared = 1; break;
                case END: new_a[i].is_prepared = 1; break;
            } 
        } else {
    //         if (cl > 9999500)
    // std::cerr<<i<<" OoO3 "<<a[i].dest<<' '<< a[i].op <<' '<<a[i].Qj <<' '<<a[i].Qk <<' '<< a[i].is_waiting<<std::endl;
            if (a[i].Qj) {
               // std::cerr<<i<<" OoO4 "<<a[i].op<<std::endl;
                std::pair<bool, int> t = RoB->ask(a[i].Qj);
               // std::cerr<<i<<" OoO5 "<<a[i].op<<std::endl;
                if (t.first) {
                    new_a[i].Qj = 0; new_a[i].Vj = t.second;
                }
            }
            if (a[i].Qk && (a[i].op < 10 || a[i].op > 14)) {
              //  std::cerr<<i<<" OoO6 "<<a[i].op <<' '<< a[i].Qk<<std::endl;
                std::pair<bool, int> t = RoB->ask(a[i].Qk);
               // std::cerr<<i<<" OoO7 "<<a[i].op<<std::endl;
                if (t.first) {
                    new_a[i].Qk = 0; new_a[i].Vk = t.second;
                }
            }
        }
    }
}
void reservation_station::clear() {
    new_cnt = 0;
    for (int i = 0; i < 32; ++i) new_a[i].is_busy = 0;
}
void reservation_station::neg_edge() {
    for (int i = 0; i < 32; ++i) a[i] = new_a[i]; 
    cnt = new_cnt;
}