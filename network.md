##### TCP
TCP三次握手的流程

```
第一次:
    客户端发送SYN,seq=x,客户端状态为SYN_SEND
    对应socket调用connect函数
第二次:
    服务端发送SYN+ACK,seq=y,ACKnum=x+1,服务端状态为
    SYN_RCVD,对应服务端listen函数
第三次:
    客户端发送ACK,ACKnum=y+1客户端进入established状态,
    服务端收到这个包也进入establised状态
    对应socket的accecpt函数
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZG0Pq4BlibjdTLTmxf0o7Dic18o8iaDKONvjpp5ompia6JicOricF3puAobTFg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

TCP三次握手的原因
```
两次握手无法保证客户端服务端都收到对方的包,连接无法确定一定建立成功
四次握手无必要,因为服务端SYN和ACK可以合并发送
```
TCP四次挥手
```
第一次挥手(FIN)发送完毕,客户端进入FIN_WAIT_1状态
第二次挥手(ACK)发送完毕,服务端进入CLOSE_WAIT状态
第三次挥手(FIN+ACK)发送完毕,服务端进入LAST_ACK状态,登录客户端最后一个ACK
第四次挥手(ACK)发送完毕,客户端进入TIME_WAIT状态,等待2MSL之后,没有收到服务端的ACK
认为正常关闭
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZGVRl1TbwY0QiaUE1N2W4Oic8rbJr52cZTb6WWICr6p9ibuibw8DPLddTTPw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">
TIME_WAIT状态为什么需要等待2MSL
```
1个MSL保证四次挥手中主动关闭方最后的ACK报文能够最终达到对端
1个MSL保证对端没有收到ACK可以重传的FIN能够到达
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZGcmrVMle4KZubY5Tciae8HO8wnpzPUZthmTXY8PpmoYjZ4FXC9ibRz0ug/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

TCP和UDP的区别
```
1. TCP面向连接,UDP是无连接的
2. TCP可靠,UDP不可靠
3. TCP是点对点连接的,UDP一对一,一对多,多对多都可以
4. TCP传输效率相对较低,而UDP传输效率高.它适用于对高速传输和实时性有较高的通信或广播通信。
    TCP适合用于网页.邮件等;UDP适合用于视频,语音广播等
5. TCP面向字节流,UDP面向报文
```
TCP首部
```
16位端口号:
    源端口号:主机该报文段是来自哪里
    目标端口号:要传给哪个上层协议或应用程序
32位序号: 一次TCP通信过程中某一个传输方向上的字节流的每个字节的编号。
32位确认号: 用作对另一方发送的tcp报文段的响应,其值是收到的TCP报文段的序号值加1。
4位头部长度:表示tcp头部有多少个32bit字,因为4位最大能标识15，所以TCP头部最长是60字节。
6位标志位: 
    URG(紧急指针是否有效)
    ACk（表示确认号是否有效
    PSH（缓冲区尚未填满）
    RST（表示要求对方重新建立连接）
    SYN（建立连接消息标志接）
    FIN（表示告知对方本端要关闭连接了）
16位窗口大小:
    是TCP流量控制的一个手段。
16位校验和:
    由发送端填充,接收端对TCP报文段执行CRC算法以检验TCP报文段在传输过程中是否损坏
16位紧急指针:一个正的偏移量
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZGwoic656sZhv2TiaWBoaZtibN0ZzXdY7oJn0RUzjhKpa8KavmicSzQCYVOA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

