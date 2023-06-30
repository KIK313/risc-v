#ifndef PREDICT
#define PREDICT
class predictor{
private:
    int state[128], new_state[128];
public:
    predictor() {
        for (int i = 0; i < (1<<7); ++i) new_state[i] = state[i] = 2;
    }
    bool get_state(int id) const { return (state[id]>>1)&1;}
    void increase(int id) {new_state[id] = (state[id] + 1) & 3;}
    void decrease(int id) {new_state[id] = state[id] - (state[id]>0);}
    void neg_edge() {
        for (int i = 0; i < (1<<7); ++i) state[i] = new_state[i];
    }
};
#endif