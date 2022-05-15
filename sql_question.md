数据库三范式
```
第一范式:
    确保每列的原子性,每列都是不可分割的最小数据单元
第二范式:
    在第一范式的基础上,要求每列都和主键相关
第三范式:
    在第二范式的基础上,要求其他列和主键是直接相关,而不是间接相关
范式化优点:
    减少数据冗余
    表中重复数据较少，更新操作比较快
    范式化的表通常比反范式化的表小
范式化缺点:
    在查询的时候通常需要很多的关联，降低性能
    增加了索引优化的难度
反范式化优点：
    可以减少表的关联
    更好的进行索引优化
反范式化缺点:
    数据重复冗余
    对数据表的修改需要更多的成本
```
B+树和B树的区别
```
1 B+树非叶子节点只存储关键字和指向子节点的指针,而B树还存储了数据,在同样大小的情况下,B+树可以存储更多的关键字
2 B+树叶子节点存储了所有关键字和数据，并且多个节点用链表连接。可以快速支撑范围查找
3 B+树非叶子节点不存储数据，所以查询时间复杂度固定为O(logN)，B树查询时间复杂度不固定，最好是O(1)

B+树比B树更适合应用于数据库索引原因:
    因为B树叶子节点和非叶子结点都存储了数据,这样就导致了非叶子结点能保存的关键字和指针变少,
    如果要保存大量数据,只能增加树的高度,导致IO操作变多,查询性能降低
```
聚簇索引和非聚簇索引
```
```
前缀索引和覆盖索引
```
```
数据库的事务
```
ACID是什么
隔离级别:
    脏读,不可重复读,欢度
mysql事务如何实现:
    undo log + redo log
Binlog和Redo log的区别和作用
谈一谈 MVCC 多版本并发控制
---------------------------------------------
原子性:
    通过undo log实现的,每条数据变更都伴随一条undo log日志的生成,
    当系统发生错误或执行回滚根据undo log做逆向操作
持久性:
    通过redo log实现的,redo log记录了数据的修改日志;数据持久化到磁盘,先是储存到缓冲池里,然后缓冲池中的数据定期同步到磁盘中;
    如果系统宕机,可能会丢失数据,系统重启后会读取redo log恢复数据

隔离性:
    mysql数据库通过MVCC + next-key机制实现了隔离性
一致性：以上3大特性，保障了事务的一致性
------------------------------------------------
binlog是二进制文件,记录了对数据库执行更改的所有操作,不包括 select、show;
因为这两个操作没有对数据本身做修改,但是若操作了数据,但是数据没有发生变化,也会记录到binlog;
常用来数据恢复，数据备份。
redo log又叫做重做日志文件,记录了事务的修改,不管事务是否提交都记录下来;
在实例和介质失败时,InnoDB存储引擎会使用redo log恢复到之前的状态，保证数据的完整性
------------------------------------
MVCC是通过在每行记录后面保存两个隐藏的列来实现的
这两个列,一个保存了行的创建时间,一个保存行的过期时间(或删除时间)
当然存储的并不是实际的时间值,而是系统版本号(system version number);
每开始一个新的事务,系统版本号都会自动递增;事务开始时刻的系统版本号会作为事务的版本号
用来和查询到的每行记录的版本号进行比较
SELECT
    InnoDB会根据以下两个条件检查每行记录：
    InnoDB只查找版本早于当前事务版本的数据行(也就是，行的系统版本号小于或等于事务的系统版本号)，
    这样可以确保事务读取的行，要么是在事务开始前已经存在的，要么是事务自身插入或者修改过的。
    行的删除版本要么未定义，要么大于当前事务版本号。这可以确保事务读取到的行，在事务开始之前未被删除。
    只有符合上述两个条件的记录，才能返回作为查询结果
INSERT
    InnoDB为新插入的每一行保存当前系统版本号作为行版本号。
DELETE
    InnoDB为删除的每一行保存当前系统版本号作为行删除标识。
UPDATE:
    InnoDB为插入一行新记录，保存当前系统版本号作为行版本号，同时保存当前系统版本号到原来的行作为行删除标识
```
Innodb 和 MyISAM 的区别
```
Innodb 支持事务。MyISAM 不支持
Innodb 支持外键。MyISAM 不支持
Innodb 主键索引的叶子节点是数据文件，辅助索引的叶子节点是主键的值。MyISAM 的主键索引和辅助索引，叶子节点都是数据文件的指针
Innodb 不保存表的行数，执行 select count(*) from tb需要全表扫描。MyISAM 用一个变量保存了整个表的行数，执行上述语句只需要读取该变量，速度很快
Innodb 所有的表在磁盘上保存在一个文件中。MyISAM 存储成三个文件。
Innodb 需要更多的内存和存储。MyISAM 可被压缩，存储空间较小。
Innodb 移植方案拷贝文件、备份 binlog，或者用 mysqldump，移植较困难。MyISAM 数据以文件形式存储，在备份和回复时可以单独针对表进行操作
Innodb 支持行锁、表锁。MyISAM 支持表锁
Innodb 在5.7版本之前不支持全文索引。MyISAM 支持全文索引
```
Innodb 的默认加锁方式是什么，是怎么实现的
```
Innodb默认加锁方式是行级锁
通过给索引上的索引项加锁来实现的
```
为什么使用数据库索引可以提高效率
在什么情况下会用不到数据库索引？
```
默认执行SQL语句是进行全表扫描,遇到匹配条件的就加入搜索结果合集
如果有索引,就会先去索引表中一次定位到特定值的行数,减少遍历匹配的行数
索引把无序的数据变成了相对有序的数据结构
```

