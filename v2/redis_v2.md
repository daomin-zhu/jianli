#### redis常见面试题
内存模型
```
used_memory:
    Redis分配器分配的内存总量,包括使用的虚拟内存;
used_memory_human:
    只是显示更友好。
used_memory_rss:
    Redis进程占据操作系统的内存,与top及ps命令看到的值是一致的
used_memory_rss:
    包括进程运行本身需要的内存、内存碎片等,但是不包括虚拟内存。
mem_fragmentation_ratio:
    内存碎片比率,该值是used_memory_rss/used_memory的比值
mem_allocator:
    Redis使用的内存分配器,在编译时指定
    可以是 libc,jemalloc或者tcmalloc,默认是jemalloc
```
redis 内存淘汰策略
```
1. noeviction 不删除策略,达到最大内存限制,需要内存直接返回错误;
2. allkeys-lru: 优先删除最近最少使用的key
3. volatite-lru: 只限制expire部分;优先删除最近最少使用的key
4. allkeys-random: 所有可用通用;随机删除一部分key
5. volatile-random: 只限制expire部分,随机删除一部分key
6. volatile-ttl: 只限制expire部分;优先删除剩余时间短的key
```
redis 高性能原因
```
1. 完全基于内存,绝大部分请求都是纯粹的内存操作
2. 数据结构简单,数据结构是专门进行设计的
3. 采用单线程,避免了不必要的上下文和竞争条件,不用考虑锁
4. 使用IO多路复用
5. 自己构建了VM机制
```
redis 持久化
```
AOF:
    定义: 通过记录redis服务器所执行的写命令日志进行持久化,
          恢复数据时执行保存的写命令进行数据恢复
    优点是: 支持秒级持久化,兼容性好
    缺点是: 文件大,恢复速度满,对性能影响比较大
RDB:
    定义: 快照snapshot持久化方式,把当前内存中的数据集快照写入硬盘
          恢复数据时直接读取硬盘上的快照文件
    优点: 文件紧凑,体积小,网络传输快,适合全量复制
    缺点: 不能做到实时持久化,已丢失;
          需要满足特定格式,兼容性差
持久化的原因:
    redis是内存数据库,如果不持久化,系统宕机之后数据会丢失;
    为了使关机也能保存数据,必须通过持久化的方式将数据从内存保存到磁盘
```
redis并发竞争
```
多个客户端使用setnx实现锁
```

分布式环境下缓存与数据库间一致性
```
如果需要强一致性不建议使用缓存;
更新数据库后及时更新缓存,更新成功后删除key
缓存失败时增加重试机制
```
缓存雪崩
```
定义:
    设置缓存时采用了相同的过期时间,导致缓存在某一时刻同时失效，
    请求全部转发到DB,DB瞬时压力过重崩溃
解决方案:
    1.考虑用加锁或者队列的方式保证缓存的单线程写，
    从而避免失效时大量的并发请求落到底层存储系统上。
    2.将缓存失效时间分散开,可以在原有的失效时间基础上增加一个随机值，
```
缓存穿透
```
定义:
    查询一个一定不存在的数据,没有缓存,每次都会请求DB,失去了缓存的意义
    在流量大时,可能DB就挂掉了,要是有人利用不存在的key频繁攻击我们的应用
    这就是漏洞
解决方案:
    1. 最常见的则是采用布隆过滤器，将所有可能存在的数据哈希到一个足够大的bitmap中，
        一个一定不存在的数据会被这个bitmap拦截掉，从而避免了对底层存储系统的查询压力。
    2. 如果一个查询返回的数据为空，我们仍然把这个空结果进行缓存.
        但它的过期时间会很短，最长不超过五分钟。
```
缓存击穿
```
定义:
    缓存在某个时间点过期的时候,恰好在这个时间点对这个Key有大量的并发请求过来，
    这些请求发现缓存过期一般都会从后端DB加载数据并回射到缓存，
    这个时候大并发的请求可能会瞬间把后端DB压垮。
解决方案:
    1.使用互斥锁(mutex key)
        在缓存失效的时候,不是立即去load db，
        而是先使用缓存工具的某些带成功操作返回值的操作,
        去set一个mutex key，当操作返回成功时，再进行load db的操作并回设缓存；
        否则，就重试整个get缓存的方法。
        SETNX，是「SET if Not eXists」的缩写，也就是只有不存在的时候才设置，
        可以利用它来实现锁的效果。
2、 永不过期
```
redis性能
```
1.Master最好不要做任何持久化工作;恢复不要恢复master,直接将一个slave切换为master;
2.策略设置为每秒同步一次为了主从复制的速度和连接的稳定性
3.Master和Slave最好在同一个局域网内尽量避免在压力很大的主库上增加从库
```
分布式锁
```
不可重入锁:
    只判断这个锁有没有被锁上，只要被锁上申请锁的线程都会被要求等待。实现简单
可重入锁:
    不仅判断锁有没有被锁上，还会判断锁是谁锁上的，当就是自己锁上的时候，
    那么他依旧可以再次访问临界资源，并把加锁次数加一。
一个可靠的分布式锁应该具备以下特性：
　　1.互斥性：作为锁，需要保证任何时刻只能有一个客户端(用户)持有锁
　　2.可重入： 同一个客户端在获得锁后，可以再次进行加锁
　　3.高可用：获取锁和释放锁的效率较高，不会出现单点故障
　　4.自动重试机制：当客户端加锁失败时，能够提供一种机制让客户端自动重试
通过setnx 来实现 
```
 如果在setnx之后执行expire之前进程意外crash或者要重启维护了，那会怎么样？
 ```
    set指令有非常复杂的参数，这个应该是可以同时把setnx和expire合成一条指令来用的！
 ```
