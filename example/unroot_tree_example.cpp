#include"testlib.h"
#include"graphgen.h"
#include<iostream>
using namespace std;
using namespace Generator;
int main(int argc, char* argv[]){
  registerGen(argc, argv, 1);
  Tree t(10);
  t.GenTree();
  vector<pair<int,int>> e=t.GetEdge();
  for(auto [u,v]:e){
    cout<<u<<" "<<v<<endl;
  }
  return 0;
}