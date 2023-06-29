#ifndef Control
#define Control
class memory;
class control {
private:
    bool state,new_state;
    bool is_end, new_is_end;
    bool cte, new_cte;
    memory* memo;
public:
    control();
    void init(memory* mm);
    bool get_state();
    void work_clk();
    void neg_edge();
    bool ed();
    void pre_wrong();
    void come_to_end();
};
#endif