#include <iostream>

#include "graphgen.h"
#include "testlib.h"
using namespace std;
using namespace Generator;
int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
    // 例如要得到一个10个点的无根树
    Tree t(10);
    t.GenTree();
    vector<pair<int, int>> e = t.GetEdge();
    for (auto [u, v] : e) {
        cout << u << " " << v << endl;
    }
    /*
    直接运行输出的是：
    7 3
    8 3
    10 5
    9 3
    2 1
    7 6
    3 4
    7 10
    10 1
    */
    return 0;
}