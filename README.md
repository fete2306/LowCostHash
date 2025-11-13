# LowCostHash

一个使用 cpp 实现的 四叉哈希树。

对于键x，需要创建的最大节点数目N为log_4 x(极度稀疏情况下),占据空间为5N*1B。

(事实上节点可以高度复用)

在节点数目超过1B容量后，会依次扩展至2B,4B,8B,16B；

节点索引与值索引同步扩增以满足需求
## 📂 文件说明

### ⚙️ 类结构说明
Index 索引类

Table 表类

TableNode 节点类

...

其他节点内容正在开发

## 📈 代价模型

对于N叉树而言，这种结构的空间大小为(N+1)log_N x,易知N=4时占用空间最小。

节点单独存储在一块连续内存中，缓存友好，可手动回收vector浪费的空间。

## 🧩 设计思路
原理是将键转换成四进制，每个节点存储4个节点索引和1个值索引。

源自上课时对哈希表空间浪费的担忧与对键的观察。最初采用的是十进制索引和列表，最后采用四进制编码和索引树结构
## 📜 许可说明

MIT License

Copyright (c) 2025 fete2306

Permission is hereby granted, free of charge, to any person obtaining a copy...
