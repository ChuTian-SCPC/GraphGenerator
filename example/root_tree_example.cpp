#include <iostream>

#include "graphgen.h"
#include "testlib.h"
using namespace std;
using namespace Generator;
int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    // 例如要得到一个10个点的有根树,根为3
    Tree t(10,true,3);
    t.GenTree();
    vector<pair<int, int>> e = t.GetEdge();
    for (auto [u, v] : e) {
        cout << u << " " << v << endl;
    }
    cout<<"Tree Root:"<<t.GetRoot()<<endl;//如果为无根树会产生错误！
    /*
    直接运行输出的是：
    3 1
    3 10
    3 5
    5 2
    1 9
    5 4
    3 6
    3 7
    10 8
    Tree Root:3
    */
    return 0;
}