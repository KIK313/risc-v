#ifndef REG_FILE
#define REG_FILE
#include <iostream>
#include <cstdio>
class control;
class register_file {
private:
    int reg[32], new_reg[32]; int stt;
    int tag[32], new_tag[32]; int new_stt;
    control* co;
public:
    register_file();
    std::pair<bool,int> get_src(int id);
    int get_ld() const;
    void init(control* cc);
    void clear_rf();
    void work_clk();
    void add_tag(int id, int tg);
    void modify_st(int id);
    void commit_st(int sttag);
    void commit_tag(int id, int tg, int val);
    void neg_edge();
    int getans();
};
#endif