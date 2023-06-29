#ifndef Memory
#define Memory
#include"lstype.h"
class control;
class reservation_station;
class memory{
private:
    bool* mem;
    ls_type lsque[33], new_lsque[33];
    int l,r,L,R;
    reservation_station* res;
    control* co;
public:
    memory();
    void init(bool* M, reservation_station* rr, control* cc);
    void push(const ls_type& x);
    bool is_full() const;
    void clk_work();
    void neg_edge();
    bool is_empty();    
};
#endif  