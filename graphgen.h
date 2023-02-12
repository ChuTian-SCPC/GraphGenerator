#include"testlib.h"

void FailGen(const char* msg,...){
	printf("\033[31mFAIL!\033[0m \n");
	va_list value;
	va_start(value,msg);
	vfprintf(stdout,msg,value);
	va_end(value);
	exit(EXIT_FAILURE);
}

namespace Generator{

int NODE_LIMIT=1e6;//结点数的上限
int EDGE_LIMIT=1e6;//边数的上限

//一颗随机生成的树,默认为无根
class Tree {
	protected:
		int node;//节点数
		bool is_root;//是否是有根树,默认为false,如果是有根树的话，边为(father,son);否则边为(father,son)与(son,father)中的任意一个
		int root;//根，默认为1,只在is_rooted=1的时候生效
		std::vector<std::pair<int, int>> edge; //边
		std::vector<int> p;
		void JudgeLimits(){
			if(node<=0){
				FailGen("node must be a positive integer.\n");
			}
			if(node>NODE_LIMIT){
				FailGen("node must less than or equal to %d, or you can change the limit.\n",NODE_LIMIT);
			}
			if(is_root && (root<0 || root>node-1)){
				FailGen("restriction of the root is [0,%d]\n",node-1);
			}
		}
		void InitGen(){
			JudgeLimits();
			edge.clear();
			p.clear();
			p.resize(node,0);
			for (int i = 0; i < node; i++) {
				p[i] = i;
			}
			shuffle(p.begin(), p.end());			
			if(is_root){
				for(int i=0;i<node;i++){
					if(p[i]==root){
						std::swap(p[0],p[i]);
						break;
					}
				}
			}
		}
		void AddEdge(int u,int v){
			if(is_root || rnd.next(2)) {
				edge.push_back({u,v});
			} else {
				edge.push_back({v,u});
			}
		}
	public:
		/**
		 * 默认为无根树
		 * @param n 结点数
		 * @param is_rt 是否有根
		 * @param rt 根,默认为0,范围应为[0,n-1],只在is_root=1时生效
		*/
		Tree(int n=1,bool is_rt=0,int rt=0) {
			node=n;
			is_root=is_rt;
			root=rt;
		}
		/**
		 * 设置结点数
		 * @param n 结点数
		*/
		void SetNode(int n) {     
			node=n;
		}
		/**
		 * 设置无根/有根树,如果是有根树的话，边为(father,son);否则边为(father,son)与(son,father)中的任意一个
		 * @param is_rt 是否有根
		*/
		void SetIsRoot(bool is_rt){
			is_root=is_rt;
		}
		/**
		 * 设置根，只在有根树的时候生效
		 * @param rt 根,范围应为[0,n-1]
		*/
		void SetRoot(int rt){
			root=rt;
		}		
		/**
		 * 获取边
		 * @return 边
		*/
		std::vector<std::pair<int, int>> GetEdge() {
			return edge;
		}
		void GenTree() {
			InitGen();
			for (int i = 1; i < node; i++) {
				int f=rnd.next(i);
				AddEdge(p[f],p[i]);
			}
			shuffle(edge.begin(),edge.end());
		}
};
//链
class Chain:public Tree {
	public:
		/**
		 * 一条链，默认为无根树
		 * @param n 结点数
		 * @param is_rt 是否有根
		 * @param rt 根,默认为0,范围应为[0,n-1],只在is_root=1时生效
		*/
		Chain(int n=1,bool is_rt=0,int rt=0) {
			node=n;
			is_root=is_rt;
			root=rt;
		}
		void GenTree(){
			InitGen();
			for (int i = 1; i < node; i++) {
				AddEdge(p[i-1],p[i]);
			}
			shuffle(edge.begin(),edge.end());
		}
};
//菊花图
class Flower:public Tree {
	public:
		/**
		 * 一个菊花图，默认为无根树
		 * @param n 结点数
		 * @param is_rt 是否有根
		 * @param rt 根,默认为0,范围应为[0,n-1],只在is_root=1时生效
		*/
		Flower(int n=1,bool is_rt=0,int rt=0) {
			node=n;
			is_root=is_rt;
			root=rt;
		}
		void GenTree(){
			InitGen();
			for (int i = 1; i < node; i++) {
				AddEdge(p[0],p[i]);
			}
			shuffle(edge.begin(),edge.end());
		}
};
}