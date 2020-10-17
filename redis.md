
### Redis
redis 的网络通信模型
epoll 源码看下

redis 各种数据结构的实现


1.Redis 是一个基于内存的高性能key-value数据库。
2.Redis相比memcached有哪些优势：

```
memcached所有的值均是简单的字符串，redis作为其替代者，支持更为丰富的数据类型
redis可以持久化其数据
```

3.Redis是单线程redis利用队列技术将并发访问变为串行访问，消除了传统数据库串行控制的开销

4.Reids常用5种数据类型string，list，set，sorted set，hash

5.Reids6种淘汰策略：
```
    noeviction: 不删除策略, 达到最大内存限制时, 如果需要更多内存, 直接返回错误信息。大多数写命令都会导致占用更多的内存(有极少数会例外。
    allkeys-lru:所有key通用; 优先删除最近最少使用(less recently used ,LRU) 的 key。
    volatile-lru:只限于设置了 expire 的部分; 优先删除最近最少使用(less recently used ,LRU) 的 key。
    allkeys-random:所有key通用; 随机删除一部分 key。volatile-random: 只限于设置了 expire 的部分; 随机删除一部分 key。
    volatile-ttl: 只限于设置了 expire 的部分; 优先删除剩余时间(time to live,TTL) 短的key。
```    

7.Redis的并发竞争问题如何解决?
```
单进程单线程模式，采用队列模式将并发访问变为串行访问。
Redis本身没有锁的概念，Redis对于多个客户端连接并不存在竞争，利用setnx实现锁。
```

12.Redis的主从复制持久化保证了即使redis服务重启也不会丢失数据，
    因为redis服务重启后会将硬盘上持久化的数据恢复到内存中，
    但是当redis服务器的硬盘损坏了可能会导致数据丢失，如果通过redis的主从复制机制就可以避免这种单点故障，

13.Redis是单线程的，但Redis为什么这么快？
```
1、完全基于内存，绝大部分请求是纯粹的内存操作，非常快速。
    数据存在内存中，类似于HashMap，HashMap的优势就是查找和操作的时间复杂度都是O(1)；
2、数据结构简单，对数据操作也简单，Redis中的数据结构是专门进行设计的；
3、采用单线程，避免了不必要的上下文切换和竞争条件，也不存在多进程或者多线程导致的切换而消耗 CPU，
    不用去考虑各种锁的问题，不存在加锁释放锁操作，没有因为可能出现死锁而导致的性能消耗；
4、使用多路I/O复用模型，非阻塞IO；这里“多路”指的是多个网络连接，“复用”指的是复用同一个线程
5、使用底层模型不同，它们之间底层实现方式以及与客户端之间通信的应用协议不一样，
    Redis直接自己构建了VM 机制 ，因为一般的系统调用系统函数的话，会浪费一定的时间去移动和请求；
```
14.为什么Redis是单线程的？
```
Redis是基于内存的操作，CPU不是Redis的瓶颈，Redis的瓶颈最有可能是机器内存的大小或者网络带宽。
既然单线程容易实现，而且CPU不会成为瓶颈，那就顺理成章地采用单线程的方案了（毕竟采用多线程会有很多麻烦！）。
```
15.Redis info查看命令：info memory

16.Redis内存模型
```
    used_memory：Redis分配器分配的内存总量（单位是字节），包括使用的虚拟内存（即swap）；Redis分配器后面会介绍。
    used_memory_human: 只是显示更友好。
    used_memory_rss：Redis进程占据操作系统的内存（单位是字节），与top及ps命令看到的值是一致的；除了分配器分配的内存之外，
    used_memory_rss: 还包括进程运行本身需要的内存、内存碎片等，但是不包括虚拟内存。
    mem_fragmentation_ratio：内存碎片比率，该值是used_memory_rss / used_memory的比值。
    mem_allocator：Redis使用的内存分配器，在编译时指定；可以是 libc 、jemalloc或者tcmalloc，默认是jemalloc；截图中使用的便是默认的jemalloc。
```
17.Redis内存划分数据
```
    作为数据库，数据是最主要的部分；这部分占用的内存会统计在used_memory中。
    进程本身运行需要的内存Redis主进程本身运行肯定需要占用内存，如代码、常量池等等；
    这部分内存大约几兆，在大多数生产环境中与Redis数据占用的内存相比可以忽略。
    这部分内存不是由jemalloc分配，因此不会统计在used_memory中。
    缓冲内存缓冲内存包括客户端缓冲区、复制积压缓冲区、AOF缓冲区等；
    其中，客户端缓冲存储客户端连接的输入输出缓冲；复制积压缓冲用于部分复制功能；
    AOF缓冲区用于在进行AOF重写时，保存最近的写入命令。在了解相应功能之前，不需要知道这些缓冲的细节；
    这部分内存由jemalloc分配，因此会统计在used_memory中。内存碎片内存碎片是Redis在分配、回收物理内存过程中产生的。
    例如，如果对数据的更改频繁，而且数据之间的大小相差很大，可能导致redis释放的空间在物理内存中并没有释放，
    但redis又无法有效利用，这就形成了内存碎片。内存碎片不会统计在used_memory中。

```
18.Redis对象有5种类型无论是哪种类型，Redis都不会直接存储，而是通过redisObject对象进行存储。

19.Redis没有直接使用C字符串(即以空字符’\0’结尾的字符数组)作为默认的字符串表示，而是使用了SDS。SDS是简单动态字符串(Simple Dynamic String)的缩写。

20.Reidis的SDS在C字符串的基础上加入了free和len字段

21.Reids主从复制复制是高可用Redis的基础，哨兵和集群都是在复制基础上实现高可用的。

```
    复制主要实现了数据的多机备份，以及对于读操作的负载均衡和简单的故障恢复。
    缺陷：
        故障恢复无法自动化；
        写操作无法负载均衡；
        存储能力受到单机的限制。
```
22.Redis哨兵在复制的基础上，哨兵实现了自动化的故障恢复。
```
        缺陷：写操作无法负载均衡；存储能力受到单机的限制。
```
23.Reids持久化触发条件RDB持久化的触发分为手动触发和自动触发两种。
24.Redis 开启AOF    Redis服务器默认开启RDB，关闭AOF；要开启AOF，需要在配置文件中配置：appendonly yes
25.AOF常用配置总结下面是AOF常用的配置项，以及默认值；前面介绍过的这里不再详细介绍。
```
    appendonly no：是否开启
    AOFappendfilename "appendonly.aof"：AOF文件名
    dir ./：RDB文件和AOF文件所在目录
    appendfsync everysec：fsync持久化策略
    no-appendfsync-on-rewrite no：AOF重写期间是否禁止
    fsync；如果开启该选项，可以减轻文件重写时CPU和硬盘的负载（尤其是硬盘），但是可能会丢失AOF重写期间的数据；需要在负载和安全性之间进行平衡
    auto-aof-rewrite-percentage 100：文件重写触发条件之一
    auto-aof-rewrite-min-size 64mb：文件重写触发提交之一
    aof-load-truncated yes：如果AOF文件结尾损坏，Redis启动时是否仍载入AOF文件
```
26.RDB和AOF的优缺点
```
RDB持久化优点：
    RDB文件紧凑，体积小，网络传输快，适合全量复制；
    恢复速度比AOF快很多。当然，与AOF相比，RDB最重要的优点之一是对性能的影响相对较小。
缺点：
    RDB文件的致命缺点在于其数据快照的持久化方式决定了必然做不到实时持久化，而在数据越来越重要的今天，
    数据的大量丢失很多时候是无法接受的，因此AOF持久化成为主流。
    此外，RDB文件需要满足特定格式，兼容性差（如老版本的Redis不兼容新版本的RDB文件）。
AOF持久化与RDB持久化相对应，AOF的优点在于支持秒级持久化、兼容性好，缺点是文件大、恢复速度慢、对性能影响大。
```
27.持久化策略选择
```
    如果Redis中的数据完全丢弃也没有关系（如Redis完全用作DB层数据的cache），那么无论是单机，还是主从架构，都可以不进行任何持久化。
    在单机环境下（对于个人开发者，这种情况可能比较常见），如果可以接受十几分钟或更多的数据丢失，选择RDB对Redis的性能更加有利；
        如果只能接受秒级别的数据丢失，应该选择AOF。
    但在多数情况下，我们都会配置主从环境，slave的存在既可以实现数据的热备，
        也可以进行读写分离分担Redis读请求，以及在master宕掉后继续提供服务。
```
28.redis缓存被击穿处理机制使用mutex。
```
`简单地来说，就是在缓存失效的时候（判断拿出来的值为空），不是立即去load db，而是先使用缓存工具的某些带成功操作返回值的操作
    （比如Redis的SETNX或者Memcache的ADD）去set一个mutex key，当操作返回成功时，再进行load db的操作并回设缓存；
    否则，就重试整个get缓存的方法
```
29.Redis还提供的高级工具像慢查询分析、性能测试、Pipeline、事务、Lua自定义命令、Bitmaps、HyperLogLog、发布/订阅、Geo等个性化功能。

30.Redis常用管理命令
```
# dbsize 返回当前数据库 key 的数量。
# info 返回当前 redis 服务器状态和一些统计信息。
# monitor 实时监听并返回redis服务器接收到的所有请求信息。
# shutdown 把数据同步保存到磁盘上，并关闭redis服务。
# config get parameter 获取一个 redis 配置参数信息。（个别参数可能无法获取）
# config set parameter value 设置一个 redis 配置参数信息。（个别参数可能无法获取）
# config resetstat 重置 info 命令的统计信息。（重置包括：keyspace 命中数、
# keyspace 错误数、 处理命令数，接收连接数、过期 key 数）
# debug object key 获取一个 key 的调试信息。
# debug segfault 制造一次服务器当机。
# flushdb 删除当前数据库中所有 key,此方法不会失败。小心慎用
# flushall 删除全部数据库中所有 key，此方法不会失败。小心慎用
```

31.Reids工具命令
```
#redis-server：Redis 服务器的 daemon 启动程序
#redis-cli：Redis 命令行操作工具。当然，你也可以用 telnet 根据其纯文本协议来操作
#redis-benchmark：Redis 性能测试工具，测试 Redis 在你的系统及你的配置下的读写性能
$redis-benchmark -n 100000 –c 50
#模拟同时由 50 个客户端发送 100000 个 SETs/GETs 查询
#redis-check-aof：更新日志检查
#redis-check-dump：本地数据库检查
```

32.为什么需要持久化？
```
由于Redis是一种内存型数据库，即服务器在运行时，系统为其分配了一部分内存存储数据，一旦服务器挂了，或者突然宕机了，
那么数据库里面的数据将会丢失，为了使服务器即使突然关机也能保存数据，必须通过持久化的方式将数据从内存保存到磁盘中。
```
33.判断key是否存在exists key +key名字

34.删除keydel key1 key2 ...

35.缓存和数据库间数据一致性问题分布式环境下（单机就不用说了）
```
非常容易出现缓存和数据库间的数据一致性问题，针对这一点的话，只能说，如果你的项目对缓存的要求是强一致性的，那么请不要使用缓存。
我们只能采取合适的策略来降低缓存和数据库间数据不一致的概率，而无法保证两者间的强一致性。
合适的策略包括 合适的缓存更新策略，更新数据库后要及时更新缓存、缓存失败时增加重试机制，例如MQ模式的消息队列。
```
36.布隆过滤器bloomfilter就类似于一个hash set，用于快速判某个元素是否存在于集合中，
    其典型的应用场景就是快速判断一个key是否存在于某容器，不存在就直接返回。布隆过滤器的关键就在于hash算法和容器大小
    
37.缓存雪崩问题存在同一时间内大量键过期（失效），接着来的一大波请求瞬间都落在了数据库中导致连接异常。
```
    解决方案：
        1、也是像解决缓存穿透一样加锁排队。
        2、建立备份缓存，缓存A和缓存B，A设置超时时间，B不设值超时时间，先从A读缓存，A没有读B，并且更新A缓存和B缓存;