共享锁和排他锁的使用场景
```
共享锁和排他锁的使用场景
更新、新增、删除默认加排它锁，查询默认不加锁
共享锁:
    使用语法select * from tb lock in share mode，自身可以读,
    其他事务也可以读(也可以继续加共享锁)，但是其他事务无法修改
排它锁:
    适用语法select * from tb for update，自身可以进行增删改查
    其他事务无法进行任何操作
```
Mysql 什么情况会造成慢查，如何查看慢查询
```
响应时间超过阈值会产生慢查询日志
一般有以下情况会造成查询慢:
    1 没有设置索引，或查询没有用到索引
    2 I/O吞吐量过小
    3 内存不足
    4 网络速度慢
    5 查询的数据量过大
    6 锁或者死锁
    7 返回了不必要的行或列
-----------------------------
````
如何处理慢查询，你一般是怎么处理慢查询的
```
    1 把数据,日志,索引放到不同的I/O设备上，增加读取速度
    2 纵向、横向分割表，减少表的尺寸
    3 升级硬件
    4 根据查询条件，建立索引，索引优化
    5 提高网速
    6 扩大服务器内存
```
数据库外键的优缺点
```
优点:
    能最大限度的保证数据的一致性和完整性
    增加ER图的可读性
缺点:
    影响数据操作的效率
    增加开发难度，导致表过多
```

b+树 b树区别,为什么不使用B树做索引
http请求流程输入url
微信朋友圈设计

 线上排障
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 索引及索引优化
```
B+Tree
--------------------------------------------------------
定义:
    一种树数据结构，是一个n叉树，每个节点通常有多个孩子
特点:
    1、数据都存储在叶子节点，并且每个叶子节点的数据都是按
    相同顺序排列存储的
    2、相邻的叶子节点都用指针连接在一点
查找过程:
    通过二分查找从上到下逐层查找，最终找到对应的叶子节点
性质:
    1、 IO次数取决于b+数的高度h，假设当前数据表的数据为N，
        每个磁盘块的数据项的数量是m，则有h=㏒(m+1)N，
        当数据量N一定的情况下，m越大，h越小
    2、 m = 磁盘块的大小/数据项的大小，磁盘块的大小也就是一个数据页的大小
        ，是固定的，如果数据项占的空间越小，数据项的数量越多，树的高度越低
        树的高度要尽量低
    3、 当b+树的数据项是复合的数据结构,b+数是按照从左到右的顺序来建立搜索树
        即索引的最左匹配特性。
-----------------------------------------------------
B+树的特点:
    单节点能够存储更多数据
    叶子节点形成有序链表,方便范围查找
    聚集索引中直接保存数据，非聚集索引中保存数据的地址
--------------------------------------------------------
索引
--------------------------------------------------------
分类:
    主键索引 PRIMARY KEY：
        一种特殊的唯一索引，不允许有空值。一般是在建表的时候同时创建主键索引
    唯一索引 UNIQUE：
        唯一索引列的值必须唯一，但允许有空值。如果是组合索引，则列值的组合必须唯一
    普通索引 INDEX：
        最基本的索引，它没有任何限制
    组合索引 INDEX：
        即一个索引包含多个列，多用于避免回表查询
    全文索引 FULLTEXT：
        即全文检索，是目前搜索引擎使用的一种关键技术
