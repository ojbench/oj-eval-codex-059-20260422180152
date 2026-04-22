#include <bits/stdc++.h>
#include "src.hpp"
using namespace std;
int main(){
    {
        ICPCJudger j(1000,1000,"a\nb\n");
        j.Submit(500,500,"a\nb\n");
        cout << "ICPC:" << j.GetScore() << "\n"; // 100
        j.Submit(2000,500,"a\nb\n");
        cout << "ICPC2:" << j.GetScore() << "\n"; // 100 stays
    }
    {
        OIJudger j(1000,1000,"a \nb\n");
        j.Submit(500,500,"a\nb\n");
        cout << "OI:" << j.GetScore() << "\n"; // 100
        j.Submit(500,500,"a\nb\n");
        cout << "OI2:" << j.GetScore() << "\n"; // 0
    }
    {
        SpacialJudger j(1000,1000,200,200,"ok\n");
        j.Submit(200,200,"ok\n");
        cout << "SP1:" << j.GetScore() << "\n"; // 100
        j.Submit(600,600,"ok\n");
        cout << "SP2:" << j.GetScore() << "\n"; // compute
    }
}