```

38.缓存并发问题
```
    这里的并发指的是多个redis的client同时set key引起的并发问题。
    比较有效的解决方案就是把redis.set操作放在队列中使其串行化，必须的一个一个执行，具体的代码就不上了，当然加锁也是可以的，
    至于为什么不用redis中的事务，留给各位看官自己思考探究。
```
39.Redis分布式redis支持主从的模式。
```
    原则：Master会将数据同步到slave，而slave不会将数据同步到master。
    Slave启动时会连接master来同步数据。这是一个典型的分布式读写分离模型。
    我们可以利用master来插入数据，slave提供检索服务。这样可以有效减少单个机器的并发访问数量
```
40.读写分离模型通过增加Slave DB的数量，读的性能可以线性增长。
```
    为了避免Master DB的单点故障，集群一般都会采用两台Master DB做双机热备，
    所以整个集群的读和写的可用性都非常高。读写分离架构的缺陷在于，不管是Master还是Slave，
    每个节点都必须保存完整的数据，如果在数据量很大的情况下，集群的扩展能力还是受限于单个节点的存储能力，
    而且对于Write-intensive类型的应用，读写分离架构并不适合。
```
41.数据分片模型为了解决读写分离模型的缺陷，可以将数据分片模型应用进来。
```
    可以将每个节点看成都是独立的master，然后通过业务实现数据分片。
    结合上面两种模型，可以将每个master设计成由一个master和多个slave组成的模型。
```
42.redis常见性能问题和解决方案：
```
    Master最好不要做任何持久化工作，如RDB内存快照和AOF日志文件如果数据比较重要，某个Slave开启AOF备份数据，
    策略设置为每秒同步一次为了主从复制的速度和连接的稳定性，Master和Slave最好在同一个局域网内尽量避免在压力很大的主库上增加从库
```
43.redis通讯协议RESP 是redis客户端和服务端之前使用的一种通讯协议；
```
    RESP 的特点：实现简单、快速解析、可读性好
```
44.Redis分布式锁实现先拿setnx来争抢锁，抢到之后，再用expire给锁加一个过期时间防止锁忘记了释放。

请你来说一说Redis的定时机制怎么实现的
```
Redis服务器是一个事件驱动程序，服务器需要处理以下两类事件：
文件事件（服务器对套接字操作的抽象）和时间事件（服务器对定时操作的抽象）。
Redis的定时机制就是借助时间事件实现的。
一个时间事件主要由以下三个属性组成：
id：时间事件标识号；when：记录时间事件的到达时间；timeProc：时间事件处理器，当时间事件到达时，服务器就会调用相应的处理器来处理时间。
一个时间事件根据时间事件处理器的返回值来判断是定时事件还是周期性事件
一个时间事件主要由以下三个属性组成：id：时间事件标识号；when：记录时间事件的到达时间；timeProc：时间事件处理器，当时间事件到达时，服务器就会调用相应的处理器来处理时间。一个时间事件根据时间事件处理器的返回值来判断是定时事件还是周期性事件。
```
请你来说一说Redis是单线程的，但是为什么这么高效呢?
```
1、完全基于内存，绝大部分请求是纯粹的内存操作，非常快速。数据存在内存中，类似于HashMap，HashMap的优势就是查找和操作的时间复杂度都是O(1)；
2、数据结构简单，对数据操作也简单，Redis中的数据结构是专门进行设计的；
3、采用单线程，避免了不必要的上下文切换和竞争条件，也不存在多进程或者多线程导致的切换而消耗 CPU，不用去考虑各种锁的问题，
    不存在加锁释放锁操作，没有因为可能出现死锁而导致的性能消耗；
4、使用多路I/O复用模型，非阻塞IO；
5、使用底层模型不同，它们之间底层实现方式以及与客户端之间通信的应用协议不一样，Redis直接自己构建了VM 机制 ，
    因为一般的系统调用系统函数的话，会浪费一定的时间去移动和请求；
```
请问Redis的数据类型有哪些，底层怎么实现?

请问Redis的rehash怎么做的，为什么要渐进rehash，渐进rehash又是怎么实现的?
请你来说一下Redis和memd的区别
请问Redis怎么实现的定期删除功能
请你说一说Redis对应的命令和数据类型...

Redis数据类型
```
1、String 类型：最基础的数据类型，简单的key-value键值对，value可以是任何数据，不限制类型，但大小最多不能超过512M。
2、list 类型：列表类型，类似一个队列，左侧为表头，右侧为表尾，左右两侧都可以插入数据；底层实现为链表。
3、set 类型：无序集合类型，key-values 集合，values内部的元素不重复。
4、zset 类型：有序集合类型，数据结构类似于java中的Map<String，List<Object>>，key-values结构，values内部元素有序。
5、hash 类型：hash表结构，类似于java中的Map<String,Map<String,Object>>。
```
Redis持久化方式
```
1、RDB持久化：快照snapshot持久化方式，把当前内存中的数据集快照写入硬盘，恢复数据时直接读取硬盘上的快照文件。
2、AOF持久化：通过记录redis服务器所执行的写命令日志进行持久化，恢复数据是，执行保存的写命令进行数据恢复。
```
Redis过期删除策略
```
1、redis存入数据时可以设置过期时间，如果不设置，则永不过期；
2、redis到期删除策略是定期随机抽样+惰性删除。
```
 
Redis内存淘汰机制
```
1、no-enviction：禁止淘汰数据，内存不足则报错。
2、allkeys-random：从内存数据集中随机淘汰数据。
3、allkey-lru：从内存数据集中淘汰最近最少使用数据（最常用）。
4、volatile-random：从设置过过期时间的数据集中随机淘汰数据，未设置过期时间的数据不受影响。
5、volatile-lru：从设置过过期时间的数据集中淘汰最近最少使用数据。
6、volatile-ttl：从设置过过期时间的数据集中淘汰将要过期的数据。
```
1、Redis的数据结构
```
Redis 的底层数据结构包含简单的动态字符串（SDS）、链表、字典、压缩列表、整数集合等等；
五大数据类型（数据对象）都是由一种或几种数结构构成。
在命令行中可以使用 OBJECT ENCODING key 来查看key的数据结构。
```
2、简单动态字符串SDS
```
 redis是使用C语言编写的，但是string数据类型并没有使用C语言的字符串，而是重新编写一个简单的动态字符串（simple dynamic string,SDS）。

/*
 * 保存字符串对象的结构
*/
struct sdshdr {
      // buf 中已占用空间的长度
      int len;   
     // buf 中剩余可用空间的长度
     int free;  
     // 数据空间
     char buf[]
};

free 表示buf数组中剩余的空间数量
len 记录了buf数组中已存储的字节长度
buf 数组是一个char类型的数据，记录具体存储的字符串，并且以 ‘\0’(空字符) 作为结束标识符
```
SDS定义较C语言的字符串几乎相同，就是多出两个属性free，len；那为何不直接使用C语言的字符串呢？
```
1、获取字符串长度复杂度为O(1)
    由于C语言没有存储字符串长度，每次获取字符串长度多需要进行循环整个字符串计算，时间复杂度为O(N)；
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

3、链表
```
Redis使用C语言编写，但并没有内置链表这种数据结构，而是自己构建了链表的实现；构成链表结构的节点为链表节点。
链表用的非常广泛，如列表键、发布与订阅、慢查询、监视器等。

typedef struct listNode {
     // 前置节点
     struct listNode * prev;
     // 后置节点
     struct listNode * next;
     // 节点的值
     void * value;
}listNode;

多个listNode可以通过prev和next指针构成双端链表，使用list持有链表

typedef struct list {
     // 表头节点
     listNode * head;
     // 表尾节点
     listNode * tail;
     // 链表所包含的节点数量
     unsigned long len;
     // 节点值复制函数
     void *(*dup)(void *ptr);
     // 节点值释放函数
     void (*free)(void *ptr);
     // 节点值对比函数
     int (*match)(void *ptr,void *key);
 } list;

head 表头指针
tail 表尾指针
len 链表长度计数器
dup、free、match 多态链表所需的类型特定的函数
```

