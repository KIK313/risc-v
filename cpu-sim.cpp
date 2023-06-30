#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
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
int a[8];
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
    for (int i = 0; i < 8; ++i) a[i] = i;
  //  cerr<<cl<<" -100"<<endl;
    while (!co.ed()) {
        random_shuffle(a,a+8);
        for (int i = 0; i < 8; ++i) {
            switch(a[i]) {
                case 0: co.work_clk(); break;
                case 1: iu.work_r_clk(cl); break;
                case 2: iu.work_w_clk(cl); break;//cerr<<"4"<<endl; 
                case 3: rob.clk_work(cl); break; //cerr<<"3"<<endl; 
                case 4: memo.clk_work(); break; //cerr<<"5"<<endl;
                case 5: pc.work_clk(); break; // cerr<<"6"<<endl; 
                case 6: rs.work_clk(cl); break; //cerr<<"7"<<endl;
                case 7: rf.work_clk(); break;
            }
        }
        cl ++;
        random_shuffle(a, a+8);
        for (int i = 0; i < 8; ++i) {
            switch(a[i]) {
                case 0: co.neg_edge(); break;
                case 1: iu.neg_edge(); break;
                case 2: memo.neg_edge(); break;
                case 3: pc.neg_edge(); break;
                case 4: rf.neg_edge(); break;
                case 5: rob.neg_edge(); break;
                case 6: rs.neg_edge(); break;
                case 7: pr.neg_edge();break;
            }
        }   
    }    
   // cerr<<cl<<" -100"<<endl;    
   cout<<(rf.getans()&255);       
   // cerr<<(rf.getans()&255)<<" ANS"<<endl;
    return 0;
}