TCP如何保证可靠性
```
首先: TCP的连接是基于三次握手,而断开则是四次挥手;确保连接和断开的可靠性。
其次: 有状态,记录TCP发送数据,TCP通过序列号保证数据包按序到达,保证数据传输不出差错。
再次: 可控制,报文校验ACK应答,超时重传,失序数据重传,丢弃重复数据,流量控制和拥塞控制等机制
```
TCP超时重传
```
定义:
    在发送数据报文时,设定一个定时器,每间隔一段时间,没有收到对方的ACK确认应答报文
    就会重发该报文
RTT: 数据包的一次往返时间

RTO: 超时重传时间

缺点:
    当一个报文段丢失时,会等待一定的超时周期然后才重传分组,增加了端到端的时延
    当一个报文段丢失时,在其等待超时的过程中.可能会出现这种情况
    其后的报文段已经被接收端接收但却迟迟得不到确认.发送端会认为也丢失了.
    从而引起不必要的重传既浪费资源也浪费时间。
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZGQeePs2NCSbIdvl997a7mWGHUWic5kGghXVFpRNPwYtOggZytGywNMaw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

快速重传
```
第一份 Seq=1 先送到了，于是就 Ack 回 2；
第二份 Seq=2 也送到了，假设也正常，于是ACK 回 3；
第三份 Seq=3 由于网络等其他原因，没送到；
第四份 Seq=4 也送到了，但是因为Seq3没收到。所以ACK回3；
后面的 Seq=4,5的也送到了，但是ACK还是回复3，因为Seq=3没收到。
发送端连着收到三个重复冗余ACK=3的确认（实际上是4个，但是前面一个是正常的ACK，后面三个才是重复冗余的），便知道哪个报文段在传输过程中丢失了，于是在定时器过期之前，重传该报文段。
最后，接收到收到了 Seq3，此时因为 Seq=4，5，6都收到了，于是ACK回7.
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZGu7I5I3EJZTdlJMGMxibIkYquScTY5XibRicykIrEHAp7qBag50qH7I4UA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

带选择确认的重传（SACK）
```
SACK机制就是.在快速重传的基础上，接收端返回最近收到的报文段的序列号范围，这样发送端就知道接收端哪些数据包没收到，酱紫就很清楚该重传哪些数据包啦。SACK标记是加在TCP头部选项字段里面的。
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZGtnwicDafN34ibW12dBjvVgUdnEnDibWKkLBiafdjHnr0UNzZKIIMDoVsLA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

滑动窗口
```
就是接受方每次收到数据包,在发送确认报文的时候.同时告诉发送方,自己的缓存区还有多少空余空间
缓冲区的空余空间,我们就称之为接受窗口大小。这就是win
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZG7FI6BmVNTicvXlsjU0DXQicF0LRXu1LhQfQmhiaDiaZkzYOGBACZrOmajQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZG7FI6BmVNTicvXlsjU0DXQicF0LRXu1LhQfQmhiaDiaZkzYOGBACZrOmajQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

TCP的流量控制
```
TCP 提供一种机制可以让发送端根据接收端的实际接收能力控制发送的数据量，这就是流量控制。
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZGQib57JdYribwbrX2jQofvNhqfIb5vKNxnWvhvvPWOVrhJkF21brtQoRg/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

SYN Flood(SYN 泛洪)
```
半连接队列:
    客户端发送SYN到服务端,服务端返回SYN+ACK,状态转换为SYN_RCVD
    这种状态的连接保存起来就是半连接队列
全连接队列:
    客户端回复ACK,三次握手完成,再被具体应用取走之前会被推入ACCEPT队列
    即全连接队列
SYN Flood定义:
    短时间内伪造不存在的IP向服务器发送大量SYN报文,导致服务器上建立大量半连接
    半连接队列满了,就无法处理正常的TCP请求
解决方法:
    syn cookie: 收到syn后,服务器根据数据包的地址和端口计算出cookie值作为synack
                的序列号,回复syn+ack后服务器并不立即分配资源进行处理,等收到ack后
                重新根据数据包地址和端口计算序列号,正确的时候才建立连接
    SYN Proxy防火墙: 服务器防火墙会对每一个SYN报文进行代理和回应,并保持半连接
                等到ACK返回后再重新狗仔SYN包发送到服务器,建立真正的连接
```

TCP粘包和拆包
```
问题: 一个完整的包可能会被TCP拆分成多个包进行发送,
     也有可能把多个小的包封装成一个大的数据包发送
产生的原因:
    1. 要发送的数据小于TCP发送缓冲区的大小,TCP将多次写入缓冲区的数据一次发送出去
        将会发生粘包
    2. 接收数据端的应用层没有及时读取接收缓冲区中的数据,将发生粘包
    3. 要发送的数据大于TCP发送缓冲区剩余空间大小,将会发生拆包
    4. 待发送数据大于MSS(最大报文长度),TCP在传输前将进行拆包;
        即TCP报文长度-TCP头部长度>MSS。