建立索引的原则:
    1、 最左前缀匹配原则,mysql会一直向右匹配直到遇到范围查询(>、<、between、like)就停止匹配
    2、 =和in可以乱序,mysql的查询优化器会帮你优化成索引可以识别的形式。
    3、 尽量选择区分度高的列作为索引，区分度的公式是count(distinct col)/count(*)，
        表示字段不重复的比例，比例越大我们扫描的记录数越少，
        唯一键的区分度是1，而一些状态、性别字段可能在大数据面前区分度就是0
        一般需要join的字段我们都要求是0.1以上，即平均1条扫描10条记录。
    4、 索引列不能参与计算，保持列“干净”,
        b+树中存的都是数据表中的字段值，但进行检索时，需要把所有元素都应用函数才能比较，
        显然成本太大。
    5、 尽量的扩展索引，不要新建索引。
        比如表中已经有a的索引，现在要加(a,b)的索引，那么只需要修改原来的索引即可。
--------------------------------------------------------
```
回表
```
定义:
    通过索引中记录的rowid访问表中的数据;
    对一个列创建索引之后，索引会包含该列的键值及键值对应行所在的rowid。
缺点:
    回表次数太多会严重影响SQL性能，如果回表次数太多，
    就不应该走索引扫描，应该直接走全表扫描。
    EXPLAIN命令结果中的Using Index意味着不会回表，通过索引就可以获得主要的数据。
    Using Where则意味着需要回表取数据。
```
索引优化规则：
```
1、前导模糊查询不能命中索引  
    EXPLAIN SELECT * FROM user WHERE name LIKE '%s%';
    非前导模糊查询则可以使用索引，可优化为使用非前导模糊查询：
    EXPLAIN SELECT * FROM user WHERE name LIKE 's%';
2、 复合索引的情况下，查询条件不包含索引列最左边部分，不会命中符合索引
    最左原则并不是说是查询条件的顺序 而是查询条件中是否包含索引最左列字段
3、 union、in、or都能够命中索引，建议使用in
4、 用or分割开的条件，如果or前的条件中列有索引，而后面的列中没有索引，
    那么涉及到的索引都不会被用到
5、 负向条件查询不能使用索引，可以优化为in查询。
    负向条件有：!=、<>、not in、not exists、not like等。
6、 范围条件查询可以命中索引。
    范围条件有：<、<=、>、>=、between等。
7、 数据库执行计算不会命中索引
8、 利用覆盖索引进行查询，避免回表
9、 建立索引的列，不允许为null。
10、更新十分频繁的字段上不宜建立索引
    因为更新操作会变更B+树，重建索引
11、区分度不大的字段上不宜建立索引
    类似于性别这种区分度不大的字段，建立索引的意义不大。
    因为不能有效过滤数据，性能和全表扫描相当。
12、业务上具有唯一特性的字段，即使是多个字段的组合，也必须建成唯一索引。
    虽然唯一索引会影响insert速度，但是对于查询的速度提升是非常明显的。
13、多表关联时，要保证关联字段上一定有索引。
--------------------------------------------------------------------
创建索引时避免以下错误观念：
    索引越多越好，认为一个查询就需要建一个索引；
    宁缺勿滥，认为索引会消耗空间、严重拖慢更新和新增速度；
    抵制唯一索引，认为业务的唯一性一律需要在应用层通过“先查后插”方式解决；
    过早优化，在不了解系统的情况下就开始优化。
```
InnoDB与MyISAM区别
```
1、 InnoDB支持事务，MyISAM不支持
    对于InnoDB每一条SQL语言都默认封装成事务
2、 InnoDB支持外键，而MyISAM不支持。
3、 InnoDB的B+树主键索引的叶子节点就是数据文件，
    辅助索引的叶子节点是主键的值；
    MyISAM的B+树主键索引和辅助索引的叶子节点都是数据文件的地址指针。
4、 InnoDB不保存表的具体行数，MyISAM保存了表的具体行数
6、 MyISAM表格可以被压缩后进行查询操作
7、 InnoDB支持表、行(默认)级锁，而MyISAM支持表级锁
    InnoDB的行锁是实现在索引上的，而不是锁在物理行记录上。
    如果访问没有命中索引，也无法使用行锁，将要退化为表锁。
8、 InnoDB表必须有主键（用户没有指定的话会自己找或生产一个主键），
    而Myisam可以没有
9、 Innodb存储文件有frm、ibd，而Myisam是frm、MYD、MYI
    Innodb：frm是表定义文件，ibd是数据文件
    Myisam：frm是表定义文件，myd是数据文件，myi是索引文件