Redis链表实现的特性如下：
```
1、双端
    链表节点带有prev和next指针，可以快速获取前置和后置节点，时间复杂度都是O(1)。
2、无环
    头节点prev指针和尾节点next指针都指向null，对链表访问以NULL为终点。
3、带表头指针和表尾指针
    可以快速的获取表头节点和表尾节点。
4、有链表长度计数器
    可以快速获取链表长度。 
5、多态
    链表可以保存各种不同类型的值，通过list的dup，free，match三个属性为节点设计值类型特定的函数。
```
4、字典
```
字典又称为符号表（symbol table）、关联数组（associative array）或者映射（map）；
字典中存储key-value键值对，并且key不重复；
字典在Redis中广泛应用，如Redis数据库就是使用字典作为底层实现的。
Redis使用的C语言没有内置这种结构，所以Redis构建了自己的字典实现。
字典使用哈希表作为底层试下，一个哈希表包含多个哈希节点，每个哈希节点保存一个键值对。

哈希表
 typedef struct dictht {
     // 哈希表数组
     dictEntry **table;
     // 哈希表大小
     unsigned long size;
     // 哈希表大小掩码，用于计算索引值
     // 总是等于size-1
     unsigned long sizemask;
     // 该哈希表已有节点的数量
     unsigned long used;
} dictht;

table是一个数组，数组元素是dictEntry结构的指针，每个dictEntry保存一个键值对
size 记录哈希表的大小
sizemask 值总是等于size-1，这个属性和哈希值一起决定一个键应该被方法table数组的哪个索引上
used 记录哈希表目前已有节点的数量
哈希表节点 
typedef struct dictEntry {
     // 键
     void *key;
     // 值
     union{
         void *val;
         uint64_tu64;
         int64_ts64;
     } v;
     // 指向下个哈希表节点，形成链表
     struct dictEntry *next;
} dictEntry;

key属性保存着键值对中的键，v属性保存着键值对中的值
键值对中的值可以使指针val、一个uint64_t整数，或是一个int64_t整数
next是指向另一个哈希表节点的指针，用以解决多个哈希值冲突问题


字典结构
typedef struct dict {
     // 类型特定函数
     dictType *type;
     // 私有数据
     void *privdata;
     // 哈希表
     dictht ht[2];
     // rehash索引
     //当rehash不在进行时，值为-1
     in trehashidx; /* rehashing not in progress if rehashidx == -1 */
} dict;

 typedef struct dictType {
     // 计算哈希值的函数
     unsigned int (*hashFunction)(const void *key);
     // 复制键的函数
     void *(*keyDup)(void *privdata, const void *key);
     // 复制值的函数
     void *(*valDup)(void *privdata, const void *obj);
     // 对比键的函数
     int (*keyCompare)(void *privdata, const void *key1, const void *key2);
     // 销毁键的函数
     void (*keyDestructor)(void *privdata, void *key);
     // 销毁值的函数
     void (*valDestructor)(void *privdata, void *obj);
} dictType;

type 属性是一个指向dictType结构的指针，每个dictType机构保存了一簇用于操作特定类型键值对的函数，Redis货位用途不同的字典设置不同的类型特定函数。
privdata 属性保存了需要传给那些类型特定函数的可选参数。
ht 属性是一个长度为2的数组，数组中的每个元素都是一个哈希表，一般情况下自字典只使用ht[0]，ht[1]只会在进行rehash时使用.
trehashidx 属性记录了rehash目前的进度，如果没有进行rehash则它的值为-1。
当一个新的键值对要添加到字典中去时，会涉及到一系列的操作，如计算索引、解决冲突、扩容等等，下面对这些操作进行描述。
```
1、哈希算法
```
添加键值对时，首先要根据键值对的键计算出哈希值和索引值，然后再根据索引值进行放入
1 #使用字典设置的哈希函数，计算键key的哈希值
2 hash = dict->type->hashFunction(key);
3 #使用哈希表的sizemask属性和哈希值，计算出索引值
4 #根据情况不同，ht[x]可以是ht[0]或者ht[1]
5 index = hash & dict->ht[x].sizemask;
```
2、结局键冲突
```
当有两个或以上数量的键值被分配到了哈希表数组的同一个索引上时，就发生了键冲突。
Redis的哈希表使用单向链表解决键冲突问题，每个新的键总是添加到单项链表的表头。
```
3、rehash（扩展或收缩）
```
哈希表具有负载因子（load factor），其始终需要保持在一个合理的范围之内，当hashI表保存的键值对过多或过少时，就需要对哈希表进行rehash（重新散列）操作，步骤许下
(1) 为字典的ht[1]分配空间，空间大小：如果是扩展操作则为ht[0].used * 2 ，也就是扩展为当前哈希表已使用空间的1倍；如果是收缩，则减小1倍。
(2) 将ht[0]内的数据重新计算哈希值和索引，并放到新分配的ht[1]空间上。
(3) 全部迁移完成后，将ht[1]设置为ht[0]，释放ht[0]并创建一个空白的哈希表为ht[1]，为下次rehash做准备。
```
4、哈希表的扩展与收缩触发条件
```
(1) 服务器目前没有在执行BGSAVE命令或者BGREWRITEAOF命令，并且哈希表的负载因子大于等等于1。
(2) 服务器目前正在执行BGSAVE命令或者BGREWRITEAOF命令，并且哈希表的负载因子大于等于5。
以上条件中任意一条被满足，程序自动开始对哈希表进行扩展；
负载因子算法：负载因子 = 哈希表以保存的节点数量 / 哈希表大小
当负载因子小于0.1时，程序自动进行收缩操作。
```
5、渐进式rehash
```
渐进式rehash就是，当ht[1]的键值对向ht[1]迁移的过程中，如果数据量过大，则不能一次性迁移， 否则会对服务器性能造成影响，而是分成多次，渐进式的进行迁移。
在rehash期间，会维持一个索引计数器rehashidx，并把每次的迁移工作分配到了添加、删除、查找、更新操作中，当rehash工作完成后rehashidx会增加1，
这样所有的ht[0]的值全部迁移完成后，程序会将rehashidx这是为-1，标识最终的rehash完成。
```
6、渐进式rehash之情期间的表操作
```
由于渐进式rehash期间，ht[0]和ht[1]中都有数据，当查找时，会先在ht[0]中进行，没找到继续到ht[1]中找；而添加操作一律会添加到ht[1]中。
```
 字典总结： 
```
Redis字典底层机构实现与java(1.6之前) 中的hashmap非常相像，都是使用单项链表解决键冲突问题。
个人疑问：jdk1.8以上已经是用红黑树解决多个键冲突问题，不知redis的键冲突是否也可以用红黑树？
```
5、跳跃表
```
跳跃表（skiplist）数据结构特点是每个节点中有多个指向其他节点的指针，从而快速访问节点。
跳跃表结构由跳跃表节点（zskiplistNode）和zskiplist两个结构组成
跳跃表节点

 typedef struct zskiplistNode {
     // 层
     struct zskiplistLevel {
         // 前进指针
         struct zskiplistNode *forward;
         // 跨度
         unsigned int span;
     } level[];
     // 后退指针
     struct zskiplistNode *backward;
     // 分值
     double score;
     // 成员对象
     robj *obj;
 } zskiplistNode;

层：为一个数组，数组中的每个数据都包含前进指针和跨度。
前进指针：指向表尾方向的其他节点的指针，用于从表头方向到表尾方向快速访问节点。
跨度：记录两个节点之间的距离，跨度越大，两个节点相聚越远，所有指向NULL的前进指针的跨度都为0。
后退指针：用于从表尾节点向表头节点访问，每个节点都有后退指针，并且每次只能后退一个节点。
分值：节点的分值是一个double类型的浮点数，跳跃表中的说有分值按从小到大排列。
成员对象：是一个指向字符串的指针，字符串则保存着一个SDS值。

跳跃表
typedef struct zskiplist {
     // 表头节点和表尾节点
     structz skiplistNode *header, *tail;
     // 表中节点的数量
     unsigned long length;
     // 表中层数最大的节点的层数
     int level;
} zskiplist;

header 指向跳跃表的表头节点，
tail指向跳跃表的表尾节点，
level记录节点中的最大层数（不含表头节点），
length跳跃表包含节点数量（不含表头节点）。
跳跃表由很多层构成（L1、L2 ...），每个层都带有两个属性前进指针和跨度。
每个节点都包含成员对象（obj）、分值（score）、后退指针（backward），头结点也包含这些属性但不会被用到
在此处只是介绍跳跃表的结构相关，关于跳跃表的层的形成，对象的插入、删除、查询等操作的原理在此处不做详解，另外会有文章进行说明。
```
6、整数集合
```
整数集合（intset）是集合键的底层实现之一，当一个集合只包含整数元素，并且元素的个数不多时，Redis就会使用整数集合作为集合键的底层实现。
整数集合可以保存int16_t、int32_t、int64_t的整数值，并且不会出现重复元素
typedef struct intset {
     // 编码方式
     uint32_t encoding;
     // 集合包含的元素数量
     uint32_t length;
     // 保存元素的数组
     int8_t contents[];
} intset;

contents数组存储的是集合中的每个元素，他的类型是int8_t，但存储数据的实际类型取决于编码方式encoding
encoding编码方式有三种INTSET_ENC_INT16、INTSET_ENC_INT32、INTSET_ENC_INT64分别对应的是int16_t、int32_t、int64_t类型
length记录整数集合的元素数量，即contents数组的长度

整数集合的升级操作
整数集合中原来保存的是小类型（如：int16_t）的整数，当插入比其类型大（如：int_64_t）的整数时，
会把整合集合里的元素的数据类型都转换成大的类型，这个过程称为升级

升级整数集合并添加新元素步骤如下：
（1）根据新元素的类型，扩展整数集合的底层数据的空间大小，并为新元素分配空间。
（2）将现有的所有元素的类型转换成与新元素相同的类型，保持原有数据有序性不变的情况下，把转换后的元素放在正确的位置上。
（3）将新元素添加到数组里。
新元素引发升级，所以新元素要么比所有元素都大，要么比所有元素都小。
当小于所有元素时，新元素放在底层数组的最开头
当大于所有元素时，新元素放在底层数据的最末尾
升级操作的好处
提升整数的灵活性，可以任意的向集合中放入3中不同类型的整数，而不用担心类型错误。
节约内存，整数集合中只有大类型出现的时候才会进行升级操作。
整数集合不支持降级操作

7、压缩列表
```
压缩列表（ziplist）是Redis为了节约内存而开发，是一系列特殊编码的连续内存块组成的顺序型数据结构。
一个压缩列表可以包含任意多个节点，每个节点可以保存一个字节数组或者一个整数值。
每个压缩列表含有若干个节点，而每个节点都由三部分构成，previous_entry_length、encoding、content，如图：

previous_entry_length 存储的是前一个节点的长度，由于压缩列表内存块连续，使用此属性值可以计算前一个节点的地址，压缩列表就是使用这一原理进行遍历。
previous_entry_length 如果前一节点长度小于254字节，那么previous_entry_length属性本身长度为1字节，存储的指就是前一节点的长度；如果大于254个字节，
        那么previous_entry_length属性本身长度为5个字节，前一个字节为0xFE(十进制254)，之后四个字节存储前一节点的长度。
encoding 记录本节点的content属性所保存数据的类型及长度，其本身长度为一字节、两字节或五字节，值得最高位为00、01或10的是字节数组的编码，
    最高位以11开头的是整数编码。
content 保存节点的值，可以是一个字节数组或者整数。
连锁更新
    当对压缩列表进行添加节点或删除节点时有可能会引发连锁更新，由于每个节点的 previous_entry_length 存在两种长度1字节或5字节，
    当所有节点previous_entry_length都为1个字节时，有新节点的长度大于254个字节，那么新的节点的后一个节点的previous_entry_length原来为1个字节，
    无法保存新节点的长度，这是就需要进行空间扩展previous_entry_length属性由原来的1个字节增加4个字节变为5个字节，
    如果增加后原节点的长度超过了254个字节则后续节点也要空间扩展，以此类推，最极端的情况是一直扩展到最后一个节点完成；
    这种现象称为连锁更新。在日常应用中全部连锁更新的情况属于非常极端的，不常出现。
```
8、总结
```
Redis的底层数据结构共有六种，简单动态字符串（SDS）、链表、字典、跳跃表、整数集合、压缩列表。
Redis中的五大数据类型的底层就是由他们中的一种或几种实现，数据的存储结构最终也会落到他们上。
可是在redis命令下使用 OBJECT ENCODING 命令查看键值对象的编码方式，也就是是以哪种结构进行的底层编码。
```

```
Redis的五大数据类型也称五大数据对象；前面介绍过6大数据结构，Redis并没有直接使用这些结构来实现键值对数据库，
而是使用这些结构构建了一个对象系统redisObject；
这个对象系统包含了五大数据对象，字符串对象（string）、列表对象（list）、哈希对象（hash）、集合（set）对象和有序集合对象（zset）；
而这五大对象的底层数据编码可以用命令OBJECT ENCODING来进行查看。

redisObject结构
typedef struct redisObject {
     // 类型
     unsigned type:4;
     // 编码
     unsigned encoding:4;
     // 指向底层实现数据结构的指针
     void *ptr;
     // ...
} robj;

