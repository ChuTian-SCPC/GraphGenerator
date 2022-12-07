#include"testlib.h"

//树的基类
class Tree {
	protected:
		int node;//节点数
		int shift;//偏移值，决定树的形状
		std::vector<std::pair<int, int>> edge; //边
	public:
		/**
		 * @param n 结点数
		*/
		Tree(int n=1) {
			node=n;
		}
		/**
		 * 设置结点数
		 * @param n 结点数
		*/
		void SetNode(int n) {
			node=n;
		}
		/**
		 * 设置树的形态（链，菊花)
		 * @param s 偏移值，决定树的形状
		*/
		void SetShift(int s) {
			shift=s;
		}
		/**
		 * 获取边
		 * @return 边
		*/
		std::vector<std::pair<int, int>> GetEdge() {
			return edge;
		}
		virtual void GenTree() {}
};

//无根树，随机生成
class UnrootTree:public Tree {
	public:
		/**
		 * @param n 结点数
		*/
		UnrootTree(int n=1) {
			node=n;
			shift=0;
		}
		/**
		 * 生成树
		*/
		void GenTree() {
			edge.clear();
			std::vector<int> p(node);
			for (int i = 0; i < node; i++) {
				p[i] = i;
			}
			shuffle(p.begin() + 1, p.end());
			for (int i = 1; i < node; i++) {
				int f = rnd.wnext(i, shift);
				if(rnd.next(2)) {
					edge.push_back({p[i],p[f]});
				} else {
					edge.push_back({p[f],p[i]});
				}
			}
			shuffle(edge.begin(),edge.end());
		}
};
//无根树，链
class UnrootChain:public UnrootTree {
	public:
		/**
		 * @param n 结点数
		*/
		UnrootChain(int n=1) {
			node=n;
			shift=100000;
		}
};
//无根树，菊花
class UnrootFlower:public UnrootTree {
	public:
		/**
		 * @param n 结点数
		*/
		UnrootFlower(int n=1) {
			node=n;
			shift=-100000;
		}
};
//有根树，随机生成
class RootTree:public Tree {
	public:
		int root,rootfather;//根，根父亲的表示
		std::vector<int> father;//父亲
		/***
		 * @param n 结点数
		 * @param rt 根
		 * @param rtf 根父亲的表示
		*/
		RootTree(int n=1,int rt=0,int rtf=-1) {
			node=n;
			shift=0;
			root=std::min(rt,(int)(n-1));
			rootfather=rtf;
		}
		/**
		 * 设置结点数
		 * @param n 结点数
		*/
		void SetNode(int n) {
			node=n;
			root=std::min((int)(n-1),root);
		}
		/**
		 * 设置根
		 * @param rt 根
		*/
		void SetRoot(int rt) {
			root=std::min(rt,(int)(node-1));
		}
		/**
		 * 设置根父亲的表示
		 * @param rtf 根父亲的表示
		*/
		void SetRootFather(int rtf) {
			rootfather=rtf;
		}
		/**
		 * 获取每个点的父亲节点
		*/
		std::vector<int> GetFather() {
			return father;
		}
		/**
		 * 生成树
		*/
		void GenTree() {
			edge.clear();
			father.resize(node);
			std::vector<int> p(node);
			for (int i = 0; i < node; i++) {
				p[i] = i;
			}
			std::swap(p[0],p[root]);
			shuffle(p.begin() + 1, p.end());
			for (int i = 1; i < node; i++) {
				int f=rnd.wnext(i, shift);
				father[p[i]] = p[f];
				if(rnd.next(2)) {
					edge.push_back({p[i],p[f]});
				} else {
					edge.push_back({p[f],p[i]});
				}
			}
			father[root]=rootfather;
			shuffle(edge.begin(),edge.end());
		}
};
//无根树，链
class RootChain:public RootTree {
	public:
		/***
		 * @param n 结点数
		 * @param rt 根
		 * @param rtf 根父亲的表示
		*/
		RootChain(int n=1,int rt=0,int rtf=-1) {
			node=n;
			shift=100000;
			root=std::min(rt,(int)(n-1));
			rootfather=rtf;
		}
};
//无根树，菊花
class RootFlower:public RootTree {
	public:
		/***
		 * @param n 结点数
		 * @param rt 根
		 * @param rtf 根父亲的表示
		*/
		RootFlower(int n=1,int rt=0,int rtf=-1) {
			node=n;
			shift=-100000;
			root=std::min(rt,(int)(n-1));
			rootfather=rtf;
		}
};
//普通图
class Graph {
	protected:
		int node,side;//点数，边数
		std::vector<std::pair<int, int>> edge;//边
		std::map<std::pair<int,int>,bool> e;//边的去重
		/**
		 *0：无向
		 *1：有向
		 *默认无向图
		*/
		bool direction;
		/**
		 *0：无重边
		 *1：有重边
		 *默认无重边
		*/
		bool multiply_edge;
		/**
		 *0：无自环
		 *1：有自环
		 *默认无自环
		*/
		bool self_loop;
		/**
		 *0：不保证连通
		 *1：保证连通
		 *默认不保证连通
		*/
		bool connect;
		bool JudgeSelfLoop(int u,int v) {
			return self_loop==0 && u==v;
		}
		bool JudgeMultiplyEdge(int u,int v) {
			if(e[ {u,v}])	return true;
			return false;
		}
		void AddEdge(int u,int v) {
			if(direction==1){
				edge.push_back({u,v});
			}
			else{
				if(rnd.next(2)) {
					edge.push_back({u,v});
				} else {
					edge.push_back({v,u});
				}	
			}	
			e[ {u,v}]=1;
			if(direction==0) {
				e[ {v,u}]=1;
			}
		}
	public:
		/**
		 * @param n 结点数
		 * @param m 边数
		 * @note 其余参数默认如下：
		 * @note direction=0 无向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=0 不保证连通
		*/
		Graph(int n=1, int m=0) {
			node=n;
			side=m;
			direction=0;
			multiply_edge=0;
			self_loop=0;
			connect=0;
		}
		/**
		 * 设置结点数
		 * @param n 结点数
		*/
		virtual void SetNode( int n) {
			node=n;
		}
		/**
		 * 设置边数
		 * @param m 边数
		*/
		virtual void SetSide( int m) {
			side=m;
		}
		/**
		 * 设置有向/无向图
		 * @param f 0：无向;1：有向;默认无向图
		*/
		virtual void SetDirection(bool f) {
			direction=f;
		}
		/**
		 * 设置能不能存在重边
		 * @param f 0：无重边;1：有重边;默认无重边
		*/
		virtual void SetMultiplyEdge(bool f) {
			multiply_edge=f;
		}
		/**
		 * 设置能不能存在自环
		 * @param f 0：无自环;1：有自环;默认无自环
		*/
		virtual void SetSelfLoop(bool f) {
			self_loop=f;
		}
		/**
		 * 设置保不保证一定连通
		 * @param f 0：不保证一定连通;1：保证一定连通;默认不保证一定连通
		*/
		virtual void SetConnect(bool f) {
			connect=f;
		}
		/**
		 * 获取边
		*/
		std::vector<std::pair<int, int>> GetEdge() {
			return edge;
		}
		/**
		 * 生成图
		*/
		virtual void GenGraph() {
			edge.clear();
			e.clear();
			int m=side;
			if(multiply_edge==0) {
				long long max=( long long)node*( long long)(node-1);
				if(direction==0) {
					max/=2;
				}
				m=std::min(( long long)side,max);
			}
			if(connect) {
				m=std::max(0,m-(node-1));
				UnrootTree tree(node);
				tree.GenTree();
				edge=tree.GetEdge();
				for(auto x:edge) {
					e[x]=1;
				}
			}
			while(m--) {
				int u,v;
				do {
					u=rnd.next(node);
					v=rnd.next(node);
				} while(JudgeSelfLoop(u,v) || JudgeMultiplyEdge(u,v));
				AddEdge(u,v);
			}
			shuffle(edge.begin(),edge.end());
		}
};
/**
 * 二分图
 * @note 强制设direction=0,self_loop=0
*/
class BipartiteGraph:public Graph {
	public:
		/**
		 * @param n 结点数
		 * @param m 边数
		 * @note 其余参数默认如下：
		 * @note direction=0 无向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=0 不保证连通
		*/
		BipartiteGraph(int n=1, int m=0) {
			node=n;
			side=m;
			direction=0;
			multiply_edge=0;
			self_loop=0;
			connect=0;
		}
		/**
		 * 强制无向图
		 * @param f 无论传入什么都会无向图
		*/
		void SetDirection(bool f) {
			direction=0;
		}
		/**
		 * 强制无自环
		 * @param f 无论传入什么都会无自环
		*/
		void SetSelfLoop(bool f) {
			self_loop=0;
		}
		/**
		 * 生成图
		*/
		void GenGraph() {
			edge.clear();
			e.clear();
			int m=side;
			if(multiply_edge==0) {
				long long max=( long long)(node/2)*( long long)((node+1)/2);
				m=std::min(( long long)side,max);
			}
			int left,right;
			std::vector<int> part[2];
			int l=1,r=node/2,limit;
			if(multiply_edge==0){
				while(l<=r){
					int mid=(l+r)/2;
					if(mid*(node-mid)<m){
						l=mid+1;
					}
					else{
						limit=r;
						r=mid-1;
					}
				}	
			}
			else{
				limit=1;
			}
			do {
				left=rnd.next(limit,node/2);
				right=node-left;
			} while(left*right<m);
			std::vector<int> p(node);
			for(int i=0; i<node; i++) {
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			for(int i=0; i<node; i++) {
				if(i<left) {
					part[0].push_back(p[i]);
				} else {
					part[1].push_back(p[i]);
				}
			}
			if(connect) {
				m=std::max(0,m-(node-1));
				for(int i=0; i<left-1; i++) {
					AddEdge(part[0][i],part[1][i]);
					AddEdge(part[1][i],part[0][i+1]);
				}
				for(int i=left-1; i<right; i++) {
					int v=rnd.any(part[0]);
					AddEdge(part[1][i],v);
				}
			}
			while(m--) {
				int u,v;
				do{
					u=rnd.any(part[0]);
					v=rnd.any(part[1]);
				}while(JudgeSelfLoop(u,v) || JudgeMultiplyEdge(u,v));
				AddEdge(u,v);
			}
			shuffle(edge.begin(),edge.end());
		}
};
/**
 * DAG
 * @note 强制设self_loop=0,direction=1
*/
class DAG:public Graph{
	public:
	/**
		 * @param n 结点数
		 * @param m 边数
		 * @note 其余参数默认如下：
		 * @note direction=1 有向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=0 不保证连通
		*/
		DAG(int n=1,int m=0) {
			node=n;
			side=m;
			direction=1;
			multiply_edge=0;
			self_loop=0;
			connect=0;
		}
		/**
		 * 强制无向图
		 * @param f 无论传入什么都会无向图
		*/
		void SetDirection(bool f) {
			direction=1;
		}
		/**
		 * 强制无自环
		 * @param f 无论传入什么都会无自环
		*/
		void SetSelfLoop(bool f) {
			self_loop=0;
		}
		/**
		 * 生成图
		*/
		void GenGraph(){
			edge.clear();
			e.clear();
			std::vector<int> p(node);
			int m=std::min(side,node*(node-1)/2);
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			if(connect){
				m=std::max(0,m-(node-1));
				for(int i=1;i<node;i++){
					int f=rnd.next(i);
					AddEdge(p[f],p[i]);
				}
			}
			while(m--){
				int u,v;
				do{
					u=rnd.next(node);
					v=rnd.next(node);
				}while(JudgeSelfLoop(p[u],p[v]) || JudgeMultiplyEdge(p[u],p[v]));
				if(u>v){
					std::swap(u,v);
				}
				AddEdge(p[u],p[v]);
			}
		}
};
/**
 * 环图
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,side=n
*/
class CycleGraph:public Graph{
	public:
		/**
		 * @param n 结点数
		 * @note 其余参数默认如下：
		 * @note side=n 边数固定
		 * @note direction=0 无向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=1 保证连通
		*/
		CycleGraph(int n=1) {
			node=n;
			side=n;
			direction=0;
			multiply_edge=0;
			self_loop=0;
			connect=1;
		}
		/**
		 * 设置点数
		 * @param n 节点数
		*/
		void SetNode(int n){
			node=n;
			side=n;
		}
		/**
		 * 强制边数等于点数
		 * @param f 无论传入什么都不会改变边数的值
		*/
		void SetSide(int m) {
			side=node;
		}
		/**
		 * 强制无重边
		 * @param f 无论传入什么都无重边
		*/
		void SetMultiplyEdge(bool f) {
			multiply_edge=0;
		}
		/**
		 * 强制无自环
		 * @param f 无论传入什么都无自环
		*/
		void SetSelfLoop(bool f) {
			self_loop=0;
		}
		/**
		 * 强制连通
		 * @param f 无论传入什么都连通
		*/
		void SetConnect(bool f) {
			connect=1;
		}
		/**
		 * 根据点集生成图
		 * @param a 点集
		 * @attention 如果结点集合有重复点则会产生错误
		*/	
		void GenGraph(std::vector<int> a){
			edge.clear();
			e.clear();
			int n=a.size();
			for(int i=1;i<n;i++){
				AddEdge(a[i],a[i-1]);
			}
			if(JudgeSelfLoop(a[0],a[n-1])==0 && JudgeMultiplyEdge(a[0],a[n-1])==0){
				AddEdge(a[0],a[n-1]);
			}
		}
		/**
		 * 根据结点数生成图
		*/
		void GenGraph(){		
			std::vector<int> p(node);
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			GenGraph(p);
			shuffle(edge.begin(),edge.end());
		}
};
/**
 * 轮图
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,side=2n-2
*/
class WheelGraph:public Graph{
	public:
		/**
		 * @param n 结点数
		 * @note 其余参数默认如下：
		 * @note side=2*n-2 边数固定
		 * @note direction=0 无向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=1 保证连通 
		*/
		WheelGraph(int n=1) {
			node=n;
			side=2*n-2;
			direction=0;
			multiply_edge=0;
			self_loop=0;
			connect=1;
		}
		/**
		 * 设置结点数
		 * @param n 结点数
		*/
		void SetNode(int n){
			node=n;
			side=2*n-2;
		}
		/**
		 * 强制边数等于2*node-2
		 * @param f 无论传入什么都不会改变边数的值
		*/
		void SetSide(int m) {
			side=2*node-2;
		}
		/**
		 * 强制无重边
		 * @param f 无论传入什么都无重边
		*/
		void SetMultiplyEdge(bool f) {
			multiply_edge=0;
		}
		/**
		 * 强制无自环
		 * @param f 无论传入什么都无自环
		*/
		void SetSelfLoop(bool f) {
			self_loop=0;
		}
		/**
		 * 强制连通
		 * @param f 无论传入什么都连通
		*/
		void SetConnect(bool f) {
			connect=1;
		}
		/**
		 * 根据指定支配点和其余点生成图
		 * @param v 指定点
		 * @param p 其余点
		 * @attention 如果结点集合有重复点则会产生错误
		*/
		void GenGraph(int v,std::vector<int> p){
			edge.clear();
			e.clear();
			CycleGraph cycle;
			cycle.GenGraph(p);
			edge=cycle.GetEdge();
			for(int i=0;i<p.size();i++){
				AddEdge(v,p[i]);
			}
			shuffle(edge.begin(),edge.end());
		}
		/**
		 * 根据指定点生成图，支配点随机
		 * @param p 点集，不可为空
		 * @attention 如果结点集合有重复点则会产生错误
		*/
		void GenGraph(std::vector<int> p){
			shuffle(p.begin(),p.end());
			std::vector<int> pre(p.begin()+1,p.end());
			GenGraph(p[0],pre);
		}
		/**
		 * 根据点数生成图
		*/
		void GenGraph(){
			std::vector<int> p(node);
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			GenGraph(p);
		}
};
/**
 * 网格图
 * @note 强制设connect=1,self_loop=0
*/
class GridGraph:public Graph{
	protected:
		int CountSide(int x,int y){
			int sum=x*(y-1)+y*(x-1)-2*((y-node%y)%y);
			if(direction==1){
				sum*=2;
			}
			return sum;
		}
	public:
		/**
		 * @param n 结点数
		 * @param m 边数
		 * @note 其余参数默认如下：
		 * @note direction=0 无向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=1 保证连通
		*/
		GridGraph(int n=1,int m=0) {
			node=n;
			side=m;
			direction=0;
			multiply_edge=0;
			self_loop=0;
			connect=1;
		}
		/**
		 * 强制无自环
		 * @param f 无论传入什么都无自环
		*/
		void SetSelfLoop(bool f) {
			self_loop=0;
		}
		/**
		 * 强制连通
		 * @param f 无论传入什么都连通
		*/
		void SetConnect(bool f) {
			connect=1;
		}
		/**
		 * 生成图
		*/
		void GenGraph(){
			edge.clear();
			e.clear();
			int m,column,row;
			std::pair<int,int> max={0,0};
			std::vector<int> possible,p(node);
			int d[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
			for(int i=1;i<node;i++){
				int x=i,y=(node+i-1)/i;
				int w=CountSide(y,x);
				if(w>max.first){
					max={w,i};
				}
				if(w>=side){
					possible.push_back(i);
				}
			}
			if(possible.size()==0){
				m=max.first;
				row=max.second;
				column=(node+row-1)/row;
			}
			else{
				row=rnd.any(possible);
				column=(node+row-1)/row;
				m=CountSide(column,row);
			}
			m=std::max(0,m-(node-1));
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			for(int i=0;i<row;i++){
				for(int j=1;j<column;j++){
					int x=i*column+j,y=x-1;
					if(x>=node)	continue;
					if(i%2==0){
						AddEdge(p[y],p[x]);
					}
					else{
						AddEdge(p[x],p[y]);
					}
				}
				int x=i*column,y=(i+1)*column;
				if(x<node && y<node){
					AddEdge(p[x],p[y]);
				}				
			}
			while(m--){
				int pos,k,px,py,nxt;
				do{
					pos=rnd.next(node);
					k=rnd.next(4);
					px=pos/column+d[k][0];
					py=pos%column+d[k][1];
					nxt=px*column+py;
				}while(px<0 || px>=row || py<0 || py>=column || nxt>=node || JudgeMultiplyEdge(p[pos],p[nxt]));
				AddEdge(p[pos],p[nxt]);
			}
			shuffle(edge.begin(),edge.end());
		}
};
/**
 * 基环树
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=0,side=n
*/
class PseudoTree:public Graph{
	public:
		/**
		 * @param n 结点数
		 * @attention n>=3,否则会产生问题
		 * @note 其余参数默认如下：
		 * @note side=n 边数固定
		 * @note direction=0 无向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=1 保证连通
		*/
		PseudoTree(int n=3) {
			node=n;
			side=n;
			direction=0;
			multiply_edge=0;
			self_loop=0;
			connect=1;
		}
		/**
		 * 设置点数
		 * @param n 点数
		*/
		void SetNode(int n){
			node=n;
			side=node;
		}
		/**
		 * 强制边数等于点数
		 * @param f 无论传入什么都不会改变边数的值
		*/
		void SetSide(int m) {
			side=node;
		}
		/**
		 * 强制无向
		 * @param f 无论传入什么都无向
		*/
		void SetDirection(bool f) {
			direction=0;
		}
		/**
		 * 强制无重边
		 * @param f 无论传入什么都无重边
		*/
		void SetMultiplyEdge(bool f) {
			multiply_edge=0;
		}
		/**
		 * 强制无自环
		 * @param f 无论传入什么都无自环
		*/
		void SetSelfLoop(bool f) {
			self_loop=0;
		}
		/**
		 * 强制连通
		 * @param f 无论传入什么都连通
		*/
		void SetConnect(bool f) {
			connect=1;
		}
		/**
		 * 根据指定环大小生成图，如果不传入参数则随机环大小
		 * @param size 环大小
		*/
		void GenGraph(int size=-1){
			edge.clear();
			e.clear();
			if(size==-1){
				size=rnd.next(3,node);
			}
			size=std::min(size,node);
			std::vector<int> p(node);
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			CycleGraph cycle;
			std::vector<int> pre(p.begin(),p.begin()+size);
			cycle.GenGraph(pre);
			edge=cycle.GetEdge();
			for(int i=size;i<node;i++){
				int f=rnd.next(i);
				AddEdge(p[i],p[f]);
			}
			shuffle(edge.begin(),edge.end());
		}
};
/**
 * 基环内向树
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=1,side=n
*/
class PseudoInTree:public PseudoTree{
	public:
		/**
		 * @param n 结点数
		 * @attention n>=3,否则会产生问题
		 * @note 其余参数默认如下：
		 * @note side=n 边数固定
		 * @note direction=1 有向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=1 保证连通
		*/
		PseudoInTree(int n=3) {
			node=n;
			side=n;
			direction=1;
			multiply_edge=0;
			self_loop=0;
			connect=1;
		}
		/**
		 * 强制有向
		 * @param f 无论传入什么都有向
		*/
		void SetDirection(bool f) {
			direction=1;
		}
		/**
		 * 根据指定环大小生成图，如果不传入参数则随机环大小
		 * @param size 环大小
		*/
		void GenGraph(int size=-1){
			edge.clear();
			e.clear();
			if(size==-1){
				size=rnd.next(3,node);
			}
			size=std::min(size,node);
			std::vector<int> p(node);
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			CycleGraph cycle;
			cycle.SetDirection(1);
			std::vector<int> pre(p.begin(),p.begin()+size);
			cycle.GenGraph(pre);
			edge=cycle.GetEdge();
			for(int i=size;i<node;i++){
				int f=rnd.next(i);
				AddEdge(p[i],p[f]);
			}
			shuffle(edge.begin(),edge.end());
		}
};
/**
 * 基环外向树
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=1,side=n
*/
class PseudoOutTree:public PseudoTree{
	public:
		/**
		 * @param n 结点数
		 * @attention n>=3,否则会产生问题
		 * @note 其余参数默认如下：
		 * @note side=n 边数固定
		 * @note direction=1 有向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=1 保证连通
		*/
		PseudoOutTree(int n=3) {
			node=n;
			side=n;
			direction=1;
			multiply_edge=0;
			self_loop=0;
			connect=1;
		}
		/**
		 * 强制有向
		 * @param f 无论传入什么都有向
		*/
		void SetDirection(bool f) {
			direction=1;
		}
		/**
		 * 根据指定环大小生成图，如果不传入参数则随机环大小
		 * @param size 环大小
		*/
		void GenGraph(int size=-1){
			edge.clear();
			e.clear();
			if(size==-1){
				size=rnd.next(3,node);
			}
			size=std::min(size,node);
			std::vector<int> p(node);
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			CycleGraph cycle;
			cycle.SetDirection(1);
			std::vector<int> pre(p.begin(),p.begin()+size);
			cycle.GenGraph(pre);
			edge=cycle.GetEdge();
			for(int i=size;i<node;i++){
				int f=rnd.next(i);
				AddEdge(p[f],p[i]);
			}
			shuffle(edge.begin(),edge.end());
		}
};
/**
 * 仙人掌
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=0
 * @note 仙人掌的边数的范围应该是[n-1,n-1+(n-1)/2]
*/
class Cactus:public Graph{
	public:
		/**
		 * @param n 结点数
		 * @param m 边数
		 * @note 其余参数默认如下：
		 * @note direction=0 无向图
		 * @note multiply_edge=0 无重边
		 * @note self_loop=0 无自环
		 * @note connect=1 保证连通
		*/
		Cactus(int n=1,int m=0) {
			node=n;
			side=m;
			direction=0;
			multiply_edge=0;
			self_loop=0;
			connect=1;
		}
		/**
		 * 强制无向
		 * @param f 无论传入什么都无向
		*/
		void SetDirection(bool f) {
			direction=0;
		}
		/**
		 * 强制有无重边
		 * @param f 无论传入什么都无重边
		*/
		void SetMultiplyEdge(bool f) {
			multiply_edge=0;
		}
		/**
		 * 强制无自环
		 * @param f 无论传入什么都无自环
		*/
		void SetSelfLoop(bool f) {
			self_loop=0;
		}
		/**
		 * 强制连通
		 * @param f 无论传入什么都连通
		*/
		void SetConnect(bool f) {
			connect=1;
		}
		/**
		 * 生成图
		*/
		void GenGraph(){
			edge.clear();
			e.clear();
			int max=node-1+(node-1)/2;
			if(side<node-1){
				side=node-1;
			}
			if(side>max){
				side=max;
			}
			int m=side;
			m-=node-1;
			std::vector<std::vector<int>> cycle;
			std::vector<int> p(node);
			for(int i=0;i<node;i++){
				p[i]=i;
			}
			shuffle(p.begin(),p.end());
			for(int i=2;i<=2*m;i+=2){
				std::vector<int> pre;
				if(i==2){
					pre.push_back(p[0]);
				}
				pre.push_back(p[i]);
				pre.push_back(p[i-1]);
				cycle.push_back(pre);
			}
			int up=node-(2*m+1);
			int add=rnd.next(0,up);
			int len=cycle.size();
			if(len==0){
				add=0;
			}
			for(int i=2*m+1;i<=2*m+add;i++){
				int w=rnd.next(len);
				cycle[w].push_back(p[i]);
			}
			for(int i=2*m+add+1;i<node;i++){
				cycle.push_back({p[i]});
			}
			shuffle(cycle.begin()+1,cycle.end());
			for(int i=0;i<cycle.size();i++){
				CycleGraph c;
				std::vector<int> pre=cycle[i];
				if(i!=0){
					int w=rnd.next(i);
					pre.push_back(rnd.any(cycle[w]));
				}
				c.GenGraph(pre);
				std::vector<std::pair<int, int>> cycedge=c.GetEdge();
				edge.insert(edge.end(),cycedge.begin(),cycedge.end());
			}
			shuffle(edge.begin(),edge.end());
		}
};