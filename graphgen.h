#include "testlib.h"

void FailGen(const char* msg, ...) {
    printf("\033[31mFAIL!\033[0m \n");
    va_list value;
    va_start(value, msg);
    vfprintf(stdout, msg, value);
    va_end(value);
    exit(EXIT_FAILURE);
}

namespace generator {

// 一颗随机生成的树,默认为无根
class Tree {
   protected:
    unsigned int node_limit = 1e6;  // 结点数的上限
    unsigned int edge_limit = 1e6;  // 边数的上限
    int begin_node = 1;             // 结点开始编号
    int node;                       // 结点数
    // 是否是有根树,默认为false,如果是有根树的话，边为(father,son);否则边为(father,son)与(son,father)中的任意一个
    bool is_root;
    int root;                               // 根，默认为1,只在is_rooted=1的时候生效
    std::vector<std::pair<int, int>> edge;  // 边
    std::vector<int> p;
    void JudgeLimits() {
        if (node <= 0) {
            FailGen("node must be a positive integer.\n");
        }
        if (node > node_limit) {
            FailGen(
                "node must less than or equal to %d, or you can change the "
                "node_limit.\n",
                node_limit);
        }
        if (is_root && (root < 0 || root > node - 1)) {
            FailGen("restriction of the root is [%d,%d]\n", begin_node,
                    node - 1 + begin_node);
        }
    }
    void Init() {
        if (is_root) {
            root -= begin_node;
        }
        JudgeLimits();
        edge.clear();
        p = rnd.perm(node, 0);
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
        u += begin_node;
        v += begin_node;
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
     * @param rt 根,默认为0,范围应为[begin_node,n-1+begin_node],只在is_root=true时生效
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
     * 获取根，只在有根的时候有效
     * @return 根
     */
    int GetRoot() {
        if (is_root) {
            return root + begin_node;
        }
        FailGen("Unroot Tree,but ask for get root.\n");
        return 0;
    }
    /**
     * 获取边
     * @return 边
     */
    std::vector<std::pair<int, int>> GetEdge() { return edge; }
    /**
     * 设置点数上限
     */
    void SetNodeLimit(unsigned int n) {
        node_limit = n;
    }
    /**
     * 设置边数上限
     */
    void SetEdgeLimit(unsigned int m) {
        edge_limit = m;
    }
    /**
     * 设置第一个结点的编号
     */
    void SetBeginNode(int n) {
        begin_node = n;
    }
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
            FailGen("restriction of the height is [%d,%d].\n", node == 1 ? 1 : 2, node);
        }
        if ((node > 1 && height <= 1) || height < 1) {
            FailGen("restriction of the height is [%d,%d].\n", node == 1 ? 1 : 2, node);
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
    unsigned int node_limit = 1e6;          // 结点数的上限
    unsigned int edge_limit = 1e6;          // 边数的上限
    int begin_node = 1;                     // 结点开始编号
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
    bool JudgeSelfLoop(int u, int v) { return self_loop == false && u == v; }
    bool JudgeMultiplyEdge(int u, int v) {
        if (multiply_edge == true)
            return false;
        if (e[{u, v}])
            return true;
        if (direction == 0 && e[{v, u}])
            return true;
        return false;
    }
    void AddEdge(int u, int v) {
        if (multiply_edge == false) {
            e[{u, v}] = 1;
            if (direction == false) {
                e[{v, u}] = 1;
            }
        }
        u += begin_node;
        v += begin_node;
        if (direction == true) {
            edge.push_back({u, v});
        } else {
            if (rnd.next(2)) {
                edge.push_back({u, v});
            } else {
                edge.push_back({v, u});
            }
        }
    }
    virtual void JudgeUpper() {
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
    virtual void JudgeLimits() {
        if (side < 0) {
            FailGen("number of edges must be a non-negative integer.\n");
        }
        if (side > edge_limit) {
            FailGen(
                "number of edges must less than or equal to %d, or you can change "
                "the edge_limit.\n",
                edge_limit);
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
    void SetNode(int n) { node = n; }
    /**
     * 设置边数
     * @param m 边数
     */
    void SetSide(int m) { side = m; }
    /**
     * 设置有向/无向图
     * @param f 0：无向;1：有向;默认无向图
     */
    void SetDirection(bool f) { direction = f; }
    /** Refactor for C++
     * 设置能不能存在重边
     * @param f 0：无重边;1：有重边;默认无重边
     */
    void SetMultiplyEdge(bool f) { multiply_edge = f; }
    /**
     * 设置能不能存在自环
     * @param f 0：无自环;1：有自环;默认无自环
     */
    void SetSelfLoop(bool f) { self_loop = f; }
    /**
     * 设置保不保证一定连通
     * @param f 0：不保证一定连通;1：保证一定连通;默认不保证一定连通
     */
    void SetConnect(bool f) { connect = f; }
    /**
     * 获取边
     */
    std::vector<std::pair<int, int>> GetEdge() { return edge; }
    /**
     * 设置点数上限
     */
    void SetNodeLimit(unsigned int n) {
        node_limit = n;
    }
    /**
     * 设置边数上限
     */
    void SetEdgeLimit(unsigned int m) {
        edge_limit = m;
    }
    /**
     * 设置第一个结点的编号
     */
    void SetBeginNode(int n) {
        begin_node = n;
    }
    /**
     * 生成图
     */
    void GenGraph() {
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
/**
 * 二分图
 * @note 强制设direction=0,self_loop=0
 */
class BipartiteGraph : public Graph {
   private:
    int left, right;  // 左部，右部大小
    bool set_left;    // 是否要随机左部大小
    void RandLeft() {
        if (set_left == true) {
            right = node - left;
            if (left < 0 || right < 0) {
                FailGen("left part or right part must greater than or equal to 0.\n");
            }
            return;
        }
        int l = 0, r = node / 2, limit;
        if (multiply_edge == 0) {
            if (side > r * (node - r)) {
                FailGen("number of edges must less than or equal to %d.\n", r * (node - r));
            }
            while (l <= r) {
                int mid = (l + r) / 2;
                if (mid * (node - mid) < side) {
                    l = mid + 1;
                } else {
                    limit = r;
                    r = mid - 1;
                }
            }
        } else {
            limit = 1;
        }
        left = rnd.next(limit, node / 2);
        right = node - left;
    }
    virtual void JudgeUpper() {
        long long limit;
        if (!multiply_edge) {
            limit = (long long)left * (long long)right;
            if (side > limit) {
                FailGen("number of edges must less than or equal to %lld.\n",
                        limit);
            }
        }
    }

   public:
    /**
     * @param n 结点数
     * @param m 边数
     * @param l 左部大小，右部=点数-左部，不设采用随机值
     * @note 其余参数默认如下：
     * @note direction=0 无向图
     * @note multiply_edge=0 无重边
     * @note self_loop=0 无自环
     * @note connect=0 不保证连通
     */
    BipartiteGraph(int n = 1, int m = 0, int l = -1) {
        node = n;
        side = m;
        direction = 0;
        multiply_edge = 0;
        self_loop = 0;
        connect = 0;
        if (l != -1) {
            set_left = true;
            left = l;
        }
    }
    /**
     * 强制无向图，禁用函数
     */
    void SetDirection(bool f) = delete;
    /**
     * 强制无自环，禁用函数
     */
    void SetSelfLoop(bool f) = delete;
    /**
     * 设置左部大小
     * @param l 左部大小
     */
    void SetLeft(int l) {
        left = l;
        set_left = true;
    }
    /**
     * 生成图
     */
    void GenGraph() {
        edge.clear();
        e.clear();
        RandLeft();
        JudgeLimits();
        std::vector<int> part[2];
        std::vector<int> p = rnd.perm(node, 0);
        for (int i = 0; i < left; i++) {
            part[0].push_back(p[i]);
        }
        for (int i = left; i < node; i++) {
            part[1].push_back(p[i]);
        }
        int m = side;
        if (connect) {
            m -= node - 1;
            std::vector<int> degree[2];
            degree[0].resize(left, 1);
            degree[1].resize(right, 1);
            for (int i = left; i < node - 1; i++) {
                degree[0][rnd.next(left)]++;
            }
            for (int i = right; i < node - 1; i++) {
                degree[1][rnd.next(right)]++;
            }
            int f = 0, d[2] = {node - 1, node - 1};
            while (d[0] + d[1] > 0) {
                for (int i = 0; i < (f == 0 ? left : right); i++) {
                    if (degree[f][i] == 1) {
                        if (d[f] == 1) {
                            for (int j = 0; j < (f == 0 ? right : left); j++) {
                                if (degree[f ^ 1][j] == 1) {
                                    AddEdge(part[f][i], part[f ^ 1][j]);
                                    d[0]--;
                                    d[1]--;
                                    degree[f][i]--;
                                    degree[f ^ 1][j]--;
                                }
                            }
                        } else {
                            int j;
                            do {
                                j = rnd.next(f == 0 ? right : left);
                            } while (degree[f ^ 1][j] < 2);
                            AddEdge(part[f][i], part[f ^ 1][j]);
                            d[0]--;
                            d[1]--;
                            degree[f][i]--;
                            degree[f ^ 1][j]--;
                        }
                    }
                }
                f ^= 1;
            }
        }
        while (m--) {
            int u, v;
            do {
                u = rnd.any(part[0]);
                v = rnd.any(part[1]);
            } while (JudgeMultiplyEdge(u, v));
            AddEdge(u, v);
        }
        shuffle(edge.begin(), edge.end());
    }
};
/**
 * DAG
 * @note 强制设self_loop=0,direction=1
 */
class DAG : public Graph {
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
    DAG(int n = 1, int m = 0) {
        node = n;
        side = m;
        direction = 1;
        multiply_edge = 0;
        self_loop = 0;
        connect = 0;
    }
    /**
     * 强制无向图,禁用函数
     */
    void SetDirection(bool f) = delete;
    /**
     * 强制无自环,禁用函数
     */
    void SetSelfLoop(bool f) = delete;
    /**
     * 生成图
     */
    void GenGraph() {
        edge.clear();
        e.clear();
        JudgeLimits();
        std::vector<int> p(node);
        int m = side;
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        shuffle(p.begin(), p.end());
        if (connect) {
            m = std::max(0, m - (node - 1));
            for (int i = 1; i < node; i++) {
                int f = rnd.next(i);
                AddEdge(p[f], p[i]);
            }
        }
        while (m--) {
            int u, v;
            do {
                u = rnd.next(node);
                v = rnd.next(node);
                if (u > v) {
                    std::swap(u, v);
                }
            } while (JudgeSelfLoop(p[u], p[v]) || JudgeMultiplyEdge(p[u], p[v]));
            AddEdge(p[u], p[v]);
        }
    }
};
/**
 * 环图
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,side=n
 */
class CycleGraph : public Graph {
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
    CycleGraph(int n = 1) {
        node = n;
        side = n;
        direction = 0;
        multiply_edge = 0;
        self_loop = 0;
        connect = 1;
    }
    /**
     * 设置点数
     * @param n 节点数
     */
    void SetNode(int n) {
        node = n;
        side = n;
    }
    /**
     * 强制边数等于点数，禁用函数
     */
    void SetSide(int m) = delete;
    /**
     * 强制无重边，禁用函数
     */
    void SetMultiplyEdge(bool f) = delete;
    /**
     * 强制无自环，禁用函数
     */
    void SetSelfLoop(bool f) = delete;
    /**
     * 强制连通，禁用函数
     */
    void SetConnect(bool f) = delete;
    /**
     * 根据点集生成图
     * @param a 点集
     * @attention 如果结点集合有重复点则会产生错误
     */
    void GenGraph(std::vector<int> a) {
        edge.clear();
        e.clear();
        int n = a.size();
        for (int i = 1; i < n; i++) {
            AddEdge(a[i], a[i - 1]);
        }
        if (JudgeSelfLoop(a[0], a[n - 1]) == 0 && JudgeMultiplyEdge(a[0], a[n - 1]) == 0) {
            AddEdge(a[0], a[n - 1]);
        }
    }
    /**
     * 根据结点数生成图
     */
    void GenGraph() {
        std::vector<int> p(node);
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        shuffle(p.begin(), p.end());
        GenGraph(p);
        shuffle(edge.begin(), edge.end());
    }
};
/**
 * 轮图
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,side=2n-2
 */
class WheelGraph : public Graph {
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
    WheelGraph(int n = 1) {
        node = n;
        side = 2 * n - 2;
        direction = 0;
        multiply_edge = 0;
        self_loop = 0;
        connect = 1;
    }
    /**
     * 设置结点数
     * @param n 结点数
     */
    void SetNode(int n) {
        node = n;
        side = 2 * n - 2;
    }
    /**
     * 强制边数等于2*node-2，禁用函数
     */
    void SetSide(int m) = delete;
    /**
     * 强制无重边，禁用函数
     */
    void SetMultiplyEdge(bool f) = delete;
    /**
     * 强制无自环，禁用函数
     */
    void SetSelfLoop(bool f) = delete;
    /**
     * 强制连通，禁用函数
     */
    void SetConnect(bool f) = delete;
    /**
     * 根据指定支配点和其余点生成图
     * @param v 指定点
     * @param p 其余点
     * @attention 如果结点集合有重复点则会产生错误
     */
    void GenGraph(int v, std::vector<int> p) {
        edge.clear();
        e.clear();
        CycleGraph cycle;
        cycle.GenGraph(p);
        edge = cycle.GetEdge();
        for (int i = 0; i < p.size(); i++) {
            AddEdge(v, p[i]);
        }
        shuffle(edge.begin(), edge.end());
    }
    /**
     * 根据指定点生成图，支配点随机
     * @param p 点集，不可为空
     * @attention 如果结点集合有重复点则会产生错误
     */
    void GenGraph(std::vector<int> p) {
        shuffle(p.begin(), p.end());
        std::vector<int> pre(p.begin() + 1, p.end());
        GenGraph(p[0], pre);
    }
    /**
     * 根据点数生成图
     */
    void GenGraph() {
        std::vector<int> p(node);
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        GenGraph(p);
    }
};
/**
 * 网格图
 * @note 强制设connect=1,self_loop=0
 */
class GridGraph : public Graph {
   protected:
    long long CountSide(int x, int y) {
        long long xl = (long long)x;
        long long yl = (long long)y;
        long long sum = xl * (yl - 1) + yl * (xl - 1) - 2 * ((yl - node % yl) % yl);
        if (direction == 1) {
            sum *= 2;
        }
        return sum;
    }
    virtual void JudgeUpper() {
        long long limit = 0;
        if (!multiply_edge) {
            int x = sqrt(node), y = (node + x - 1) / x;
            limit = CountSide(x, y);
            if (direction) {
                limit *= 2;
            }
            if (side > limit) {
                FailGen("number of edges must less than or equal to %lld.\n",
                        limit);
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
     * @note connect=1 保证连通
     */
    GridGraph(int n = 1, int m = 0) {
        node = n;
        side = m;
        direction = 0;
        multiply_edge = 0;
        self_loop = 0;
        connect = 1;
    }
    /**
     * 强制无自环，禁用函数
     */
    void SetSelfLoop(bool f) = delete;
    /**
     * 强制连通，禁用函数
     */
    void SetConnect(bool f) = delete;
    /**
     * 生成图
     */
    void GenGraph() {
        edge.clear();
        e.clear();
        JudgeLimits();
        int m = side, column, row;
        std::vector<int> p(node);
        int d[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        if (multiply_edge == 0) {
            std::pair<int, int> max = {0, 0};
            std::vector<int> possible;
            for (int i = 1; i <= node; i++) {
                int x = i, y = (node + i - 1) / i;
                long long w = CountSide(y, x);
                if (direction == 1) {
                    w *= 2;
                }
                if (w > max.first) {
                    max = {w, i};
                }
                if (w >= side) {
                    possible.push_back(i);
                }
            }
            if (possible.size() == 0) {
                m = max.first;
                row = max.second;
                column = (node + row - 1) / row;
            } else {
                row = rnd.any(possible);
                column = (node + row - 1) / row;
                m = CountSide(column, row);
            }
        } else {
            row = rnd.next(1, node);
            column = (node + row - 1) / row;
        }
        m = std::max(0, m - (node - 1));
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        shuffle(p.begin(), p.end());
        for (int i = 0; i < row; i++) {
            for (int j = 1; j < column; j++) {
                int x = i * column + j, y = x - 1;
                if (x >= node) continue;
                if (i % 2 == 0) {
                    AddEdge(p[y], p[x]);
                } else {
                    AddEdge(p[x], p[y]);
                }
            }
            int x = i * column, y = (i + 1) * column;
            if (x < node && y < node) {
                AddEdge(p[x], p[y]);
            }
        }
        while (m--) {
            int pos, k, px, py, nxt;
            do {
                pos = rnd.next(node);
                k = rnd.next(4);
                px = pos / column + d[k][0];
                py = pos % column + d[k][1];
                nxt = px * column + py;
            } while (px < 0 || px >= row || py < 0 || py >= column || nxt >= node || JudgeMultiplyEdge(p[pos], p[nxt]));
            AddEdge(p[pos], p[nxt]);
        }
        shuffle(edge.begin(), edge.end());
    }
};
/**
 * 基环树
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=0,side=n
 */
class PseudoTree : public Graph {
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
    PseudoTree(int n = 3) {
        node = n;
        side = n;
        direction = 0;
        multiply_edge = 0;
        self_loop = 0;
        connect = 1;
    }
    /**
     * 设置点数
     * @param n 点数
     */
    void SetNode(int n) {
        node = n;
        side = node;
    }
    /**
     * 强制边数等于点数，禁用函数
     */
    void SetSide(int m) = delete;
    /**
     * 强制无向，禁用函数
     */
    void SetDirection(bool f) = delete;
    /**
     * 强制无重边，禁用函数
     */
    void SetMultiplyEdge(bool f) = delete;
    /**
     * 强制无自环，禁用函数
     */
    void SetSelfLoop(bool f) = delete;
    /**
     * 强制连通，禁用函数
     */
    void SetConnect(bool f) = delete;
    /**
     * 根据指定环大小生成图，如果不传入参数则随机环大小
     * @param size 环大小
     */
    void GenGraph(int size = -1) {
        edge.clear();
        e.clear();
        if (size == -1) {
            size = rnd.next(3, node);
        }
        if (size > node || size < 3) {
            FailGen("Cycle size must in [3 , %d], but found %d.\n", node, size);
        }
        size = std::min(size, node);
        std::vector<int> p(node);
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        shuffle(p.begin(), p.end());
        CycleGraph cycle;
        std::vector<int> pre(p.begin(), p.begin() + size);
        cycle.GenGraph(pre);
        edge = cycle.GetEdge();
        for (int i = size; i < node; i++) {
            int f = rnd.next(i);
            AddEdge(p[i], p[f]);
        }
        shuffle(edge.begin(), edge.end());
    }
};
/**
 * 基环内向树
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=1,side=n
 */
class PseudoInTree : public PseudoTree {
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
    PseudoInTree(int n = 3) {
        node = n;
        side = n;
        direction = 1;
        multiply_edge = 0;
        self_loop = 0;
        connect = 1;
    }
    /**
     * 强制有向，禁用函数
     */
    void SetDirection(bool f) = delete;
    /**
     * 根据指定环大小生成图，如果不传入参数则随机环大小
     * @param size 环大小
     */
    void GenGraph(int size = -1) {
        edge.clear();
        e.clear();
        if (size == -1) {
            size = rnd.next(3, node);
        }
        if (size > node || size < 3) {
            FailGen("Cycle size must in [3 , %d], but found %d.\n", node, size);
        }
        size = std::min(size, node);
        std::vector<int> p(node);
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        shuffle(p.begin(), p.end());
        CycleGraph cycle;
        cycle.SetDirection(true);
        std::vector<int> pre(p.begin(), p.begin() + size);
        cycle.GenGraph(pre);
        edge = cycle.GetEdge();
        for (int i = size; i < node; i++) {
            int f = rnd.next(i);
            AddEdge(p[i], p[f]);
        }
        shuffle(edge.begin(), edge.end());
    }
};
/**
 * 基环外向树
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=1,side=n
 */
class PseudoOutTree : public PseudoTree {
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
    PseudoOutTree(int n = 3) {
        node = n;
        side = n;
        direction = 1;
        multiply_edge = 0;
        self_loop = 0;
        connect = 1;
    }
    /**
     * 强制有向，禁用函数
     */
    void SetDirection(bool f) = delete;
    /**
     * 根据指定环大小生成图，如果不传入参数则随机环大小
     * @param size 环大小
     */
    void GenGraph(int size = -1) {
        edge.clear();
        e.clear();
        if (size == -1) {
            size = rnd.next(3, node);
        }
        if (size > node || size < 3) {
            FailGen("Cycle size must in [3 , %d], but found %d.\n", node, size);
        }
        size = std::min(size, node);
        std::vector<int> p(node);
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        shuffle(p.begin(), p.end());
        CycleGraph cycle;
        cycle.SetDirection(1);
        std::vector<int> pre(p.begin(), p.begin() + size);
        cycle.GenGraph(pre);
        edge = cycle.GetEdge();
        for (int i = size; i < node; i++) {
            int f = rnd.next(i);
            AddEdge(p[f], p[i]);
        }
        shuffle(edge.begin(), edge.end());
    }
};
/**
 * 仙人掌
 * @note 强制设self_loop=0,connect=1,multiply_edge=0,direction=0
 * @note 仙人掌的边数的范围应该是[n-1,n-1+(n-1)/2]
 */
class Cactus : public Graph {
   private:
    virtual void JudgeUpper() {
        int limit = node - 1 + (node - 1) / 2;
        if (side > limit) {
            FailGen("number of edges must less than or equal to %d.\n", limit);
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
     * @note connect=1 保证连通
     */
    Cactus(int n = 1, int m = 0) {
        node = n;
        side = m;
        direction = 0;
        multiply_edge = 0;
        self_loop = 0;
        connect = 1;
    }
    /**
     * 强制无向，禁用函数
     */
    void SetDirection(bool f) = delete;
    /**
     * 强制有无重边，禁用函数
     */
    void SetMultiplyEdge(bool f) = delete;
    /**
     * 强制无自环，禁用函数
     */
    void SetSelfLoop(bool f) = delete;
    /**
     * 强制连通，禁用函数
     */
    void SetConnect(bool f) = delete;
    /**
     * 生成图
     */
    void GenGraph() {
        edge.clear();
        e.clear();
        JudgeLimits();
        int m = side;
        m -= node - 1;
        std::vector<std::vector<int>> cycle;
        std::vector<int> p(node);
        for (int i = 0; i < node; i++) {
            p[i] = i;
        }
        shuffle(p.begin(), p.end());
        for (int i = 2; i <= 2 * m; i += 2) {
            std::vector<int> pre;
            if (i == 2) {
                pre.push_back(p[0]);
            }
            pre.push_back(p[i]);
            pre.push_back(p[i - 1]);
            cycle.push_back(pre);
        }
        int up = node - (2 * m + 1);
        int add = rnd.next(0, up);
        int len = cycle.size();
        if (len == 0) {
            add = 0;
        }
        for (int i = 2 * m + 1; i <= 2 * m + add; i++) {
            int w = rnd.next(len);
            cycle[w].push_back(p[i]);
        }
        for (int i = 2 * m + add + 1; i < node; i++) {
            cycle.push_back({p[i]});
        }
        shuffle(cycle.begin() + 1, cycle.end());
        for (int i = 0; i < cycle.size(); i++) {
            CycleGraph c;
            std::vector<int> pre = cycle[i];
            if (i != 0) {
                int w = rnd.next(i);
                pre.push_back(rnd.any(cycle[w]));
            }
            c.GenGraph(pre);
            std::vector<std::pair<int, int>> cycedge = c.GetEdge();
            edge.insert(edge.end(), cycedge.begin(), cycedge.end());
        }
        shuffle(edge.begin(), edge.end());
    }
};
}  // namespace generator