redis是以键值对存储数据的，所以对象又分为键对象和值对象，即存储一个key-value键值对会创建两个对象，键对象和值对象。
键对象总是一个字符串对象，而值对象可以是五大对象中的任意一种。
type属性存储的是对象的类型，也就是我们说的 string、list、hash、set、zset中的一种，可以使用命令 TYPE key 来查看。
encoding属性记录了队形所使用的编码，即这个对象底层使用哪种数据结构实现。
表中列出了底层编码常量及对应的OBJECT ENCODING 命令的输出，前三项都是字符串结构
我们在存入key-value键值对时并不会指定对象的encoding，而是Redis会根据不统的使用场景来为一个对象设置不同的编码，可以达到节约内存、加快访问速度等目的。
```
2、字符串对象(string)
```
字符串对象底层数据结构实现为简单动态字符串（SDS）和直接存储，但其编码方式可以是int、raw或者embstr，区别在于内存结构的不同。
（1）int编码
字符串保存的是整数值，并且这个正式可以用long类型来表示，那么其就会直接保存在redisObject的ptr属性里，并将编码设置为int，如图：
（2）raw编码
 字符串保存的大于32字节的字符串值，则使用简单动态字符串（SDS）结构，并将编码设置为raw，此时内存结构与SDS结构一致，内存分配次数为两次，
 创建redisObject对象和sdshdr结构，如图：
（3）embstr编码
 字符串保存的小于等于32字节的字符串值，使用的也是简单的动态字符串（SDS结构），但是内存结构做了优化，用于保存顿消的字符串；内存分配也只需要一次就可完成，分配一块连续的空间即可，如图：

 字符串对象总结：
在Redis中，存储long、double类型的浮点数是先转换为字符串再进行存储的。
raw与embstr编码效果是相同的，不同在于内存分配与释放，raw两次，embstr一次。
embstr内存块连续，能更好的利用缓存在来的优势
int编码和embstr编码如果做追加字符串等操作，满足条件下会被转换为raw编码；embstr编码的对象是只读的，一旦修改会先转码到raw。
```
3、列表对象(list)
```
列表对象的编码可以是ziplist和linkedlist之一。
（1） ziplist编码
ziplist编码的哈希随想底层实现是压缩列表，每个压缩里列表节点保存了一个列表元素。
（2）linkedlist编码
linkedlist编码底层采用双端链表实现，每个双端链表节点都保存了一个字符串对象，在每个字符串对象内保存了一个列表元素。
列表对象编码转换：
列表对象使用ziplist编码需要满足两个条件：一是所有字符串长度都小于64字节，二是元素数量小于512，不满足任意一个都会使用linkedlist编码。
两个条件的数字可以在Redis的配置文件中修改，list-max-ziplist-value选项和list-max-ziplist-entries选项。
图中StringObject就是上一节讲到的字符串对象，字符串对象是唯一个在五大对象中作为嵌套对象使用的。
``` 

4、哈希对象(hash)
```
哈希对象的编码可以是ziplist和hashtable之一。
（1）ziplist编码
ziplist编码的哈希对象底层实现是压缩列表，在ziplist编码的哈希对象中，key-value键值对是以紧密相连的方式放入压缩链表的，
先把key放入表尾，再放入value；键值对总是向表尾添加。
（2）hashtable编码
hashtable编码的哈希对象底层实现是字典，哈希对象中的每个key-value对都使用一个字典键值对来保存。
字典键值对即是，字典的键和值都是字符串对象，字典的键保存key-value的key，字典的值保存key-value的value。
哈希对象编码转换：
哈希对象使用ziplist编码需要满足两个条件：一是所有键值对的键和值的字符串长度都小于64字节；
二是键值对数量小于512个；不满足任意一个都使用hashtable编码。
以上两个条件可以在Reids配置文件中修改hash-max-ziplist-value选项和hash-max-ziplist-entries选项。
```
5、集合对象(set)
```
集合对象的编码可以是intset和hashtable之一。
（1）intset编码
intset编码的集合对象底层实现是整数集合，所有元素都保存在整数集合中。
（2）hashtable编码
hashtable编码的集合对象底层实现是字典，字典的每个键都是一个字符串对象，保存一个集合元素，不同的是字典的值都是NULL；可以参考java中的hashset结构。
集合对象编码转换：
集合对象使用intset编码需要满足两个条件：一是所有元素都是整数值；二是元素个数小于等于512个；不满足任意一条都将使用hashtable编码。
以上第二个条件可以在Redis配置文件中修改et-max-intset-entries选项。
```
 6、有序集合对象(zset)
 ```
有序集合的编码可以是ziplist和skiplist之一。
（1）ziplist编码 
ziplist编码的有序集合对象底层实现是压缩列表，其结构与哈希对象类似，不同的是两个紧密相连的压缩列表节点，
第一个保存元素的成员，第二个保存元素的分值，而且分值小的靠近表头，大的靠近表尾。
（2）skiplist编码
skiplist编码的有序集合对象底层实现是跳跃表和字典两种；
每个跳跃表节点都保存一个集合元素，并按分值从小到大排列；节点的object属性保存了元素的成员，score属性保存分值；
字典的每个键值对保存一个集合元素，字典的键保存元素的成员，字典的值保存分值。
```
为何skiplist编码要同时使用跳跃表和字典实现？
```
跳跃表优点是有序，但是查询分值复杂度为O(logn)；字典查询分值复杂度为O(1) ，但是无序，所以结合连个结构的有点进行实现。
虽然采用两个结构但是集合的元素成员和分值是共享的，两种结构通过指针指向同一地址，不会浪费内存。
```
有序集合编码转换：
有序集合对象使用ziplist编码需要满足两个条件：一是所有元素长度小于64字节；二是元素个数小于128个；不满足任意一条件将使用skiplist编码。
以上两个条件可以在Redis配置文件中修改zset-max-ziplist-entries选项和zset-max-ziplist-value选项。
 ```