```
事务的ACID
```
原子性（A）：
    是指事务要么都成功，要么都失败。
    成功就影响数据库，失败就对数据库不影响，保持原样。
一致性（C）：
    是指应用层系统从一种正确的状态，在事务成功后，达成另一种正确的状态。
隔离性（I）：
    隔离是指当多个事务提交时，让它们按顺序串行提交，每个时刻只有一个事务提交。
持久性（D）：
    是指事务一旦提交后，对数据库中的数据改变是永久性的。
```
```
脏读：
    A事务在读取数据时，B事务对同一个数据修改了，
    但B未提交，A再读取时，读到了B修改后的数据，
    但是B事务提交失败，回滚，A后读到的数据就是B修改后的脏数据。
不可重复读：
    A事务读取数据，B事务改了这个数据，也提交成功了，
    A再读取就是B修改后的数据，再也不能重复读到最开始的那个数据值
幻读：
    A事务读取数据，B事务改了这个数据（update），也提交成功了，
    A再读这个数据，SQL机制强行让A仍然读之前读到的数据值，
    这就是可重复读，这种机制对Insert操作无效，A事务在可重复读的机制下，
    读取数据，B事务insert一条数据，提交成功，
    A再读这个数据，会显示B插入的数据，此为幻读。
```
事务的四个隔离级别
```
Read uncommitted(未授权读取、读未提交)： 
    如果一个事务已经开始写数据，则另外一个事务则不允许同时进行写操作，
    但允许其他事务读此行数据。该隔离级别可以通过“排他写锁”实现。
    这样就避免了更新丢失，却可能出现脏读。
    也就是说事务B读取到了事务A未提交的数据。
Read committed（授权读取、读提交）： 
    读取数据的事务允许其他事务继续访问该行数据，
    但是未提交的写事务将会禁止其他事务访问该行。
    该隔离级别避免了脏读，但是却可能出现不可重复读。
    事务A事先读取了数据，事务B紧接了更新了数据，并提交了事务，
    而事务A再次读取该数据时，数据已经发生了改变。
Repeatable read（可重复读取）： 
    可重复读是指在一个事务内，多次读同一数据。
    在这个事务还没有结束时，另外一个事务也访问该同一数据。
    那么，在第一个事务中的两次读数据之间，即使第二个事务对数据进行修改，
    第一个事务两次读到的的数据是一样的。
    这样就发生了在一个事务内两次读到的数据是一样的，因此称为是可重复读。
    读取数据的事务将会禁止写事务（但允许读事务），写事务则禁止任何其他事务。
    这样避免了不可重复读取和脏读，但是有时可能出现幻象读。
    （读取数据的事务）这可以通过“共享读锁”和“排他写锁”实现。
Serializable（序列化）： 
    提供严格的事务隔离。它要求事务序列化执行，事务只能一个接着一个地执行，
    但不能并发执行。如果仅仅通过“行级锁”是无法实现事务序列化的，
    必须通过其他机制保证新插入的数据不会被刚执行查询操作的事务访问到。
    序列化是最高的事务隔离级别，同时代价也花费最高，性能很低，一般很少使用，
    在该级别下，事务顺序执行，不仅可以避免脏读、不可重复读，还避免了幻像读。 
```

B-与B+树区别
```
关键字数量不同：
    B+树分支结点M个关键字，叶子节点也有M个；
    B树分支结点则存在 k-1 个关键码
数据存储位置不同：
    B+树数据存储在叶子结点上；
    B树存储在每个结点上；
查询不同：
    B+树是从根节点到叶子节点的路径；
    B树是只需要找到数据就可以
分支节点存储信息不同：
    B+树存索引信息；
    B树存的是数据关键字
小结：
B树：
    二叉树，每个结点只存储一个关键字，等于则命中，
    小于走左结点，大于走右结点；
B-树：
    多路搜索树，每个结点存储M/2到M个关键字，
    非叶子结点存储指向关键字范围的子结点；
    所有关键字在整颗树中出现，且只出现一次，非叶子结点可以命中；
B+树：
    在B-树基础上，为叶子结点增加链表指针，
    所有关键字都在叶子结点中出现，非叶子结点作为叶子结点的索引；
    B+树总是到叶子结点才命中；
B*树： 
    在B+树基础上，为非叶子结点也增加链表指针，
    将结点的最低利用率从1/2提高到2
```

MySQL的联合索引
```
定义:
    对多个字段同时建立的索引
    (有顺序，ABC，ACB是完全不同的两种联合索引。)
