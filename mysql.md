
##### 基础
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

##### 索引
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

#### 事务

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
mvcc
```
```