7、总结
```
在Redis的五大数据对象中，string对象是唯一个可以被其他四种数据对象作为内嵌对象的；
列表（list）、哈希（hash）、集合（set）、有序集合（zset）底层实现都用到了压缩列表结构，并且使用压缩列表结构的条件都是在元素个数比较少、字节长度较短的情况下；
四种数据对象使用压缩列表的优点：
（1）节约内存，减少内存开销，Redis是内存型数据库，所以一定情况下减少内存开销是非常有必要的。
（2）减少内存碎片，压缩列表的内存块是连续的，并分配内存的次数一次即可。
（3）压缩列表的新增、删除、查找操作的平均时间复杂度是O(N)，在N再一定的范围内，这个时间几乎是可以忽略的，并且N的上限值是可以配置的。
（4）四种数据对象都有两种编码结构，灵活性增加。
``` 
2、RDB持久化
```
RDB持久化是最直接的持久化方式，直接将内存中的数据保存到RDB文件中，当恢复时也是直接从RDB文件中恢复；
RDB文件是经过压缩的二进制文件，这里对文件结构不做详解。
RDB持久化可以手动执行，也可以设置定期执行。
RDB持久化命令有两个SAVE（同步）和BGSAVE（异步），同步持久化过程中，会拒绝客户端的所有请求；异步则是创建子进程执行，不会对客户端产生影响，具体可以看上一章的命令介绍。
自动间隔性保存
因为BGSAVE命令是异步执行，不会阻塞服务器，所以Redis允许用户自行配置SAVE选项，当选项触发时自动执行BGSAVE命令。 
当用户开启了触发自动BGSAVE后，如果不配置save选项，服务器会使用默认设置，如下：
（1）在900秒内，对数据库进行了至少1次修改。
（2）在300秒内，对数据库进行了至少10次修改。
（3）在60秒内，对数据库进行了至少10000次修改。
以上三个条件，满足任意一条，就会进行BGSAVE操作
```
3、AOF持久化
```
AOF持久化与RDB不同，AOF持久化是通过记录服务器所执行的命令来保存数据的。
被写入AOF文件的所有命令都是以Redis请求协议格式保存的。
数据的还原，就是通过读取AOF文件的这些命令进行的。
执行的命名并不是直接写入AOF文件的，而是先写入缓冲区，没执行一条命令就会追加到缓冲区的末尾，当一条命令执行完成后，返回数据前，会将缓冲区的数据写入到AOF文件中。
AOF文件的载入与还原
AOF持久化的数据还原过程就是读取AOF中命令重新执行命令的过程。
（1）Redis会创建一个伪客户端，伪客户端与真实的客户端执行命令的效果是一样的，只是不带网络连接。
（2）从AOF文件分析并读取一条命令。
（3）伪客户端执行这条命令。
（4）重复2和3过程，知道AOF文件中的所有命令处理完成。
AOF文件重写
AOF文件的持久化是记录被执行对的命令，这样随着时间越来越长，AOF文件中的内容会越来越多，体积也会越来越大，
    文件越大恢复数据的时间也越多。
在命令执行的过程中有些键值对被删除了，有些被修改了，而这些过程命令是完全没有必要再执行一遍的，
所以Redis提供了AOF文件的重写功能对AOF进行重建，使用重建后的文件要比元AOF文件体积小很多。
AOF文件重写，并不需要对原AOF文件进行任何访问改动，他是通过对数据库内的数据读取来操作的，
即查看数据库内有什么数据，然后根据数据类型进行创建这些数据的写入命令。
AOF文件重写过程中，创建写入命令时会先检查元素数量，如果数量超过了redis.h/REDIS_AOF_REWRITE_ITEMS_PER_CMD=64常量的值，
就会分成多条命令，
AOF文件重写是有子进程进行的，并不影响主进程处理命令；子进程而不是线程，因为进程带有数据副本，不锁数据的情况下，能保证安全。
AOF文件子进程重写过程中，主进程仍然在处理数据，这样造成了子进程和主进程的数据不一致，子进程数据少了一部分，这种情况下Redis会创建一个AOF重写缓冲区；这样少的那部分命令会写到AOF重写缓冲区中，重写完成后，再把缓冲区这些命令写进新的AOF文件中，然后用新的AOF文件替换就得AOF文件。
4、总结
Redis初始化会创建一批数据库，每个数据库的内部数据结构都是字典，key-value的最终存储也会落到字典上。
AOF持久化比RDB持久化频率更高、速度更快；当有AOF持久化时，RDB持久化命令不会再执行；
但当RDB持久化命令执行时，AOF命令会等待其执行完成后再执行，而其他RDB命令不会执行。
AOF文件重写过程不会影响旧的AOF文件，即便AOF重写过程失败，也不会干扰原来的AOF恢复数据，只有在成功之后才会替换原来的文件。

```
redies集群
```
Redis集群详解
Redis有三种集群模式，分别是：
主从模式
Sentinel模式
Cluster模式


主从模式
主从模式是三种模式中最简单的，在主从复制中，数据库分为两类：主数据库(master)和从数据库(slave)。

其中主从复制有如下特点：
主数据库可以进行读写操作，当读写操作导致数据变化时会自动将数据同步给从数据库
从数据库一般都是只读的，并且接收主数据库同步过来的数据
一个master可以拥有多个slave，但是一个slave只能对应一个master
slave挂了不影响其他slave的读和master的读和写，重新启动后会将数据从master同步过来
master挂了以后，不影响slave的读，但redis不再提供写服务，master重启后redis将重新对外提供写服务
master挂了以后，不会在slave节点中重新选一个master
工作机制：
当slave启动后，主动向master发送SYNC命令。master接收到SYNC命令后在后台保存快照（RDB持久化）和缓存保存快照这段时间的命令，
然后将保存的快照文件和缓存的命令发送给slave。slave接收到快照文件和命令后加载快照文件和缓存的执行命令。
复制初始化后，master每次接收到的写命令都会同步发送给slave，保证主从数据一致性。
安全设置：
当master节点设置密码后，
客户端访问master需要密码
启动slave需要密码，在配置文件中配置即可
客户端访问slave不需要密码
缺点：
    从上面可以看出，master节点在主从模式中唯一，若master挂掉，则redis无法对外提供写服务。

主从模式搭建
环境准备：
master节点                  192.168.30.128
slave节点                   192.168.30.129
slave节点                   192.168.30.130

    
修改配置：
192.168.30.128
# vim /usr/local/redis/redis.conf
bind 192.168.30.128               #监听ip，多个ip用空格分隔
daemonize yes               #允许后台启动
logfile "/usr/local/redis/redis.log"                #日志路径
dir /data/redis                 #数据库备份文件存放目录
masterauth 123456               #slave连接master密码，master可省略
requirepass 123456              #设置master连接密码，slave可省略

appendonly yes                  #在/data/redis/目录生成appendonly.aof文件，将每一次写操作请求都追加到appendonly.aof 文件中

===========================================================================
192.168.30.129
# vim /usr/local/redis/redis.conf
bind 192.168.30.129
daemonize yes
logfile "/usr/local/redis/redis.log"
dir /data/redis
replicaof 192.168.30.128 6379
masterauth 123456
requirepass 123456
appendonly yes

=======================================================================================
192.168.30.130
# mkdir -p /data/redis
# vim /usr/local/redis/redis.conf
bind 192.168.30.130
daemonize yes
logfile "/usr/local/redis/redis.log"
dir /data/redis
replicaof 192.168.30.128 6379
masterauth 123456
requirepass 123456
appendonly yes

全部启动redis：
# systemctl start redis

查看集群状态：
# redis-cli -h 192.168.30.128 -a 123456

192.168.30.128:6379> info replication
# Replication
role:master
connected_slaves:2
slave0:ip=192.168.30.129,port=6379,state=online,offset=168,lag=1
slave1:ip=192.168.30.130,port=6379,state=online,offset=168,lag=1
master_replid:fb4941e02d5032ad74c6e2383211fc58963dbe90
master_replid2:0000000000000000000000000000000000000000
master_repl_offset:168
second_repl_offset:-1
repl_backlog_active:1
repl_backlog_size:1048576
repl_backlog_first_byte_offset:1

# redis-cli -h 192.168.30.129 -a 123456 info replication
# Replication
role:slave
master_host:192.168.30.128
master_port:6379
master_link_status:up
master_last_io_seconds_ago:1
master_sync_in_progress:0
slave_repl_offset:196
slave_priority:100
slave_read_only:1
connected_slaves:0
master_replid:fb4941e02d5032ad74c6e2383211fc58963dbe90
master_replid2:0000000000000000000000000000000000000000
master_repl_offset:196
second_repl_offset:-1
repl_backlog_active:1
repl_backlog_size:1048576
repl_backlog_first_byte_offset:1
repl_backlog_histlen:196
可以看到，在master节点写入的数据，很快就同步到slave节点上，而且在slave节点上无法写入数据。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Sentinel模式

主从模式的弊端就是不具备高可用性，当master挂掉以后，Redis将不能再对外提供写入操作，因此sentinel应运而生。
sentinel中文含义为哨兵，顾名思义，它的作用就是监控redis集群的运行状况，特点如下：
sentinel模式是建立在主从模式的基础上，如果只有一个Redis节点，sentinel就没有任何意义
当master挂了以后，sentinel会在slave中选择一个做为master，并修改它们的配置文件，
    其他slave的配置文件也会被修改，比如slaveof属性会指向新的master
当master重新启动后，它将不再是master而是做为slave接收新的master的同步数据
sentinel因为也是一个进程有挂掉的可能，所以sentinel也会启动多个形成一个sentinel集群
多sentinel配置的时候，sentinel之间也会自动监控
当主从模式配置密码时，sentinel也会同步将配置信息修改到配置文件中，不需要担心
一个sentinel或sentinel集群可以管理多个主从Redis，多个sentinel也可以监控同一个redis
sentinel最好不要和Redis部署在同一台机器，不然Redis的服务器挂了以后，sentinel也挂了
工作机制：
每个sentinel以每秒钟一次的频率向它所知的master，slave以及其他sentinel实例发送一个 PING 命令 
如果一个实例距离最后一次有效回复 PING 命令的时间超过 down-after-milliseconds 选项所指定的值， 
    则这个实例会被sentinel标记为主观下线。 
如果一个master被标记为主观下线，则正在监视这个master的所有sentinel要以每秒一次的频率确认master的确进入了主观下线状态
当有足够数量的sentinel（大于等于配置文件指定的值）在指定的时间范围内确认master的确进入了主观下线状态，
     则master会被标记为客观下线 
在一般情况下， 每个sentinel会以每 10 秒一次的频率向它已知的所有master，slave发送 INFO 命令 
当master被sentinel标记为客观下线时，sentinel向下线的master的所有slave发送 INFO 命令的频率会从 10 秒一次改为 1 秒一次 
若没有足够数量的sentinel同意master已经下线，master的客观下线状态就会被移除；
  若master重新向sentinel的 PING 命令返回有效回复，master的主观下线状态就会被移除

当使用sentinel模式的时候，客户端就不要直接连接Redis，而是连接sentinel的ip和port，
由sentinel来提供具体的可提供服务的Redis实现，这样当master节点挂掉以后，sentinel就会感知并将新的master节点提供给使用者。

Sentinel模式搭建
环境准备：
master节点              192.168.30.128          sentinel端口：26379

slave节点               192.168.30.129          sentinel端口：26379

slave节点               192.168.30.130          sentinel端口：26379

修改配置：
前面已经下载安装了redis，这里省略，直接修改sentinel配置文件。

192.168.30.128
# vim /usr/local/redis/sentinel.conf
daemonize yes
logfile "/usr/local/redis/sentinel.log"
dir "/usr/local/redis/sentinel"                 #sentinel工作目录
sentinel monitor mymaster 192.168.30.128 6379 2                 #判断master失效至少需要2个sentinel同意，建议设置为n/2+1，n为sentinel个数
sentinel auth-pass mymaster 123456
sentinel down-after-milliseconds mymaster 30000                 #判断master主观下线时间，默认30s

这里需要注意，sentinel auth-pass mymaster 123456需要配置在
sentinel monitor mymaster 192.168.30.128 6379 2下面，否则启动报错：

# /usr/local/bin/redis-sentinel /usr/local/redis/sentinel.conf

*** FATAL CONFIG FILE ERROR ***
Reading the configuration file, at line 104
>>> 'sentinel auth-pass mymaster 123456'
No such master with specified name.

全部启动sentinel：
# mkdir /usr/local/redis/sentinel && chown -R redis:redis /usr/local/redis
# /usr/local/bin/redis-sentinel /usr/local/redis/sentinel.conf
任一主机查看日志：
# tail -f /usr/local/redis/sentinel.log
21574:X 09 May 2019 15:32:04.298 # Sentinel ID is 30c417116a8edbab09708037366c4a7471beb770
21574:X 09 May 2019 15:32:04.298 # +monitor master mymaster 192.168.30.128 6379 quorum 2
21574:X 09 May 2019 15:32:04.299 * +slave slave 192.168.30.129:6379 192.168.30.129 6379 @ mymaster 192.168.30.128 6379
21574:X 09 May 2019 15:32:04.300 * +slave slave 192.168.30.130:6379 192.168.30.130 6379 @ mymaster 192.168.30.128 6379
21574:X 09 May 2019 15:32:16.347 * +sentinel sentinel 79b8d61626afd4d059fb5a6a63393e9a1374e78f 192.168.30.129 26379 @ mymaster 192.168.30.128 6379
21574:X 09 May 2019 15:32:31.584 * +sentinel sentinel d7b429dcba792103ef0d80827dd0910bd9284d21 192.168.30.130 26379 @ mymaster 192.168.30.128 6379

Sentinel模式下的几个事件：
reset-master ：主服务器已被重置。
slave ：一个新的从服务器已经被 Sentinel 识别并关联。
failover-state-reconf-slaves ：故障转移状态切换到了 reconf-slaves 状态。
failover-detected ：另一个 Sentinel 开始了一次故障转移操作，或者一个从服务器转换成了主服务器。
slave-reconf-sent ：领头（leader）的 Sentinel 向实例发送了 [SLAVEOF](/commands/slaveof.html) 命令，
    为实例设置新的主服务器。
slave-reconf-inprog ：实例正在将自己设置为指定主服务器的从服务器，但相应的同步过程仍未完成。
slave-reconf-done ：从服务器已经成功完成对新主服务器的同步。
-dup-sentinel ：对给定主服务器进行监视的一个或多个 Sentinel 已经因为重复出现而被移除 —— 
    当 Sentinel 实例重启的时候，就会出现这种情况。
sentinel ：一个监视给定主服务器的新 Sentinel 已经被识别并添加。
sdown ：给定的实例现在处于主观下线状态。
sdown ：给定的实例已经不再处于主观下线状态。
odown ：给定的实例现在处于客观下线状态。
odown ：给定的实例已经不再处于客观下线状态。
new-epoch ：当前的纪元（epoch）已经被更新。
try-failover ：一个新的故障迁移操作正在执行中，等待被大多数 Sentinel 选中（waiting to be elected by the majority）。
elected-leader ：赢得指定纪元的选举，可以进行故障迁移操作了。
failover-state-select-slave ：故障转移操作现在处于 select-slave 状态 —— Sentinel 正在寻找可以升级为主服务器的从服务器。
no-good-slave ：Sentinel 操作未能找到适合进行升级的从服务器。
    Sentinel 会在一段时间之后再次尝试寻找合适的从服务器来进行升级，又或者直接放弃执行故障转移操作。
selected-slave ：Sentinel 顺利找到适合进行升级的从服务器。
failover-state-send-slaveof-noone ：Sentinel 正在将指定的从服务器升级为主服务器，等待升级功能完成。
failover-end-for-timeout ：故障转移因为超时而中止，不过最终所有从服务器都会开始复制新的主服务器
    （slaves will eventually be configured to replicate with the new master anyway）。
failover-end ：故障转移操作顺利完成。所有从服务器都开始复制新的主服务器了。
switch-master ：配置变更，主服务器的 IP 和地址已经改变。 这是绝大多数外部用户都关心的信息。
tilt ：进入 tilt 模式。
tilt ：退出 tilt 模式。

master宕机演示：
192.168.30.128
# systemctl stop redis
# tail -f /usr/local/redis/sentinel.log
22428:X 09 May 2019 15:51:29.287 # +sdown master mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:29.371 # +odown master mymaster 192.168.30.128 6379 #quorum 2/2
22428:X 09 May 2019 15:51:29.371 # +new-epoch 1
22428:X 09 May 2019 15:51:29.371 # +try-failover master mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:29.385 # +vote-for-leader 30c417116a8edbab09708037366c4a7471beb770 1
22428:X 09 May 2019 15:51:29.403 # d7b429dcba792103ef0d80827dd0910bd9284d21 voted for 30c417116a8edbab09708037366c4a7471beb770 1
22428:X 09 May 2019 15:51:29.408 # 79b8d61626afd4d059fb5a6a63393e9a1374e78f voted for 30c417116a8edbab09708037366c4a7471beb770 1
22428:X 09 May 2019 15:51:29.451 # +elected-leader master mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:29.451 # +failover-state-select-slave master mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:29.528 # +selected-slave slave 192.168.30.129:6379 192.168.30.129 6379 @ mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:29.528 * +failover-state-send-slaveof-noone slave 192.168.30.129:6379 192.168.30.129 6379 @ mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:29.594 * +failover-state-wait-promotion slave 192.168.30.129:6379 192.168.30.129 6379 @ mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:30.190 # +promoted-slave slave 192.168.30.129:6379 192.168.30.129 6379 @ mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:30.190 # +failover-state-reconf-slaves master mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:30.258 * +slave-reconf-sent slave 192.168.30.130:6379 192.168.30.130 6379 @ mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:30.511 # -odown master mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:31.233 * +slave-reconf-inprog slave 192.168.30.130:6379 192.168.30.130 6379 @ mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:31.233 * +slave-reconf-done slave 192.168.30.130:6379 192.168.30.130 6379 @ mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:31.297 # +failover-end master mymaster 192.168.30.128 6379
22428:X 09 May 2019 15:51:31.297 # +switch-master mymaster 192.168.30.128 6379 192.168.30.129 6379
22428:X 09 May 2019 15:51:31.298 * +slave slave 192.168.30.130:6379 192.168.30.130 6379 @ mymaster 192.168.30.129 6379
22428:X 09 May 2019 15:51:31.298 * +slave slave 192.168.30.128:6379 192.168.30.128 6379 @ mymaster 192.168.30.129 6379
22428:X 09 May 2019 15:52:31.307 # +sdown slave 192.168.30.128:6379 192.168.30.128 6379 @ mymaster 192.168.30.129 6379
从日志中可以看到，master已经从192.168.30.128转移到192.168.30.129上

192.168.30.129上查看集群信息
# /usr/local/bin/redis-cli -h 192.168.30.129 -p 6379 -a 123456
Warning: Using a password with '-a' or '-u' option on the command line interface may not be safe.
192.168.30.129:6379> info replication
# Replication
role:master
connected_slaves:1
slave0:ip=192.168.30.130,port=6379,state=online,offset=291039,lag=1
master_replid:757aff269236ed2707ba584a86a40716c1c76d74
master_replid2:47a862fc0ff20362be29096ecdcca6d432070ee9
master_repl_offset:291182
second_repl_offset:248123
repl_backlog_active:1
repl_backlog_size:1048576
repl_backlog_first_byte_offset:1
repl_backlog_histlen:291182

192.168.30.129:6379> set key4 linux
OK
当前集群中只有一个slave——192.168.30.130，master是192.168.30.129，且192.168.30.129具有写权限。
192.168.30.130上查看redis的配置文件也可以看到replicaof 192.168.30.129 6379，这是sentinel在选举master是做的修改。
重新把192.168.30.128上进程启动
# systemctl start redis
# tail -f /usr/local/redis/sentinel.log
22428:X 09 May 2019 15:51:31.297 # +switch-master mymaster 192.168.30.128 6379 192.168.30.129 6379
22428:X 09 May 2019 15:51:31.298 * +slave slave 192.168.30.130:6379 192.168.30.130 6379 @ mymaster 192.168.30.129 6379
22428:X 09 May 2019 15:51:31.298 * +slave slave 192.168.30.128:6379 192.168.30.128 6379 @ mymaster 192.168.30.129 6379
22428:X 09 May 2019 15:52:31.307 # +sdown slave 192.168.30.128:6379 192.168.30.128 6379 @ mymaster 192.168.30.129 6379
22428:X 09 May 2019 16:01:24.872 # -sdown slave 192.168.30.128:6379 192.168.30.128 6379 @ mymaster 192.168.30.129 6379
查看集群信息

# /usr/local/bin/redis-cli -h 192.168.30.128 -p 6379 -a 123456
Warning: Using a password with '-a' or '-u' option on the command line interface may not be safe.

192.168.30.128:6379> info replication
# Replication
role:slave
master_host:192.168.30.129
master_port:6379
master_link_status:up
master_last_io_seconds_ago:0
master_sync_in_progress:0
slave_repl_offset:514774
slave_priority:100
slave_read_only:1
connected_slaves:0
master_replid:757aff269236ed2707ba584a86a40716c1c76d74
master_replid2:0000000000000000000000000000000000000000
master_repl_offset:514774
second_repl_offset:-1
repl_backlog_active:1
repl_backlog_size:1048576
repl_backlog_first_byte_offset:376528
repl_backlog_histlen:138247

192.168.30.128:6379> get key4
"linux"

192.168.30.128:6379> set key5
(error) ERR wrong number of arguments for 'set' command
即使192.168.30.128重新启动redis服务，也是作为slave加入redis集群，192.168.30.129仍然是master。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Cluster模式

sentinel模式基本可以满足一般生产的需求，具备高可用性。但是当数据量过大到一台服务器存放不下的情况时，
主从模式或sentinel模式就不能满足需求了，这个时候需要对存储的数据进行分片，将数据存储到多个Redis实例中。
cluster模式的出现就是为了解决单机Redis容量有限的问题，将Redis的数据根据一定的规则分配到多台机器。
cluster可以说是sentinel和主从模式的结合体，通过cluster可以实现主从和master重选功能，所以如果配置两个副本三个分片的话，
就需要六个Redis实例。因为Redis的数据是根据一定规则分配到cluster的不同机器的，当数据量过大时，可以新增机器进行扩容。
使用集群，只需要将redis配置文件中的cluster-enable配置打开即可。每个集群中至少需要三个主数据库才能正常运行，新增节点非常方便。
cluster集群特点：
多个redis节点网络互联，数据共享
所有的节点都是一主一从（也可以是一主多从），其中从不提供服务，仅作为备用
不支持同时处理多个key（如MSET/MGET），因为redis需要把key均匀分布在各个节点上，
  并发量很高的情况下同时创建key-value会降低性能并导致不可预测的行为
支持在线增加、删除节点
客户端可以连接任何一个主节点进行读写

Cluster模式搭建
环境准备：
三台机器，分别开启两个redis服务（端口）
192.168.30.128              端口：7001,7002
192.168.30.129              端口：7003,7004
192.168.30.130              端口：7005,7006

修改配置文件：
192.168.30.128
# mkdir /usr/local/redis/cluster
# cp /usr/local/redis/redis.conf /usr/local/redis/cluster/redis_7001.conf
# cp /usr/local/redis/redis.conf /usr/local/redis/cluster/redis_7002.conf
# chown -R redis:redis /usr/local/redis
# mkdir -p /data/redis/cluster/{redis_7001,redis_7002} && chown -R redis:redis /data/redis
# vim /usr/local/redis/cluster/redis_7001.conf

bind 192.168.30.128
port 7001
daemonize yes
pidfile "/var/run/redis_7001.pid"
logfile "/usr/local/redis/cluster/redis_7001.log"
dir "/data/redis/cluster/redis_7001"
#replicaof 192.168.30.129 6379
masterauth 123456
requirepass 123456
appendonly yes
cluster-enabled yes
cluster-config-file nodes_7001.conf
cluster-node-timeout 15000
# vim /usr/local/redis/cluster/redis_7002.conf

bind 192.168.30.128
port 7002
daemonize yes
pidfile "/var/run/redis_7002.pid"
logfile "/usr/local/redis/cluster/redis_7002.log"
dir "/data/redis/cluster/redis_7002"
#replicaof 192.168.30.129 6379
masterauth "123456"
requirepass "123456"
appendonly yes
cluster-enabled yes
cluster-config-file nodes_7002.conf
cluster-node-timeout 15000
其它两台机器配置与192.168.30.128一致，此处省略

启动redis服务：
# redis-server /usr/local/redis/cluster/redis_7001.conf
# tail -f /usr/local/redis/cluster/redis_7001.log
# redis-server /usr/local/redis/cluster/redis_7002.conf
# tail -f /usr/local/redis/cluster/redis_7002.log
其它两台机器启动与192.168.30.128一致，此处省略

安装ruby并创建集群（低版本）：
如果redis版本比较低，则需要安装ruby。任选一台机器安装ruby即可
# yum -y groupinstall "Development Tools"
# yum install -y gdbm-devel libdb4-devel libffi-devel libyaml libyaml-devel ncurses-devel openssl-devel readline-devel tcl-devel

# mkdir -p ~/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}

# wget http://cache.ruby-lang.org/pub/ruby/2.2/ruby-2.2.3.tar.gz -P ~/rpmbuild/SOURCES

# wget http://raw.githubusercontent.com/tjinjin/automate-ruby-rpm/master/ruby22x.spec -P ~/rpmbuild/SPECS

# rpmbuild -bb ~/rpmbuild/SPECS/ruby22x.spec

# rpm -ivh ~/rpmbuild/RPMS/x86_64/ruby-2.2.3-1.el7.x86_64.rpm

# gem install redis                 #目的是安装这个，用于配置集群
# cp /usr/local/redis/src/redis-trib.rb /usr/bin/

# redis-trib.rb create --replicas 1 192.168.30.128:7001 192.168.30.128:7002 192.168.30.129:7003 192.168.30.129:7004 192.168.30.130:7005 192.168.30.130:7006
创建集群：
我这里是redis5.0.4，所以不需要安装ruby，直接创建集群即可
# redis-cli -a 123456 --cluster create 192.168.30.128:7001 192.168.30.128:7002 192.168.30.129:7003 192.168.30.129:7004 192.168.30.130:7005 192.168.30.130:7006 --cluster-replicas 1
Warning: Using a password with '-a' or '-u' option on the command line interface may not be safe.
>>> Performing hash slots allocation on 6 nodes...
Master[0] -> Slots 0 - 5460
Master[1] -> Slots 5461 - 10922
Master[2] -> Slots 10923 - 16383
Adding replica 192.168.30.129:7004 to 192.168.30.128:7001
Adding replica 192.168.30.130:7006 to 192.168.30.129:7003
Adding replica 192.168.30.128:7002 to 192.168.30.130:7005
M: 80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001
   slots:[0-5460] (5461 slots) master
S: b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002
   replicates 6788453ee9a8d7f72b1d45a9093838efd0e501f1
M: 4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003
   slots:[5461-10922] (5462 slots) master
S: b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004
   replicates 80c80a3f3e33872c047a8328ad579b9bea001ad8
M: 6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005
   slots:[10923-16383] (5461 slots) master
S: 277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006
   replicates 4d74ec66e898bf09006dac86d4928f9fad81f373
Can I set the above configuration? (type 'yes' to accept): yes                  #输入yes，接受上面配置
>>> Nodes configuration updated
>>> Assign a different config epoch to each node
>>> Sending CLUSTER MEET messages to join the cluster

可以看到，

192.168.30.128:7001是master，它的slave是192.168.30.129:7004；

192.168.30.129:7003是master，它的slave是192.168.30.130:7006；

192.168.30.130:7005是master，它的slave是192.168.30.128:7002
自动生成nodes.conf文件：

# ls /data/redis/cluster/redis_7001/
appendonly.aof  dump.rdb  nodes-7001.conf

# vim /data/redis/cluster/redis_7001/nodes-7001.conf 

6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557454406312 5 connected 10923-16383
277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557454407000 6 connected
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557454408371 5 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 myself,master - 0 1557454406000 1 connected 0-5460
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557454407366 4 connected
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557454407000 3 connected 5461-10922
vars currentEpoch 6 lastVoteEpoch 0
集群操作
登录集群：
# redis-cli -c -h 192.168.30.128 -p 7001 -a 123456                  # -c，使用集群方式登录
1
查看集群信息：
192.168.30.128:7001> CLUSTER INFO                   #集群状态

cluster_state:ok
cluster_slots_assigned:16384
cluster_slots_ok:16384
cluster_slots_pfail:0
cluster_slots_fail:0
cluster_known_nodes:6
cluster_size:3
cluster_current_epoch:6
cluster_my_epoch:1
cluster_stats_messages_ping_sent:580
cluster_stats_messages_pong_sent:551
cluster_stats_messages_sent:1131
cluster_stats_messages_ping_received:546
cluster_stats_messages_pong_received:580
cluster_stats_messages_meet_received:5
cluster_stats_messages_received:1131
列出节点信息：
192.168.30.128:7001> CLUSTER NODES                  #列出节点信息

6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557455176000 5 connected 10923-16383
277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557455174000 6 connected
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557455175000 5 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 myself,master - 0 1557455175000 1 connected 0-5460
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557455174989 4 connected
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557455175995 3 connected 5461-10922
这里与nodes.conf文件内容相同
写入数据：
192.168.30.128:7001> set key111 aaa
-> Redirected to slot [13680] located at 192.168.30.130:7005                #说明数据到了192.168.30.130:7005上
OK

192.168.30.130:7005> set key222 bbb
-> Redirected to slot [2320] located at 192.168.30.128:7001                 #说明数据到了192.168.30.128:7001上
OK

192.168.30.128:7001> set key333 ccc
-> Redirected to slot [7472] located at 192.168.30.129:7003                 #说明数据到了192.168.30.129:7003上
OK

192.168.30.129:7003> get key111
-> Redirected to slot [13680] located at 192.168.30.130:7005
"aaa"

192.168.30.130:7005> get key333
-> Redirected to slot [7472] located at 192.168.30.129:7003
"ccc"

192.168.30.129:7003> 

可以看出redis cluster集群是去中心化的，每个节点都是平等的，连接哪个节点都可以获取和设置数据。

当然，平等指的是master节点，因为slave节点根本不提供服务，只是作为对应master节点的一个备份。

增加节点：
192.168.30.129上增加一节点：

# cp /usr/local/redis/cluster/redis_7003.conf /usr/local/redis/cluster/redis_7007.conf

# vim /usr/local/redis/cluster/redis_7007.conf

bind 192.168.30.129
port 7007
daemonize yes
pidfile "/var/run/redis_7007.pid"
logfile "/usr/local/redis/cluster/redis_7007.log"
dir "/data/redis/cluster/redis_7007"
#replicaof 192.168.30.129 6379
masterauth "123456"
requirepass "123456"
appendonly yes
cluster-enabled yes
cluster-config-file nodes_7007.conf
cluster-node-timeout 15000

# mkdir /data/redis/cluster/redis_7007

# chown -R redis:redis /usr/local/redis && chown -R redis:redis /data/redis

# redis-server /usr/local/redis/cluster/redis_7007.conf 

192.168.30.130上增加一节点：

# cp /usr/local/redis/cluster/redis_7005.conf /usr/local/redis/cluster/redis_7008.conf

# vim /usr/local/redis/cluster/redis_7007.conf

bind 192.168.30.130
port 7008
daemonize yes
pidfile "/var/run/redis_7008.pid"
logfile "/usr/local/redis/cluster/redis_7008.log"
dir "/data/redis/cluster/redis_7008"
#replicaof 192.168.30.130 6379
masterauth "123456"
requirepass "123456"
appendonly yes
cluster-enabled yes
cluster-config-file nodes_7008.conf
cluster-node-timeout 15000

# mkdir /data/redis/cluster/redis_7008

# chown -R redis:redis /usr/local/redis && chown -R redis:redis /data/redis

# redis-server /usr/local/redis/cluster/redis_7008.conf 

集群中增加节点：

192.168.30.129:7003> CLUSTER MEET 192.168.30.129 7007
OK

192.168.30.129:7003> CLUSTER NODES

4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 myself,master - 0 1557457361000 3 connected 5461-10922
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 master - 0 1557457364746 1 connected 0-5460
277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557457362000 6 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557457363000 4 connected
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557457362000 5 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557457362729 0 connected
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557457363739 5 connected 10923-16383

192.168.30.129:7003> CLUSTER MEET 192.168.30.130 7008
OK

192.168.30.129:7003> CLUSTER NODES

4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 myself,master - 0 1557457489000 3 connected 5461-10922
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 master - 0 1557457489000 1 connected 0-5460
277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557457489000 6 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557457488000 4 connected
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557457489472 5 connected
1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 master - 0 1557457489259 0 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557457489000 0 connected
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557457490475 5 connected 10923-16383
可以看到，新增的节点都是以master身份加入集群的

更换节点身份：
将新增的192.168.30.130:7008节点身份改为192.168.30.129:7007的slave

# redis-cli -c -h 192.168.30.130 -p 7008 -a 123456 cluster replicate e51ab166bc0f33026887bcf8eba0dff3d5b0bf14
1
cluster replicate后面跟node_id，更改对应节点身份。也可以登入集群更改

# redis-cli -c -h 192.168.30.130 -p 7008 -a 123456

192.168.30.130:7008> CLUSTER REPLICATE e51ab166bc0f33026887bcf8eba0dff3d5b0bf14
OK

192.168.30.130:7008> CLUSTER NODES

277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557458316881 3 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 master - 0 1557458314864 1 connected 0-5460
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557458316000 3 connected 5461-10922
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557458315872 5 connected 10923-16383
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557458317890 5 connected
1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 myself,slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557458315000 7 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557458315000 1 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557458314000 0 connected

查看相应的nodes.conf文件，可以发现有更改，它记录当前集群的节点信息

# cat /data/redis/cluster/redis_7001/nodes-7001.conf

1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557458236169 7 connected
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557458235000 5 connected 10923-16383
277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557458234103 6 connected
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557458235129 5 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 myself,master - 0 1557458234000 1 connected 0-5460
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557458236000 4 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557458236000 0 connected
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557458233089 3 connected 5461-10922
vars currentEpoch 7 lastVoteEpoch 0

删除节点：
192.168.30.130:7008> CLUSTER FORGET 1a1c7f02fce87530bd5abdfc98df1cffce4f1767
(error) ERR I tried hard but I can't forget myself...               #无法删除登录节点

192.168.30.130:7008> CLUSTER FORGET e51ab166bc0f33026887bcf8eba0dff3d5b0bf14
(error) ERR Can't forget my master!                 #不能删除自己的master节点

192.168.30.130:7008> CLUSTER FORGET 6788453ee9a8d7f72b1d45a9093838efd0e501f1
OK              #可以删除其它的master节点

192.168.30.130:7008> CLUSTER NODES

277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557458887328 3 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 master - 0 1557458887000 1 connected 0-5460
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557458886000 3 connected 5461-10922
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave - 0 1557458888351 5 connected
1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 myself,slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557458885000 7 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557458883289 1 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557458885310 0 connected

192.168.30.130:7008> CLUSTER FORGET b4d3eb411a7355d4767c6c23b4df69fa183ef8bc
OK              #可以删除其它的slave节点

192.168.30.130:7008> CLUSTER NODES

277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557459031397 3 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 master - 0 1557459032407 1 connected 0-5460
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557459035434 3 connected 5461-10922
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557459034000 5 connected 10923-16383
1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 myself,slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557459032000 7 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557459034000 1 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557459034427 0 connected

保存配置：
192.168.30.130:7008> CLUSTER SAVECONFIG                 #将节点配置信息保存到硬盘
OK
 
# cat /data/redis/cluster/redis_7001/nodes-7001.conf

1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557458236169 7 connected
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557458235000 5 connected 10923-16383
277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557458234103 6 connected
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557458235129 5 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 myself,master - 0 1557458234000 1 connected 0-5460
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557458236000 4 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557458236000 0 connected
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557458233089 3 connected 5461-10922
vars currentEpoch 7 lastVoteEpoch 0

# redis-cli -c -h 192.168.30.130 -p 7008 -a 123456
Warning: Using a password with '-a' or '-u' option on the command line interface may not be safe.

192.168.30.130:7008> CLUSTER NODES
277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557459500741 3 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 master - 0 1557459500000 1 connected 0-5460
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557459501000 3 connected 5461-10922
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557459500000 5 connected 10923-16383
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557459499737 5 connected
1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 myself,slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557459499000 7 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 slave 80c80a3f3e33872c047a8328ad579b9bea001ad8 0 1557459501750 1 connected
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557459498000 0 connected
可以看到，之前删除的节点又恢复了，这是因为对应的配置文件没有删除，执行CLUSTER SAVECONFIG恢复。

模拟master节点挂掉：
192.168.30.128

# netstat -lntp |grep 7001
tcp        0      0 192.168.30.128:17001    0.0.0.0:*               LISTEN      6701/redis-server 1 
tcp        0      0 192.168.30.128:7001     0.0.0.0:*               LISTEN      6701/redis-server 1 

# kill 6701

192.168.30.130:7008> CLUSTER NODES

277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557461178000 3 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 master,fail - 1557460950483 1557460947145 1 disconnected
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557461174922 3 connected 5461-10922
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557461181003 5 connected 10923-16383
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557461179993 5 connected
1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 myself,slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557461176000 7 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 master - 0 1557461178981 8 connected 0-5460
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557461179000 0 connected

对应7001的一行可以看到，master fail，状态为disconnected；而对应7004的一行，slave已经变成master。

重新启动7001节点：
# redis-server /usr/local/redis/cluster/redis_7001.conf

192.168.30.130:7008> CLUSTER NODES

277daeb8660d5273b7c3e05c263f861ed5f17b92 192.168.30.130:7006@17006 slave 4d74ec66e898bf09006dac86d4928f9fad81f373 0 1557461307000 3 connected
80c80a3f3e33872c047a8328ad579b9bea001ad8 192.168.30.128:7001@17001 slave b6331cbc986794237c83ed2d5c30777c1551546e 0 1557461305441 8 connected
4d74ec66e898bf09006dac86d4928f9fad81f373 192.168.30.129:7003@17003 master - 0 1557461307962 3 connected 5461-10922
6788453ee9a8d7f72b1d45a9093838efd0e501f1 192.168.30.130:7005@17005 master - 0 1557461304935 5 connected 10923-16383
b4d3eb411a7355d4767c6c23b4df69fa183ef8bc 192.168.30.128:7002@17002 slave 6788453ee9a8d7f72b1d45a9093838efd0e501f1 0 1557461306000 5 connected
1a1c7f02fce87530bd5abdfc98df1cffce4f1767 192.168.30.130:7008@17008 myself,slave e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 0 1557461305000 7 connected
b6331cbc986794237c83ed2d5c30777c1551546e 192.168.30.129:7004@17004 master - 0 1557461308972 8 connected 0-5460
e51ab166bc0f33026887bcf8eba0dff3d5b0bf14 192.168.30.129:7007@17007 master - 0 1557461307000 0 connected

可以看到，7001节点启动后为slave节点，并且是7004的slave节点。即master节点如果挂掉，它的slave节点变为新master节点继续对外提供服务，而原来的master节点如果重启，则变为新master节点的slave节点。

另外，如果这里是拿7007节点做测试的话，会发现7008节点并不会切换，这是因为7007节点上根本没数据。集群数据被分为三份，采用哈希槽 (hash slot)的方式来分配16384个slot的话，它们三个节点分别承担的slot 区间是：

节点7004覆盖0－5460
节点7003覆盖5461－10922
节点7005覆盖10923－16383
三张图秒懂Redis集群设计原理 
Redis集群设计包括2部分：哈希Slot和节点主从，本篇博文通过3张图来搞明白Redis的集群设计。想扩展并发读就添加Slaver，想扩展并发写就添加Master，想扩容也就是添加Master，任何一个Slaver或者几个Master挂了都不会是灾难性的故障。..
```


 如果在setnx之后执行expire之前进程意外crash或者要重启维护了，那会怎么样？
 ```
    set指令有非常复杂的参数，这个应该是可以同时把setnx和expire合成一条指令来用的！
 ```