好处:
    建立这样的索引相当于建立了索引a、ab、abc三个索引。
    节省写操作开销和磁盘空间开销
注意项:
    单个索引需要注意的事项，组合索引全部通用。
    最左匹配原则。(A,B,C) 这样3列，mysql会首先匹配A，然后再B，C.
    如果用(B,C)这样的数据来检索的话，就会找不到A使得索引失效。
    如果使用(A,C)这样的数据来检索的话，就会先找到所有A的值然后匹配C，
    此时联合索引是失效的。
```
为什么要分库分表
（设计高并发系统的时候，数据库层面应该如何设计）
```
原因:
    分库和分表都是为了防止数据库服务因为同一时间的访问量（增删查改）
    过大导致宕机而设计的一种应对策略。

单库单表存在的问题:
单库太大：
    数据库里面的表太多，所在服务器磁盘空间装不下，IO次数多CPU忙不过来。
单表太大:                                                            
    一张表的字段太多，数据太多。查询起来困难                            
--------------------------------------------------------------------
解决方案:
一、主从复制
    带来的问题:
        写操作拓展起来比较困难，因为要保证多个主库的数据一致性。
        复制延时：意思是同步带来的时间消耗。
        锁表率上升：读写分离，命中率少，锁表的概率提升。
        表变大，缓存率下降：此时缓存率一旦下降，带来的就是时间上的消耗。
    总结:
        主从复制架构随着用户量的增加、访问量的增加、
        数据量的增加依然会带来大量的问题
-----------------------------------------------------------------------
```

分库分表
```
分表:
垂直分表：
    表中的字段较多，一般将不常用的、 数据较大、长度较长的拆分到“扩展表“。
    一般情况加表的字段可能有几百列，此时是按照字段进行数竖直切。注意垂直分是列多的情况。
水平分表:
    单表的数据量太大。按照某种规则（RANGE,HASH取模等），切分到多张表里面去。 
    但是这些表还是在同一个库中，所以库级别的数据库操作还是有IO瓶颈。
    这种情况是不建议使用的，因为数据量是逐渐增加的，当数据量增加到一定的程度还需要再进行切分。比较麻烦。
分库:
垂直分库:
    一个数据库的表太多。此时就会按照一定业务逻辑进行垂直切，
    比如用户相关的表放在一个数据库里，订单相关的表放在一个数据库里。
    注意此时不同的数据库应该存放在不同的服务器上，此时磁盘空间、内存、TPS等等都会得到解决。
水平分库:
    水平分库理论上切分起来是比较麻烦的，它是指将单张表的数据切分到多个服务器上去，
    每个服务器具有相应的库与表，只是表中数据集合不同。 
    水平分库分表能够有效的缓解单机和单库的性能瓶颈和压力，突破IO、连接数、硬件资源等的瓶颈。
```
分库分表之后的问题
```
联合查询困难:
    联合查询不仅困难，而且可以说是不可能，
    因为两个相关联的表可能会分布在不同的数据库，不同的服务器中。
需要支持事务:
    分库分表后，就需要支持分布式事务了。数据库本身为我们提供了事务管理功能，
    但是分库分表之后就不适用了。如果我们自己编程协调事务，代码方面就又开始了麻烦。
跨库join困难:
    分库分表后表之间的关联操作将受到限制，我们无法join位于不同分库的表，
    也无法join分表粒度不同的表， 结果原本一次查询能够完成的业务，
    可能需要多次查询才能完成。 我们可以使用全局表，所有库都拷贝一份。
结果合并麻烦:
    比如我们购买了商品，订单表可能进行了拆分等等，此时结果合并就比较困难。 
```

sql语句执行的流程
```
1、 客户端连接到MySQL服务器时,服务器对其进行认证
2、 正式查询前回检测查询缓存，如果存在就返回结果
3、 缓存不存在,解析器回根据查询语句构造解析树,
    用于根据语法规则严重语句是否正确
4、 查询优化器将解析树转化为查询计划,优化器会执行
    自己认为的最优的执行计划
5、 执行计划调用查询执行引擎,查询引擎通过一系列API接口查询到数据
6、得到数据后，返回给客户端数据同时将数据存在查询缓存中
-------------------------------------------------
查询缓存器：
    默认关闭
```
聚簇索引
```
1、 使用记录主键值的大小来进行记录和页的排序
    ：页内的记录按照主键的大小顺序排成一个单向链表
      各个存放用户记录的页也是根据页中用户记录的主键大小顺序排查一个双向链表
