# GraphGenerator
在使用前应熟悉了解"testlib.h"，采用了它的rnd作为随机数生成器。

对于树、图数据的生成，其中边的返回值均为```vector<pair<int,int>>```。

默认结点编号都是从1开始,可以通过修改```BEGIN_NODE```的值进行修改。

结点和边的数量限制都是10^6，可以分别通过```NODE_LIMIT```和```EDGE_LIMIT```进行修改。

类均在命名空间```Generator```中，可以在最开始加上```using namespace Generator;```。

在example中有各种示例。

(待更新)
