#### String

```
rune: Unicode编码,对应uinit32
byte: ASCII编码,对应uinit8
一个英文字符对应一个byte,一个中文字符对应三个byte
一个rune对应一个UTF-8字符,所以一个中文字符对应一个rune
len:
    计算的是byte长度
range:
    是按照rune处理的
```

#### Slice
```
slice结构:
    |data|len|cap|
----------------------------------------------------
slice 扩容规则:
STEP1: 预估容量
        如果 oldCap *2 < cap ,newCap = cap
        如果 oldCap *2 > cap:
            如果 oldLen<1024 ,newCap = oldCap*2
            如果 oldLen>1024, newCap = oldCap*1.25 (循环处理)
STEP2: 计算内存
       分配内存 = 预估容量个数*类型长度
STEP3: 匹配内存
        获取足够大且最小的内存块
总结: 最终的数量与预估容量并不一定是一致的
-----------------------------------------------
slice分配内存:
    使用make初始化长度
    使用append扩容
make用法:
    mkae([]T,len,cap)
make 与 new的区别:
    1 make仅适用于slice,map,channel,new适用于全部类型
    2 make返回T,new返回*T
    3 make分配内存,new不分配内存
```
#### 内存对齐
```
用空间换时间,希望读取一次能够取出数据
类型对齐边界:
    所有成员中的最大值
结构体对齐:
    每个成员起始地址必须能被长度整除
    结构体的总长度必须类型对齐的倍数
```
#### map
```
底层使用hash表,是无序的
解决冲突办法:
    1 开放寻址法,多次寻址
    2 开链法
扩容规则:
    负载因子>6.5: 
        进行翻倍扩容,存储时新旧桶都存储数据,同时每次操作时也迁移部分数据
        迁移结束时,销毁旧桶
    负载因子没超标,但溢出桶太多: 等量扩容
    桶的数量>2^4时使用溢出桶
    溢出桶太多判断: 常规桶数量 <= 2^15 , 溢出桶>=2^桶数量
                   常规桶数量 > 2^15, 溢出桶数量> 2^15
    等量扩容的意义:
        溢出桶太多基本由于删除太多,等量扩容相当于重新hash,能够减少
        溢出桶的使用,使数据存储的更加紧凑

```
#### 函数调用栈
```
参数都是值传递
```

#### 闭包
```
闭包对象运行时创建,包含捕获变量和函数入口两部分;

捕获列表分三种情况:
2 修改捕获变量,拷贝到堆上,使用的时变量的地址,要看
    实际执行时地址对应的值
3 捕获列表时形参, 栈上直接拷贝到堆上
4 返回值, 闭包放在堆上,然后拷贝回栈上
```
#### 方法
```
值接收者和指针接收者的区别:
    值接收者传的是值的副本,如果修改的话,与之前的值没关系,只是修改了副本
    指针接收者传的是指针的副本,如果修改的话,是能够找到原始的值的
```
#### defer
```
defer传参传的时拷贝值,闭包的捕获变量时获取的值的地址
defer:
    编译阶段直接处理插入代码
    循环里面例外
```
#### panic和recover
```
panic终止程序执行,recover终止panic执行
什么时栈扫描
```
































