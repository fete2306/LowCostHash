# LowCostHash

一个使用cpp实现的，基于四叉索引树构建的哈希表。

对于单个键x，预计需要log_4 x个节点，节点可复用。

单个节点大小约为5B(4节点索引+1值索引).

支持任意类型的键值对，可自动处理键类型不同引起的哈希冲突

## 文件说明

LowCostHash.cpp
包含类 LowCostHash的实现与部分测试案例。

### 类结构说明

DATA

容器类，一个利用实例化的模板方法来存储变量类型的类

包含一个void*与若干实例化的模板方法

Indexes

索引类，计算四进制索引，并保存键的信息

一个递归构造的多维动态表，支持自动扩容。

Value

值类，保存值(void*)与键(由Indexes传入)

可调用DATA的方法来处理哈希冲突

TableNode

模板类，支持自定义节点类型，可选uint8_t,uint16_t,uint32_t,uint64_t

包含四个节点索引p0,p1,p2,p3(pointer)和一个值索引value

Table

模板类，包含一个Table数组和一个Value数组

包含存入，取出以及扩容，收缩的方法

LowCostHash

外部容器类，包裹Table，主要调用Table内部函数

### 成员列表


#### DATA

##### `void* data`

存储指向目标对象的指针。

##### `const std::type_info* id`

保存变量类型的 type_info 指针，用于比较两个 DATA 的真实类型。

##### `std::function<bool(DATA*,DATA*)> _equal`

存储实例化后的比较函数，使 DATA 支持运行期类型判断与比较。

##### `bool equal(DATA* other)`

比较两个 DATA 是否相同。

若类型不同，自动返回 false。

##### `bool equal_template(DATA* self, DATA* other)`

模板方法，由外部实例化，用于存储类型专属的比较逻辑。

---

#### Indexes

##### `std::vector<int> data`

存储拆解后的四进制索引路径，每个元素 ∈ {0,1,2,3}。

##### `DATA* key`

保存键的副本，避免外部数据失效。

##### `template<typename k> size_t hash(const k& key)`

通用哈希函数，调用 `std::hash<k>`。

##### `void get_indexes(size_t key)`

对哈希值持续右移，每 2bit 得到 0~3 的索引，塞入 vector。

##### `template<typename K> Indexes(K key)`

构造函数：

* 保存 key 的副本
* 对 key 进行哈希
* 构建四进制索引路径

##### `void print()`

打印 key（地址）、路径长度与路径内容。

---

#### TableNode

##### `T p0, p1, p2, p3`

四个子节点索引，默认值为最大值表示“空”。

##### `T value`

值索引，指向 Table.Values 中的位置。默认也是最大值。

---

#### Table

##### `std::vector<TableNode<T>> Nodes`

四叉树的节点数组，0 号节点为根。

##### `std::vector<void*> Values`

存储所有值的数组，索引由节点的 `value` 指定。

##### `T NodesMAX`

节点索引可达到的最大值。

---

##### `Table()`

初始化根节点。

##### `template<typename goal> Table<goal>* up()`

将表扩容为更高位类型（如 uint8 → uint16）。

复制所有节点索引，并保留 Values。

##### `void shrink_to_fit()`

压缩 Nodes 和 Values 的容量。

---

##### `template<typename K> void save(Indexes<K> indexes, void* value)`

沿着四叉路径：

* 若子节点不存在则创建新的节点
* 路径结束后将 value 存入 Values
* 存储值索引到当前节点的 value 中

  若节点容量不足会由 LowCostHash 自动触发 up()。

---

##### `template<typename K, typename V> V* get(Indexes<K> indexes)`

沿路径查找并返回目标类型的指针。

---

##### `template<typename K> Indexes<K> get_indexes(K key)`

辅助方法：生成一个 Indexes 用于查找或存储。

---

#### LowCostHash

##### `void* data`

指向当前 Table（真实类型可能为 Table<uint8_t> 等）。

##### `uint8_t typeId`

表示当前使用的索引类型：

* 8 → uint8_t
* 16 → uint16_t
* 32 → uint32_t
* 64 → uint64_t

表可能在超限时自动升级类型。

---

##### `LowCostHash()`

默认构造 Table<uint8_t>。

---

##### `void shrink_to_fit()`

根据 typeId 调用具体 Table 的 shrink_to_fit。

---

##### `template<typename K> void save(Indexes<K> indexes, void* value)`

对外统一接口：

* 判断当前 Table 是否会溢出
* 若溢出 → 自动 up()
* 调用具体 Table 的 save()

---

##### `template<typename K, typename V> V* get(Indexes<K> indexes)`

统一 get 接口：返回 V*。

## 使用示例

```cpp
LowCostHash table=LowCostHash();//声明表类
Indexes indexes=Indexes(1101);//声明一个索引类
std::string value="love";//声明一个值
table.save(indexes,&value);//存入值
indexes.print();//输出索引类
std::string str= table.get(std::string(indexes);//通过索引获取值
table.shrink_to_fit();//收缩vector的剩余空间
```

## 代价模型

单个键x会创建log_4 x个节点，全部都可被复用，作为其他键的路径点/叶子节点

每个节点占据5B内存（实际上常常被对齐到8B），理论上是5log_4 x，但实际上是 8log_4 x甚至更糟糕

理论性能尚可，实际不具备应用价值。目前没有想到怎么解决

## 设计思路

灵感来自于数值分析课程上对哈希表与键值对的思考

最初的灵感是用python实现的，十进制索引，多维嵌套列表。

之后扩展至32维的二进制索引树

随后尝试转cpp并完成四叉树和节点压缩

节点复用和实例化模板方法存储类型，都是在阅读材料时涌现出的思路

节点压缩则参考自python dict的索引数组(虽然我的方法要差很多)

## 许可说明

MIT License

Copyright (c) 2025 fete2306

Permission is hereby granted, free of charge, to any person obtaining a copy...