45.Redis做异步队列一般使用list结构作为队列，rpush生产消息，lpop消费消息。当lpop没有消息的时候，要适当sleep一会再重试。
```
    缺点：在消费者下线的情况下，生产的消息会丢失，得使用专业的消息队列如rabbitmq等。

```
能不能生产一次消费多次呢？
```
    使用pub/sub主题订阅者模式，可以实现1:N的消息队列。

```

51.多节点 Redis 分布式锁：
```
    Redlock 算法获取当前时间（start）。依次向 N 个 Redis节点请求锁。请求锁的方式与从单节点 Redis获取锁的方式一致。
    为了保证在某个 Redis节点不可用时该算法能够继续运行，获取锁的操作都需要设置超时时间，需要保证该超时时间远小于锁的有效时间。
    这样才能保证客户端在向某个 Redis节点获取锁失败之后，可以立刻尝试下一个节点。计算获取锁的过程总共消耗多长时间（consumeTime = end - start）。
    如果客户端从大多数 Redis节点（>= N/2 + 1) 成功获取锁，并且获取锁总时长没有超过锁的有效时间，这种情况下，客户端会认为获取锁成功，否则，获取锁失败。
    如果最终获取锁成功，锁的有效时间应该重新设置为锁最初的有效时间减去 consumeTime。如果最终获取锁失败，客户端应该立刻向所有 Redis节点发起释放锁的请求。
```

