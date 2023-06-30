#include<iostream>
#include<cstdio>
#include<cstring>
#include"ALU.h"
#include"control.h"
#include"insu.h"
#include"mem.h"
#include"opcode.h"
#include"pc.h"
#include"regf.h"
#include"ROB.h"
#include"RS.h"
#include"predict.h"
#include<iostream>
using namespace std;
bool Mem[1<<25];
char ch[22];
inline int HtoD(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    return ch - 'A' + 10;
}
void store_memory() {
    scanf("%s", ch);
    char code[8];
    while (1) {
        int pos = 0;
        for (int i = 1; i < strlen(ch); ++i) pos = pos * 16 + HtoD(ch[i]);
        pos = pos * 8;
        //std::cerr<<pos<<" -100\n";
        while(1) {
            if (scanf("%s", ch) == EOF) return; 
            if (ch[0] == '@') break;
            
            int x = 0; 
            code[0] = ch[1]; code[1] = ch[0];
            scanf("%s", ch); code[2] = ch[1]; code[3] = ch[0];
            scanf("%s", ch); code[4] = ch[1]; code[5] = ch[0];
            scanf("%s", ch); code[6] = ch[1]; code[7] = ch[0];
            for (int i = 0; i < 8; ++i) x |= (HtoD(code[i]) << (4*i));
            //if (pos == 4116 * 8) cerr << x << " IOI"<<endl;
            //cerr<<x<<" check the instruction"<<endl;
            for (int i = 0; i < 32; ++i) Mem[pos+i] = (x >> i) & 1;
            pos += 32;
        }
    }
}
int main() {
  //  freopen("a.in","r",stdin);
    memset(Mem, 0, sizeof(Mem));
    store_memory();
    alunit al; control co; instruction_unit iu; memory memo; program_counter pc;
    register_file rf; reorder_buffer rob; reservation_station rs; predictor pr;
    co.init(&memo); iu.init(Mem, &rob, &rs, &rf, &co, &pc, &pr); memo.init(Mem, &rs, &co);
    pc.init(&co); rob.init(&rs, &co, &rf, &memo, &pc, &pr); rs.init(&rob, &al, &memo, &co, &pc);
    rf.init(&co);
    int cl = 0;
  //  cerr<<cl<<" -100"<<endl;
    while (!co.ed()) {
        cl ++;
        co.work_clk(); //cerr<<"1"<<endl;
        iu.work_r_clk(cl); //cerr<<"2"<<endl; 
        rob.clk_work(cl); //cerr<<"3"<<endl; 
        iu.work_w_clk(cl); //cerr<<"4"<<endl; 
        memo.clk_work(); //cerr<<"5"<<endl;
        pc.work_clk(); // cerr<<"6"<<endl; 
        rs.work_clk(cl); //cerr<<"7"<<endl;
        rf.work_clk();
       // if (cl>9999500)   
     //  cerr<<cl<<" CLOCK TIME -100"<<endl;
        co.neg_edge(); iu.neg_edge(); memo.neg_edge(); pc.neg_edge();
        rf.neg_edge(); rob.neg_edge(); rs.neg_edge(); pr.neg_edge();
    }    
   // cerr<<cl<<" -100"<<endl;    
   cout<<(rf.getans()&255);       
   // cerr<<(rf.getans()&255)<<" ANS"<<endl;
    return 0;
}