2、 叶子节点存储的是完整的用户记录
note:
    聚簇索引是innoDB存储引擎自动创建的，没有主键也会默认创建一列
```
非聚簇索引
```
1、 叶子节点内部使用name字段排序,叶子节点之间也使用name字段排序
2、 叶子节点不再是完整的数据记录,而是name和主键值
```
非聚簇索引查询过程
```
1、 根据查询条件,采用name的非聚簇索引,先定位到该非聚簇索引某些记录行
2、 根据记录行找到相应的id,再根据id到聚簇索引中找到相关记录
```
为什么索引使用B+树
```
1、 B+树的磁盘读写代价更低
    B+树的内部节点并没有执行关键字具体信息的指针,因此器内部节点相对B树
    更小,如果把索引同一内部节点的关键字存放在同一盘块中，那么盘块所能容纳的
    关键字数量也越多,一次性读入内存的需要查找的关键字也就越多，相对IO读写次数
    就降低了
2、 由于B+树的数据都存储再叶子节点中,分组节点均为索引,方便扫库，只需要扫一遍叶子节点
    即可，但是B树因为其分组节点同样存储者数据,如果需要找数据的化，就需要进行一次中序
    遍历,所以B+树更适合在区间查询的情况
```
explain用法
```
id: 
    一般来说一个select一个唯一id,如果是子查询,就有两个select,id是不一样的
select_type:
    simple: 不包含union和子查询的查询都算simple类型
    primary:
        包括union union all ，其中最左边的查询即为primary
    union:
        包括union union all 除了最左边的查询，其他的查询类型为union
table:
    现在这一行是关于哪张表的
type: 访问方法
    ref:
        普通二级索引与常量进行等值匹配
    ref_or_null:
        普通二级索引与常量进行等值匹配，该索引可能为null
    const:
        主键活唯一二级索引列与常量进行等值匹配
    range:
        范围区间的查询
    all:
        全表扫描
key:
    经过查询优化器计算不同索引的成本,最终选择的成本最低的索引
rows:
    全表扫描时代表需要扫描的行数
    使用索引代表预计扫描的行数

```
MySQL redo日志和undo日志、二进制日志
```
innodb事务日志包括redo log和undo log。
redo log是重做日志，提供前滚操作，
undo log是回滚日志，提供回滚操作。
二进制日志:
    引起或可能引起数据库改变的事件
    只在事务提交的时候写入，提交之前写入cache,提交时写入
----------------------------------------
innodb通过force log at commit机制实现事务的持久性:
    即在事务提交的时候，必须先将该事务的所有事务日志写入到磁盘上
    的redo log file和undo log file中进行持久化
MySQL支持用户定义在commit时如何将log buffer中的日志刷log file
设置为1时:
    事务每次提交都会将log buffer中的日志写入os buffer并调用fsync
    刷到file on desk ; 数据不会丢失,IO性能较差
设置为0时:
    事务提交时不会将log buffer中的日志写入到os buffer，而是每秒
    写入os buffer并调用fsync()写入到file on disk
设置为2时:
    每次提交都仅写入到os buffer，然后时每秒调用fsync()将os buffer
    中的日志写入到file on disk
--------------------------------------------------------------

redo log:
    通常是物理日志，记录的是数据页的物理修改，而不是某一行或某几行修改成怎样怎样，
    它用来恢复提交后的物理数据页(恢复数据页，且只能恢复到最后一次提交的位置)。
undo:
    作用:
        提供回滚
        多个行版本控制(MVCC)
    undo log是逻辑日志。可以认为当delete一条记录时，undo log中会记录一条对应的insert记录，反之亦然，当update一条记录时，它记录一条对应相反的update记录。
    undo log也会产生redo log 因为undo log也要实现持久性保护

------------------------------------------------------------------------------
delete update 如何操作
delete 操作不好直接删除,而是将delete对象打上flag,标记为删除，
    最终的删除操作有purge线程处理
update分两种情况:
    不是主键列,在undo log中直接反向记录是如何update的
    是主键列,先删除该行,在插入一行目标行                                            
    
```

redo日志
```
重做日志:
    数据写到数据库之前，先记录修改日志，在操作到具体的行数据
redo日志何时刷新到磁盘?
    log buffer空间不足 mysql认为日志占总容量的一半即写入日志
    事务提交时,redo日志会刷新到磁盘
后台线程不同的刷新：
    后台线程定时刷新到磁盘
checkpoint:
    ???
```
undo日志
```
记录回滚语句
```