52.Redis 中设置过期时间主要通过以下四种方式
```
    expire key seconds：设置 key 在 n 秒后过期；
    pexpire key milliseconds：设置 key 在 n 毫秒后过期；
    expireat key timestamp：设置 key 在某个时间戳（精确到秒）之后过期；
    pexpireat key millisecondsTimestamp：设置 key 在某个时间戳（精确到毫秒）之后过期；
```
53.Reids三种不同删除策略定时删除：
```
    在设置键的过期时间的同时，创建一个定时任务，当键达到过期时间时，立即执行对键的删除操作惰性删除：
    放任键过期不管，但在每次从键空间获取键时，都检查取得的键是否过期，如果过期的话，就删除该键，如果没有过期，就返回该键定期删除：
    每隔一点时间，程序就对数据库进行一次检查，删除里面的过期键，至于要删除多少过期键，以及要检查多少个数据库，则由算法决定。
```

54.定时删除
```
优点：对内存友好，定时删除策略可以保证过期键会尽可能快地被删除，并释放国期间所占用的内存
缺点：对cpu时间不友好，在过期键比较多时，删除任务会占用很大一部分cpu时间，在内存不紧张但cpu时间紧张的情况下，
      将cpu时间用在删除和当前任务无关的过期键上，影响服务器的响应时间和吞吐量

定期删除由于定时删除会占用太多cpu时间，影响服务器的响应时间和吞吐量以及惰性删除浪费太多内存，有内存泄露的危险，所以出现一种整合和折中这两种策略的定期删除策略。
定期删除策略每隔一段时间执行一次删除过期键操作，并通过限制删除操作执行的时长和频率来减少删除操作对CPU时间的影响。定时删除策略有效地减少了因为过期键带来的内存浪费。
```
56.惰性删除
```
优点：对cpu时间友好，在每次从键空间获取键时进行过期键检查并是否删除，删除目标也仅限当前处理的键，这个策略不会在其他无关的删除任务上花费任何cpu时间。
缺点：对内存不友好，过期键过期也可能不会被删除，导致所占的内存也不会释放。甚至可能会出现内存泄露的现象，当存在很多过期键，而这些过期键又没有被访问到，
        这会可能导致它们会一直保存在内存中，造成内存泄露。
```


