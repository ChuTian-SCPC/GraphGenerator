# GraphGenerator
在使用前应熟悉了解"testlib.h"。

对于树、图数据的生成，其中边的返回值均为```vector<pair<int,int>>```，树中父亲结点的返回值为```vector<int>```。

默认结点编号都是从0开始。

最开始加上```using namespace Generator;```

### Tree

比较随机的树的生成方式，并不是按照完全随机（purfer序列）生成的。

生成方案是：随机一个排列，从第2个点到第n个点随机坐标比它小的点作为父亲。

其中有一个偏移量shift，设置为0，与树的形态相关，可以自行设置。偏移量相关的代码为```f=rnd.wnext(i,shift)```。

使用的方法是：

```cpp
//设置树的结点数为10
Tree tree;
tree.SetNode(10);
//或者
Tree tree(10);

//生成树及获取边
tree.GenTree();
vector<pait<int,int>> e=tree.GetEdge();
```
### UnrootTree

无根树

### UnrootChain

无根树，会生成一条链。

其shift值为100000

### UnrootFlower

无根树，会生成一朵菊花

其shift值为-100000

### RootTree

有根树，在原本的基础上增加了一个获取每个结点父亲数组的函数。

其中根节点默认为0，根节点的父亲的表示默认为-1。

```cpp
vector<int> fa=tree.GetFather();
```
### RootChain

有根树，会生成一条链

### RootFlower

有根树，会生成一朵菊花

### Graph

根据给定的点数和边数生成一个随机图。可以设置存不存在重边，存不存在自环，有没有向，保不保证连通。

其中连通对于有向图来说是弱连通。

边数在一些情况下会有上下限，如果给的边数低于下限或者高于上限，就会强制变成上下限。

```cpp
//设置图的结点数是10，边数是20
Graph graph;
graph.SetNode(10);
graph.SetSide(20);
//或者
Graph graph(10);
graph.SetSide(20);
//或者
Graph graph(10,20);

//设置图的属性
graph.SetSelfLoop(true);//可以有自环
graph.SetMultiplyEdge(true);//可以有重边
graph.SetDirection(true);//有向图
graph.SetConnect(true);//保证连通

//生成树及获取边
fraph.GenGraph();
vector<pait<int,int>> e=tree.GetEdge();
```
### BipartiteGraph

二分图。

点集可以被分成两个部分，并且两个部分的内部点之间都没有边。

所以如果无重边的话边数最大为(n/2)\*(n+1)/2。

构造方法：随机将一个排列分成左部和右部，每次连边分别选择左右部各一个点

### DAG

有向无环图

构造方法：随机一个排列，随机边的时候排列中左边较小的点指向排列中坐标较大的点。

### CycleGraph

环图

构造方法：随机一个排列，然后一个跟一个连起来，最后头尾相连。

### WheelGraph

轮图

构造方法：随机一个排列，将除了第一个点之外的点构造一个环图，然后将第一个点与这些点相连。

### GridGraph

网格图

对于无重边来说：

构造的类似网格状，所以设行为row，列为column。

枚举column,row=(node+column-1)/column。

一个row\*column<=n的网格图最多的边数为row\*(column-1)+column\*(row-1)-2\*((column-node%column)%column)。

对于可以选取的column随机选取一个。

将一维数组转换为二维数组去看，每个点只能向上下左右四个方向连边。

### PseudoTree

基环树

基环树是指n个点n条边的图，其中存在一个环。

可以指定环的大小进行构造。没有的话就随机一个大小。

构造方法：随机一个排列，先按照环的个数把前一部分构造一个轮图，然后随机把后面的点加到前面的点上。

### PseudoInTree

基环内向树，每个点的出度都为1。

构造方法和基环树相同，只不过指定了方向。

### PseudoOutTree

基环外向树，每个点的出度都为1。

构造方法和基环树相同，只不过指定了方向。

### Cactus

仙人掌

一张无向连通图的每条边最多在一个环内。

可以得到它的边数范围为[n-1,n-1+(n-1)/2]。

构造方法：

这个图里一共有k=side-(node-1)个环，最少需要2\*k+1个点。

我们将这些点按照3,2,2……的放入k个集合中。

对于剩下的点，随机有多少个点属于环中，剩下的x个则为独立点。将属于环中的点随机分配给k个环。

一共有x+k个点集，将从第2个到第x+k个点集打乱。

对于每个点集，如果不是第一个点集，则从前面的点集中随机选取一个点，然后一起构成一个环图。
