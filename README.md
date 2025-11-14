# 🟦 README.md

# QuadHash – A Low-Cost Quadtree-Based Hash Table

# QuadHash —— 基于四叉树结构的低成本哈希表

---

## 📌 Overview 概述

**QuadHash** is a hash table implemented using a  **quadtree-based index structure** , designed to minimize memory overhead and maximize cache locality.

QuadHash 通过将键的哈希值转为  **四进制路径** ，构建一棵  **四叉索引树** ，在极度稀疏的场景下依旧保持极低的空间成本，同时支持指数级节点扩容。

---

## 🚀 Features 特性

* **4-way quadtree index structure（四叉树索引结构）**
* **Compact memory usage（极低空间占用）**
* **Auto-scaling node index type: uint8 → uint16 → uint32 → uint64（自动扩容）**
* **Cache-friendly contiguous node array（缓存友好）**
* **User-defined value pointers（支持任意类型值指针）**
* **Shrinkable storage（可手动收缩 vector 空间）**

---

## 📂 File Structure 文件结构

<pre class="overflow-visible!" data-start="1504" data-end="1626"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>QuadHash.cpp         </span><span># 主实现（原 LowCostHashPro1.cpp）</span><span>
QuadHashBase.cpp     </span><span># 基础版本（原 LowCostHash.cpp）</span><span>
LICENSE
README.md
</span></span></code></div></div></pre>

---

## 📘 How It Works 工作原理

### 1. Convert key hash → base-4 index sequence

将哈希值转为  **四进制序列** ：

<pre class="overflow-visible!" data-start="1727" data-end="1798"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>key → </span><span>hash</span><span> → (digit_0, digit_1, digit_2, ...)
digit ∈ {0,1,2,3}
</span></span></code></div></div></pre>

Each digit indexes one branch of a quadtree node.

每一位代表四叉树节点的一个分支。

---

### 2. Node Structure 节点结构

每个节点包含  **4 个子节点索引 + 1 个值索引** ：

<pre class="overflow-visible!" data-start="1934" data-end="2055"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre! language-cpp"><span><span>class</span><span></span><span>TableNode</span><span><T> {
    T p0, p1, p2, p3; </span><span>// 4-way children</span><span>
    T value;          </span><span>// index of stored value</span><span>
};
</span></span></code></div></div></pre>

This yields excellent cache locality by storing all nodes in one large vector.

节点被连续存储于一块 vector 中，具备高度缓存友好性。

---

### 3. Automatic Type Expansion 自动扩容

当节点索引接近上限时：

<pre class="overflow-visible!" data-start="2226" data-end="2267"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>uint8</span><span>  → </span><span>uint16</span><span> → </span><span>uint32</span><span> → </span><span>uint64</span><span>
</span></span></code></div></div></pre>

对应代码使用 `Table<T>::up<Next>()` 进行整体复制并升级。

---

## 📈 Space Model 空间模型

对于键 `x`，四叉树最大节点数（极度稀疏）为：

<pre class="overflow-visible!" data-start="2366" data-end="2385"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>N</span><span> = log₄(x)
</span></span></code></div></div></pre>

节点空间：

<pre class="overflow-visible!" data-start="2394" data-end="2459"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>5N </span><span>bytes</span><span></span><span>(each node = 4</span><span> child pointers + </span><span>1</span><span> value pointer)
</span></span></code></div></div></pre>

相比 N 叉树的空间模型：

<pre class="overflow-visible!" data-start="2476" data-end="2497"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>(N+</span><span>1</span><span>) log_N x
</span></span></code></div></div></pre>

可知  **N = 4 时空间最优** 。

---

## 🧩 Design Motivation 设计动机

本项目源自课程讨论中对传统哈希表空间浪费问题的思考：

* 普通哈希表在极度稀疏场景下浪费大量空间
* 观察键的特征后，发现“逐位路径索引”更高效
* 最初采用十进制索引 + 列表
* 最终演化为 **四进制 + 四叉树结构**

---

## 🔧 Example Usage 使用示例

<pre class="overflow-visible!" data-start="2709" data-end="2888"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre! language-cpp"><span><span>LowCostHash table;
Indexs indexs = </span><span>Indexs</span><span>(</span><span>1101</span><span>);
std::string value = </span><span>"hello"</span><span>;

table.</span><span>save</span><span>(indexs, &value);

std::cout << *(table.</span><span>get</span><span><std::string>(indexs)) << std::endl;
</span></span></code></div></div></pre>

---

## 🛠 Class Overview 类结构说明

### `Indexs`

* Stores base-4 index sequence from hash
* 获取四进制路径索引

### `TableNode<T>`

* Single quadtree node
* 四叉树节点（p0/p1/p2/p3/value）

### `Table<T>`

* Holds all nodes
* Maintains value array
* Supports type expansion
* 节点存储、值存储、自动扩容

### `LowCostHash`

* High-level wrapper
* 自动选择 8/16/32/64 bit 索引

---

## 📄 License 许可

This project is under the MIT License.

本项目采用 MIT 许可证。

