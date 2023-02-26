#include "testlib.h"

void FailGen(const char* msg, ...) {
  printf("\033[31mFAIL!\033[0m \n");
  va_list value;
  va_start(value, msg);
  vfprintf(stdout, msg, value);
  va_end(value);
  exit(EXIT_FAILURE);
}

namespace Generator {

unsigned int NODE_LIMIT = 1e6;  // 结点数的上限
unsigned int EDGE_LIMIT = 1e6;  // 边数的上限
unsigned int BEGIN_NODE = 1;    // 结点开始编号

// 一颗随机生成的树,默认为无根
class Tree {
 protected:
  int node;  // 结点数
  // 是否是有根树,默认为false,如果是有根树的话，边为(father,son);否则边为(father,son)与(son,father)中的任意一个
  bool is_root;
  int root;  // 根，默认为1,只在is_rooted=1的时候生效
  std::vector<std::pair<int, int>> edge;  // 边
  std::vector<int> p;
  void JudgeLimits() {
    if (node <= 0) {
      FailGen("node must be a positive integer.\n");
    }
    if (node > NODE_LIMIT) {
      FailGen(
          "node must less than or equal to %d, or you can change the "
          "NODE_LIMIT.\n",
          NODE_LIMIT);
    }
    if (is_root && (root < 0 || root > node - 1)) {
      FailGen("restriction of the root is [%d,%d]\n", BEGIN_NODE,
              node - 1 + BEGIN_NODE);
    }
  }
  void Init() {
    if (is_root) {
      root -= BEGIN_NODE;
    }
    JudgeLimits();
    edge.clear();
    p.resize(node);
    for (int i = 0; i < node; i++) {
      p[i] = i;
    }
    shuffle(p.begin(), p.end());
    if (is_root) {
      for (int i = 0; i < node; i++) {
        if (p[i] == root) {
          std::swap(p[0], p[i]);
          break;
        }
      }
    }
  }
  void AddEdge(int u, int v) {
		u+=BEGIN_NODE;
		v+=BEGIN_NODE;
    if (is_root || rnd.next(2)) {
      edge.push_back({u, v});
    } else {
      edge.push_back({v, u});
    }
  }