解决方案：
    1.发送端将每个数据包封装为固定长度
    2.在数据尾部增加特殊字符进行分割
    3.将数据分为两部分,一部分是头部,一部分是内容体;
        其中头部结构大小固定,且有一个字段声明内容体的大小
```
<img src="https://mmbiz.qpic.cn/mmbiz_png/PoF8jo1PmpwwTDZWIWFC9LakSgxrYMZG54k9yjr7f5bwo57gkkh89MnVanhgiaeibYsSiaKSY7wHMfXhsFXzzxGGw/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

TCP有限状态机

<img src="https://mmbiz.qpic.cn/mmbiz_jpg/e13svNCGUiaqDibs2JI0ibbsCTFIsXQY8OBrbc9SoUMzWEF2RvOLVwCib8MjoM4ROSb0evV9JgzKoe5jMiaWjUz8Pxg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

TCP异常分析
```
1.Errno 111(Connection refused):
    试图与一个不存在的端口建立连接(主机正常)
2.试图与一个某端口建立连接但该主机已经宕机(主机宕机):
    分2种情况,一种刚刚宕机,一种宕机了很长时间
    宕机很长时间后,连接失败,网络不通
    刚刚宕机,ARP协议有缓存,可以发送数据,服务器没回复
    导致客户端一直发送。
3.刚刚建立时,服务器应用被阻塞


```

```
粘包
connect reset by peer，这里顺便说下 Broken pipe，
```
进程间通信
```
管道(pipe):
    管道是一种半双工的通信方式,数据只能单向流动,而且只能在具有亲缘关系的进程间使用
    进程的亲缘关系通常是指父子进程关系。
消息队列通信:
    消息队列本质上是一种解耦的方式,消息队列克服了信号传递信息少以及缓冲区大小受限等缺点。
信号量:
    信号量是一个计数器,可以用来控制多个进程对共享资源的访问
信号:
    信号是一种比较复杂的通信方式，用于通知某个进程事件已经发生。
共享内存通信:
    共享内存就是映射一段能被其他进程所访问的内存,这段共享内存由一个进程创建
    但多个进程都可以访问,共享内存是最快的 IPC 方式
socket:
    套接口也是一种进程间通信机制,与其他通信机制不同的是,它可用于不同机器间的进程通信
```


. 多路复用
```
select缺点:
    1. 单个进程所打开的FD是有限制的,通过FD_SETSIZE设置,默认1024
    2. 每次调用select,都需要把fd集合从用户态拷贝到内核态,这个开销在fd很多时会很大
    3. 对socket扫描时是线性扫描,采用轮询的方法,效率较低（高并发时）
poll缺点:
    1. 每次调用poll,都需要把fd集合从用户态拷贝到内核态,这个开销在fd很多时会很大
    2. 对socket扫描时是线性扫描,采用轮询的方法,效率较低（高并发时）
epoll LT 与 ET模式的区别:
epoll有EPOLLLT和EPOLLET两种触发模式，LT是默认的模式，ET是“高速”模式。
LT模式下，只要这个fd还有数据可读，每次 epoll_wait都会返回它的事件，提醒用户程序去操作
ET模式下，它只会提示一次，直到下次再有数据流入之前都不会再提示了，无论fd中是否还有数据可读。所以在ET模式下，read一个fd的时候一定要把它的buffer读完，或者遇到EAGAIN错误
```

##### HTTP

浏览器输入url的过程
```
1.DNS域名解析；
2.建立TCP连接；
3.发送HTTP请求；
4.服务器处理请求；
5.返回响应结果；
6.关闭TCP连接；
7.浏览器解析HTML；
8.浏览器布局渲染
```
##### UDP

udp如何实现可靠性
```
1. 添加seq/ack机制,确保数据发送到对端
2. 添加发送和接收缓冲区,主要是用户超时重传
3. 添加超时重传机制
```