Redis主从同步的实现原理和过程、产生数据丢失的原因
```
数据丢失的场景：
1、 程序bug或人为误操作。
2、 因客户端缓冲区内存使用过大，导致大量键被LRU淘汰。
3、 主库故障后自动重启，可能导致数据丢失。
4、 网络分区的问题，可能导致短时间的写入数据丢失。
5、 主从复制数据不一致，发生故障切换后，出现数据丢失。
6、 大量过期键，同时被淘汰清理。
```
redis定时机制
```
Redis的定时机制就是借助时间事件实现的
一个时间事件主要由以下三个属性组成：
id: 时间事件标识号
when:   记录时间事件的到达时间
timeProc:   时间事件处理器，当时间事件到达时，服务器就会调用相应的处理器来处理时间。
```
redis rehash机制
```
渐进式hash
1. 当负载因子>1时,开辟一个2倍空间的桶
2. 在扩容过程中,每次操作都进行一次迁移
   将扩容操作分散在每一次操作之中
3. 迁移完毕,删除原桶

```
54.定时删除
```
优点:
    对内存友好，定时删除策略可以保证过期键会尽可能快地被删除，并释放国期间所占用的内存
缺点:
    对cpu时间不友好,在过期键比较多时,删除任务会占用很大一部分cpu时间,
    在内存不紧张但cpu时间紧张的情况下,将cpu时间用在删除和当前任务无关的过期键上,
    影响服务器的响应时间和吞吐量
```
56.惰性删除
```
优点:
    对cpu时间友好,在每次从键空间获取键时进行过期键检查并是否删除.
    删除目标也仅限当前处理的键,这个策略不会在其他无关的删除任务上花费任何cpu时间。
缺点:
    对内存不友好,过期键过期也可能不会被删除,导致所占的内存也不会释放;
    甚至可能会出现内存泄露的现象,当存在很多过期键,而这些过期键又没有被访问到;
    这会可能导致它们会一直保存在内存中,造成内存泄露
```
redis 高可用架构
```
主从模式:
    特点:
        主数据库进行读写操作,变化的数据会自动将数据同步给从数据库;
        从数据库一般只读,并接收主数据库同步的数据;
        一个master可以拥有多个slave,一个slave只能对应一个master;
        slave重启后会同步主数据库的数据
    工作机制:
        slave发送SYNC到master,master进行RDB和AOF备份;
        AOF保存的时RDB备份期间的命令;先传送RDB文件,再传输AOF命令
        初始化之后,master每次的命令都会发送到slave
        以保证主从数据一致性
    缺点:
        master挂掉,redis无法对外提供写服务;
        slave切换成master需要手工介入,选择一个slave作为master;
        如果slave重启的话,会同步master的空数据;
哨兵模式:
    特点:
        sentinel能够再master宕机的情况下,自动切换slave为master
    机制:
        1. 每个sentinel向其他master,slave,sentinel发送PING
        如果超时,就会被标记主观下线;
        2. 足够的sentinel标记master主观下线后,master标记为客观下线;
           向所有slave的发送INFO命令,不够数量的话客观上线移除
           如果PING命令重新有效,主观下线状态被移除
        3. 当使用sentinel模式的时候,客户端直接连接sentinel的IP和PORT;
           这样master节点挂掉,sentinel会提供新的master节点提供给使用者;
        4. 如果判定客观下线成功,断开master与slave连接,选取一个slave作为master,
           将其他的slave连接到新的master,并告知客户端新的服务器地址

集群模式:
    特点:
        支持高并发的读写,故障自动转移;
    机制:
        1. 数据切片,redis集群有16384个哈希槽,每个keyCRC16校验后对16384取模
        2. 主从复制 每个节点对应多个从节点,主节点宕机,会启用从节点;
           如果半数主节点认为A节点超时,那么认为A节点宕机;集群无法提供服务
```
#### 底层数据结构