 public:
  /**
   * 默认为无根树
   * @param n 结点数
   * @param is_rt 是否有根
   * @param rt 根,默认为0,范围应为[0,n-1],只在is_root=true时生效
   */
  Tree(int n = 1, bool is_rt = 0, int rt = 0) {
    node = n;
    is_root = is_rt;
    root = rt;
  }
  /**
   * 设置结点数
   * @param n 结点数
   */
  void SetNode(int n) { node = n; }
  /**
   * 设置无根/有根树,如果是有根树的话，边为(father,son);否则边为(father,son)与(son,father)中的任意一个
   * @param is_rt 是否有根
   */
  void SetIsRoot(bool is_rt) { is_root = is_rt; }
  /**
   * 设置根，只在有根树的时候生效
   * @param rt 根,范围应为[0,n-1]
   */
  void SetRoot(int rt) { root = rt; }
  /**
   * 获取边
   * @return 边
   */
  std::vector<std::pair<int, int>> GetEdge() { return edge; }
  /**
   * 生成树
   */
  void GenTree() {
    Init();
    for (int i = 1; i < node; i++) {
      int f = rnd.next(i);
      AddEdge(p[f], p[i]);
    }
    shuffle(edge.begin(), edge.end());
  }
};
// 一条链，默认为无根树
class Chain : public Tree {
 public:
  /**
   * 一条链，默认为无根树
   * @param n 结点数
   * @param is_rt 是否有根
   * @param rt 根,默认为0,范围应为[0,n-1],只在is_root=true时生效
   */
  Chain(int n = 1, bool is_rt = 0, int rt = 0) {
    node = n;
    is_root = is_rt;
    root = rt;
  }
  /**
   * 生成树
   */
  void GenTree() {
    Init();
    for (int i = 1; i < node; i++) {
      AddEdge(p[i - 1], p[i]);
    }
    shuffle(edge.begin(), edge.end());
  }
};
// 一个菊花图，默认为无根树
class Flower : public Tree {
 public:
  /**
   * 一个菊花图，默认为无根树
   * @param n 结点数
   * @param is_rt 是否有根
   * @param rt 根,默认为0,范围应为[0,n-1],只在is_root=true时生效
   */
  Flower(int n = 1, bool is_rt = 0, int rt = 0) {
    node = n;
    is_root = is_rt;
    root = rt;
  }
  /**
   * 生成树
   */
  void GenTree() {
    Init();
    for (int i = 1; i < node; i++) {
      AddEdge(p[0], p[i]);
    }
    shuffle(edge.begin(), edge.end());
  }
};
// 一颗限制高度的树，必须有根
class HeightTree : public Tree {
 private:
  int height;       // 树的高度
  bool set_height;  // 判断是否设置树高
 public:
  /**
   * 一个限制高度的树
   * @param n 结点数
   * @param is_rt 是否有根，必须有根
   * @param h 树高度,不指定的话在生成的时候随机高度
   * @param rt 根,默认为0,范围应为[0,n-1],只在is_root=true时生效
   */
  HeightTree(int n = 1, int h = -1, int rt = 0) {
    node = n;
    is_root = 1;
    root = rt;
    if (h != -1) {
      SetHeight(h);
    }
  }
  /**
   * 设置树的高度
   * @param h 树高
   */
  void SetHeight(int h) {
    height = h;
    set_height = 1;
  }
  /**
   * 设置树的高度为随机生成
   */
  void ResetHeight() { set_height = 0; }
  /**
   * 强制为有根树，函数不可调用
   * @param is_rt 是否有根
   */
  void SetIsRoot(bool is_rt) = delete;
  /**
   * 生成树
   */
  void GenTree() {
    if (!set_height) {
      SetHeight(rnd.next(node == 1 ? 1 : 2, node));
    }
    if (height > node) {
      FailGen("restriction of the height is [%d,%d].\n", node == 1 ? 1 : 2,node);
    }
    if ((node > 1 && height <= 1) || height < 1) {
      FailGen("restriction of the height is [%d,%d].\n", node == 1 ? 1 : 2,node);
    }
    Init();
    std::vector<int> number(height, 1);
    int w = node - height;
    for (int i = 1; i <= w; i++) {
      number[rnd.next(1, height - 1)]++;
    }
    int l = 0, r = 0, k = 0;
    for (int i = 1; i < node; i++) {
      if (r + number[k] == i) {
        l = r;
        r += number[k];
        k++;
      }
      int f = rnd.next(l, r - 1);
      AddEdge(p[f], p[i]);
    }
    shuffle(edge.begin(), edge.end());
  }
};
// 普通图
class Graph {
 protected:
  int node, side;                         // 点数，边数
  std::vector<std::pair<int, int>> edge;  // 边
  std::map<std::pair<int, int>, bool> e;  // 边的去重
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
  bool JudgeSelfLoop(int u, int v) { return self_loop == 0 && u == v; }
  bool JudgeMultiplyEdge(int u, int v) {
    if (multiply_edge == 1)
      return false;
    if (e[{u, v}])
      return true;
    if (direction == 0 && e[{v, u}])
      return true;
    return false;
  }
  void AddEdge(int u, int v) {
		u+=BEGIN_NODE;
		v+=BEGIN_NODE;
    if (direction == 1) {
      edge.push_back({u, v});
    } else {
      if (rnd.next(2)) {
        edge.push_back({u, v});
      } else {
        edge.push_back({v, u});
      }
    }
    if (multiply_edge) {
      return;
    }
    e[{u, v}] = 1;
    if (direction == 0) {
      e[{v, u}] = 1;
    }
  }
	void JudgeUpper(){
		long long limit;
    if (!multiply_edge) {
      limit = (long long)node * (long long)(node - 1) / 2;
      if (direction) {
        limit *= 2;
      }
      if (self_loop) {
        limit += node;
      }
      if (side > limit) {
        FailGen("number of edges must less than or equal to %lld.\n",
                limit);
      }
    }
	}
  void JudgeLimits() {
		if(side<0){
			FailGen("number of edges must be a non-negative integer.\n");
		}
    if (side > EDGE_LIMIT) {
      FailGen(
          "number of edges must less than or equal to %d, or you can change "
          "the EDGE_LIMIT.\n");
    }
		JudgeUpper();
    if (connect) {
      if (side < node - 1) {
        FailGen("number of edges must greater than or equal to %d.\n",
                node - 1);
      }
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
  Graph(int n = 1, int m = 0) {
    node = n;
    side = m;
    direction = 0;
    multiply_edge = 0;
    self_loop = 0;
    connect = 0;
  }
  /**
   * 设置结点数
   * @param n 结点数
   */
  virtual void SetNode(int n) { node = n; }
  /**
   * 设置边数
   * @param m 边数
   */
  virtual void SetSide(int m) { side = m; }
  /**
   * 设置有向/无向图
   * @param f 0：无向;1：有向;默认无向图
   */
  virtual void SetDirection(bool f) { direction = f; }
  /**
   * 设置能不能存在重边
   * @param f 0：无重边;1：有重边;默认无重边
   */
  virtual void SetMultiplyEdge(bool f) { multiply_edge = f; }
  /**
   * 设置能不能存在自环
   * @param f 0：无自环;1：有自环;默认无自环
   */
  virtual void SetSelfLoop(bool f) { self_loop = f; }
  /**
   * 设置保不保证一定连通
   * @param f 0：不保证一定连通;1：保证一定连通;默认不保证一定连通
   */
  virtual void SetConnect(bool f) { connect = f; }
  /**
   * 获取边
   */
  std::vector<std::pair<int, int>> GetEdge() { return edge; }
  /**
   * 生成图
   */
  virtual void GenGraph() {
    edge.clear();
    e.clear();
    JudgeLimits();
    int m = side;
    if (connect) {
      m -= node - 1;
      Tree tree(node);
      tree.GenTree();
      edge = tree.GetEdge();
      if (!multiply_edge) {
        for (auto x : edge) {
          e[x] = 1;
        }
      }
    }
    while (m--) {
      int u, v;
      do {
        u = rnd.next(node);
        v = rnd.next(node);
      } while (JudgeSelfLoop(u, v) || JudgeMultiplyEdge(u, v));
      AddEdge(u, v);
    }
    shuffle(edge.begin(), edge.end());
  }
};
}  // namespace Generator