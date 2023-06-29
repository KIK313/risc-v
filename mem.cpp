#include"lstype.h"
#include"mem.h"
#include"RS.h"
#include"control.h"
memory::memory() {
    l = r = 0; 
    L = R = 0;
}
void memory::init(bool* M, reservation_station *rr, control *cc) {
    mem = M; res = rr; co = cc;
}
void memory::push(const ls_type& x) {
    new_lsque[R] = x;
    R = (R + 1) & 31;
}
bool memory::is_full() const {
    return (((r+1)&31) == l);
}
bool memory::is_empty() {
    return (l == r);
}
void memory::clk_work() { // lb lh lw lbu lhu sb sh sw
    if (co->get_state()) {
        L = 0; R = 0; int x = l;
        while (x != r) {
            if (lsque[x].type >= 5) {
                new_lsque[R] = lsque[x];
                R = (R + 1) & 31;
            }           
            x = (x + 1) & 31;
        }
        return;
    }
    if (l == r) return;
    if (lsque[l].clk == 2) {
        L = (l + 1) & 31;
        if (lsque[l].type < 5) {
            int t = 0;
            if (lsque[l].type % 3 == 0) t = 8;
            if (lsque[l].type % 3 == 1) t = 16;
            if (lsque[l].type == 2) t = 32;
            int x = 0;
            for (int i = 0; i < t; ++i) x |= (mem[i+lsque[l].pos*8] << i);
            if (lsque[l].type < 3) {
                if ((x >> (t-1)) & 1) x = x & (-1);
            }     
            res->ld_ready(lsque[l].Rs_id, x);
        } else {
            int t = 0;
            if (lsque[l].type == 5) t = 8;
            if (lsque[l].type == 6) t = 16;
            if (lsque[l].type == 7) t = 32;
            for (int i = 0; i < t; ++i) mem[lsque[l].pos*8+i] = (lsque[l].val >> i) & 1;
        }         
    } else new_lsque[l].clk++;
}
void memory::neg_edge() {
    l = L; r = R;
    int x = l;
    while (x != r) {
        lsque[x] = new_lsque[x];
        x = (x + 1) & 31;
    }
} 