redis 类型对照


|基础数据类型|可能的编码方式|
|:----|:----:|
|字符串|INT,EMBSTR,RAW|
|列表|之前是ziplist,linkedlist;3.2之后quiklist|
|集合|intset,hashtable|
|有序集合|ziplist,skiplist|
|散列|ziplist,hashtable|

字符串
```
底层SDS:
    len: 已使用字节数(不包括\0)
    alloc: 总共分配的字节数(不包括\0)
    flags: 标记属性,是sdshdr8还是sdshdr16
           包含5种,5,9,16,32,64,方便根据长度选择合适的类型
           节约内存
    buf: 字节数组
```
![image](https://mmbiz.qpic.cn/mmbiz_png/OqTAl3WTC7Gq4Mear6kfeD5qHWmojzZwQMQXH7jatA6qic7FxhOlD3lMxqC4iaGuE7grIlFJ6sicCCRbynhTicXRmA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

SDS与字符串比较
```
1、获取字符串长度复杂度为O(1)
    由于C语言没有存储字符串长度，每次获取字符串长度多需要进行循环整个字符串计算，时间复杂度为O(N)
    而SDS记录了存储的字符串的长度，获取字符串长度时直接获取len的属性值即可，时间复杂度为O(1)；
    而SDS中设置和更新长度是API中自动完成，无需手动进行操作。

2、杜绝缓冲区溢出
    C语言在进行两个字符串拼接时，一旦没有分配足够的内存空间，就会造成溢出；
    而SDS在修改字符串时，会先根据len的值，检查内存空间是否足够，
    如果不足会先分配内存空间，再进行字符串修改，这样就杜绝了缓冲区溢出。
3、减少修改字符串时带来的内存重新分配次数
    C语言不记录字符串长度，所以当修改时，会重新分配内存；
    如果是正常字符串，内存空间不够会产生溢出；
    如果是缩短字符串，不重重分配会产生泄露。
SDS采用空间预分配和惰性释放空间两种优化策略
空间预分配：
    对字符串进行增长操作，会分配出多余的未使用空间，这样如果以后的扩展，在一定程度上可以减少内存重新分配的次数。
惰性释放空间：
    对字符串经过缩短操作，并不会立即释放这些空间，而是使用free来记录这些空间的数量，当进行增长操作时，这些记录的空间就可以被重新利用；
    SDS提供了响应的API进行手动释放空间，所以不会造成内存浪费。
4、二进制安全
    C语言的字符串中不能包含空字符（因为C语言是以空字符判断字符串结尾的），所以不能保存一些二进制文件（有可能包含空字符，如图片）；
    SDS则是以len来判断字符串结尾，所以SDS结构可以存储图片等，并且都是以二进制方式进行处理。
5、兼容部分C字符串函数
    SDS结构中buf保存字符串同样是以空字符结尾，所以可以兼容C语言的部分字符串操作API。
```

列表
```
特点:
    双端列表: 带有指向前置节点和后置节点的指针
    无环: 表头节点的前置节点表尾节点的后置节点指向NULL
    链表长度计数器: 记录长度
    多态: 链表节点使用void*
```
![image](https://mmbiz.qpic.cn/mmbiz_png/OqTAl3WTC7Gq4Mear6kfeD5qHWmojzZwbJxwcmialX3lgnMv3gocSuvgic5bUDfNIpRiaicVXAEZTZ6icReicJMNBovQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

字典
```
```
![image](https://mmbiz.qpic.cn/mmbiz_png/OqTAl3WTC7Gq4Mear6kfeD5qHWmojzZwc4KGUvAU6PYaLgaYHiaksQSZO0SHKErKibXVyWXHXTahl1UjGDr9W8xA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1)

.编码

int
```
字符串对象为整数,并且整数值在long的范围内,则设置为int编码
```
raw
```
字符串对象保存的是字符串,且长度大于32字节,会使用SDS保存字符串
编码设置为raw
```
embstr
```
字符串对象保存的是字符串,且长度小于32字节,会使用embstr保存
embstr对SDS进行了优化,将SDS和字符串对象的值存放在一起
```
![image](https://upload-images.jianshu.io/upload_images/12321605-38f23873b2076cb5.png?imageMogr2/auto-orient/strip|imageView2/2/w/538/format/webp)

quiklist
```
3.2版本之后的list底层编码
结合双向链表和压缩列表结合,使用双向指针连接压缩列表
节约了内存空间
```
![image](https://upload-images.jianshu.io/upload_images/13587608-3e211c93f32ff058.png?imageMogr2/auto-orient/strip|imageView2/2/w/850/format/webp)

intset
```
set所有元素都是整数,且元素数量不大于512的时候使用intset
其他情况使用hastable
```
ziplist
```
由一系列特殊编码的连续内存块组成的顺序存储结构,在内存中是连续存储的
为了节省内存ziplist的每个元素所占的内存大小可以不同,
每个节点可以用来存储一个整数或者一个字符串
```
![image](https://img-blog.csdnimg.cn/20190430153606404.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pnYW9x,size_16,color_FFFFFF,t_70)
![image](https://img-blog.csdnimg.cn/20190430153651857.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pnYW9x,size_16,color_FFFFFF,t_70)

hashtable
```
```
![image](https://img-blog.csdn.net/20180608093936352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![image](https://img-blog.csdnimg.cn/20201104070257582.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3p3eDkwMDEwMg==,size_16,color_FFFFFF,t_70#pic_center)

跳表
```
定义:
    一个随机化的数据结构,实质就是一种可以进行二分查找的有序链表;
    跳表在原有的有序链表上面增加了多级索引,通过索引来实现快速查找;
    跳表不仅能提高搜索性能,同时也可以提高插入和删除操作的性能。
性质:
    1. 由很多层结构组成
    2.每一层都是一个有序的链表
    3.最底层(Level 1)的链表包含所有元素
    4.如果一个元素出现在Level i的链表中，则它在Level i之下的链表也都会出现。
    5.每个节点包含两个指针，一个指向同一链表中的下一个元素，一个指向下面一层的元素。 
```
![image](https://mmbiz.qpic.cn/mmbiz_jpg/OqTAl3WTC7HeXI9EvD3qof0ppmXdIrOTOk4YlUusCG2PiaJqANqiateoRaAJiczoX2ZVw7PXiaYALwgnrbuY5Uia13A/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

跳表的操作:

get
```
查找元素 117
比较21,比21大,往后面找
比较37,比37大,比链表最大值小,从37的下面一层开始找
比较71,比71大,比链表最大值小,从71的下面一层开始找
比较85,比85大,从后面找
比较117,等于117,找到了节点。
```
![image](https://mmbiz.qpic.cn/mmbiz_jpg/OqTAl3WTC7HeXI9EvD3qof0ppmXdIrOT1XCYFnf14hzjicdOUxffdR8Wzic7nol9ffKRId7LZzwrT6TicXO7DbB7Q/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

insert
---
插入 119， K = 2

![image](https://mmbiz.qpic.cn/mmbiz_jpg/OqTAl3WTC7HeXI9EvD3qof0ppmXdIrOTSLpo1GOkXn1XWvRGa0GZ0TxGbTjNKbYe2icPKCfStRh39HAC4UMLn3Q/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)
如果 K 大于链表的层数，则要添加新的层

![image](https://mmbiz.qpic.cn/mmbiz_jpg/OqTAl3WTC7HeXI9EvD3qof0ppmXdIrOTvs9F6prTnxdn1uMzUP2oFIanZXFxia2g83AVd78hImcPFZLPNAKllLA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)

删除
```
1.定义一个跳表的Node类
2.私有变量，保存层数K和链表最高层最小的节点（即最左结点）
3.按照上面的图，写出每个函数，注意边界判断
```
![image](https://mmbiz.qpic.cn/mmbiz_jpg/OqTAl3WTC7HeXI9EvD3qof0ppmXdIrOTA4fzGu5eLiaCHuBChnbmI1SAIWxQXbLRJ7R4dGibwaXfrQ7QbBPuUF6w/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1)