58.Redis常见的几种缓存策略
```
Cache-AsideRead-ThroughWrite-ThroughWrite-Behind

```
59.Redis Module 实现布隆过滤器Redis module 
```
是Redis 4.0 以后支持的新的特性，这里很多国外牛逼的大学和机构提供了很多牛逼的Module 只要编译引入到Redis 中就能轻松的实现我们某些需求的功能。
在Redis 官方Module 中有一些我们常见的一些模块，我们在这里就做一个简单的使用。
neural-redis 主要是神经网络的机器学，集成到redis 可以做一些机器训练感兴趣的可以尝试
RedisSearch 主要支持一些富文本的的搜索
RedisBloom 支持分布式环境下的Bloom 过滤器
```

60.Redis 到底是怎么实现“附近的人”使用方式GEOADD key longitude latitude member [longitude latitude member ...]
```
将给定的位置对象（纬度、经度、名字）添加到指定的key。
其中，key为集合名称，member为该经纬度所对应的对象。
在实际运用中，当所需存储的对象数量过多时，可通过设置多key(如一个省一个key)的方式对对象集合变相做sharding，避免单集合数量过多。
成功插入后的返回值：(integer) N其中N为成功插入的个数。
```


hash表的实现，包括STL中的哈希桶长度常数。
```
hash表的实现主要涉及两个问题：散列函数和碰撞处理。
    1）hash function （散列函数）。最常见的散列函数：f(x) = x % TableSize 
    .2）碰撞问题（不同元素的散列值相同）。解决碰撞问题的方法有许多种，包括线性探测、二次探测、开链等做法。
        SGL版本使用开链法，使用一个链表保持相同散列值的元素。虽然开链法并不要求表格大小必须为质数，但SGI STL仍然以质数来设计表格大小，并且将28个质数（逐渐呈现大约两倍的关系）计算好，以备随时访问，同时提供一个函数，用来查询在这28个质数之中，“最接近某数并大于某数”的质数。
```


9、hash表如何rehash，怎么处理其中保存的资源
```
    先想想为什么需要rehash:因为，当loadFactor（负载因子）<=1时，hash表查找的期望复杂度为O(1). 因此，每次往hash表中添加元素时，我们必须保证是在loadFactor <1的情况下，才能够添加。模仿C++的vector扩容方式，Hash表中每次发现loadFactor==1时，就开辟一个原来桶数组的两倍空间（称为新桶数组），然后把原来的桶数组中元素全部转移过来到新的桶数组中。注意这里转移是需要元素一个个重新哈希到新桶中的。
10、Redis的rehash怎么做的，为什么要渐进rehash，渐进rehash怎么实现的为了避免rehash对服务器造成影响，服务器不是一次将ht[0]里面的所有键值对全部rehash到ht[1]，而是分多次、渐进式地将ht[0]里面的键值对慢慢地rehash到ht[1].
```


Redis服务器是一个事件驱动程序，服务器需要处理以下两类事件：
```
文件事件（服务器对套接字操作的抽象）和时间事件（服务器对定时操作的抽象）。Redis的定时机制就是借助时间事件实现的。一个时间事件主要由以下三个属性组成：id：时间事件标识号；when：记录时间事件的到达时间；timeProc：时间事件处理器，当时间事件到达时，服务器就会调用相应的处理器来处理时间。一个时间事件根据时间事件处理器的返回值来判断是定时事件还是周期性事件。
```



Reactor模型：
```
    1）Handle：即操作系统中的句柄，是对资源在操作系统层面上的一种抽象，它可以是打开的文件、一个连接(Socket)、Timer等。
        由于Reactor模式一般使用在网络编程中，因而这里一般指Socket Handle，即一个网络连接。
    2）Synchronous Event Demultiplexer（同步事件复用器）：阻塞等待一系列的Handle中的事件到来，如果阻塞等待返回，即表示在返回的Handle中可以不阻塞的执行返回的事件类型。这个模块一般使用操作系统的select来实现。
    3）Initiation Dispatcher：用于管理Event Handler，即EventHandler的容器，用以注册、移除EventHandler等；另外，它还作为Reactor模式的入口调用Synchronous Event Demultiplexer的select方法以阻塞等待事件返回，当阻塞等待返回时，根据事件发生的Handle将其分发给对应的Event Handler处理，即回调EventHandler中的handle_event()方法。
    4）Event Handler：定义事件处理方法：handle_event()，以供InitiationDispatcher回调使用。
    5）Concrete Event Handler：事件EventHandler接口，实现特定事件处理逻辑。
```

单核机器上写多线程程序，是否需要考虑加锁，为什么？
线程需要保存哪些上下文，SP、PC、EAX这些寄存器是干嘛用的

质数比合数更容易避免冲撞，也就是说使用质数时，哈希效果更好，原始数据经哈希后分布更均匀。
redis的主从复制怎么做的

Redis旧版复制功能只有同步和命令传播。新版复制功能加入了部分同步的功能。
```
）命令传播：当主服务器会将自己执行的写命令，也即是造成主从服务器不一致的那条写命令，发送给从服务器执行，当从服务器执行了相同的写命令之后，主从服务器将再次回到一致状态。3）部分同步：（断线后重复制）复制偏移量：通过对比主从服务器的复制偏移量，程序可以很容易地知道主从服务器是否处于一致状态。复制积压缓冲区：主服务保存最近的写命令到复制积压缓冲区，是一个先进先出队列服务器运行ID：从服务器记录上次同步的主服务器的Id。
```



### 扩展问题
如何把一个文件快速下发到100w个服务器

gossip算法？Gossip有众多的别名“闲话算法”、“疫情传播算法”、“病毒感染算法”、“谣言传播算法”。
如何判断一个图是否连同？

DFS、BFS、并查集

3. 如何实现一个引用计数指针，以及其中要注意的点？
```
这也是一个比较有内容的问题。大致说了一下方案，面试官又追问了诸如入侵性与非入侵性设计的区别、优劣，以及析构对象时多动态库架构情况下本地堆问题，
还有引用计数的多线程安全问题，有锁怎么实现，无锁怎么实现。

```
5. 为什么stl中的内存分配器要设计为一个模板参数而不是一个构造函数参数？这个就属于瞎聊了，各抒己见呗。最后扯到类型系统如何帮助程序员排错之类的问题。

