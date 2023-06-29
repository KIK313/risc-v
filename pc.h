#ifndef Program_Counter
#define Program_Counter
class control;
class program_counter {
private:
    int pc, new_pc;
    bool is_clear, new_is_clear;
    int clpos,new_clpos;
    bool is_stop, new_is_stop;
    bool is_restart, new_is_restart;
    int rest,new_rest;
    control* co;
public:
    program_counter();
    void init(control* cc);
    int get_pc() const;
    bool is_rest();
    bool get_stop();
    void work_clk();
    void stop_pc();
    void work_again(int pos);
    void move_pc(int x);
    void get_clear(int x);
    void neg_edge();
};
#endif