1. 使⽤两种命令创建⼀个⽂件？
```
a. touch a.txt
b. vi a.txt
c. mkdir abc
d. cat > a.txt 建⽴⼀⽂件，然后把接下来的键盘输⼊写⼊⽂件，直到按Ctrl+D为⽌.
```
2. 硬链接和软连接的区别？
```
a. 硬链接：

1、⽂件有相同的 inode 及 data block；

2、只能对已存在的⽂件进⾏创建；

3、不能交叉⽂件系统进⾏硬链接的创建；

4、不能对⽬录进⾏创建，只可对⽂件创建；

5、删除⼀个硬链接⽂件并不影响其他有相同 inode 号的⽂件。

b. 软链接：

1、软链接有⾃⼰的⽂件属性及权限等；

2、可对不存在的⽂件或⽬录创建软链接；

3、软链接可交叉⽂件系统；

4、软链接可对⽂件或⽬录创建；

5、创建软链接时，链接计数 i_nlink 不会增加；

6、删除软链接并不影响被指向的⽂件，但若被指向的原⽂件被删除，则相关软连接被称为死链接（即 dangling link，若被指向路径⽂件被重新创建，死链接可恢复为正常的软链接）。
```
3. linux常⽤命令有哪些？
```
查找关闭端⼝进程 netstat -nlp | grep :3306 kill pid

删除⽂件 rm -rf

查找⽇志 cat xx.log | grep 'xxx' | more

解压tar.gz tar -xzvf file.tar.gz

创建⽂件 touch filename cat > filename

修改⽂件 vi
```
4. 怎么查看⼀个java线程的资源耗⽤？

linux下，所有的java内部线程，其实都对应了⼀个进程id，也就是说，linux上的jvm将java程序中的线程映射为操作系统进 程。

1、jps -lvm或者ps -ef | grep java查看当前机器上运⾏的Java应⽤进程

2、top -Hp pid可以查看Java所有线程的资源耗⽤

3、printf "%x\n"pid等到线程ID的16进制

4、jstack Java应⽤进程ID | grep 线程ID的16进制

5. Load过⾼的可能性有哪些？

cpu load的飙升，⼀⽅⾯可能和full gc的次数增⼤有关，⼀⽅⾯可能和死循环有关系

6. /etc/hosts⽂件什么作⽤？

在当前主机给ip设置别名，通过该别名可以访问到该ip地址，通过别名、ip访问的效果是⼀样的

7. 如何快速的将⼀个⽂本中的"abc"转换成"xyz"？

vi filename编辑⽂本，按Esc键，输⼊:%s/abc/xyz/g

8. 如何在log⽂件中搜索找出error的⽇志？

cat xx.log | grep 'error'

9. 发现硬盘空间不够，如何快速找出占⽤空间最⼤的⽂件?

find . -type f -size +100M | xargs du -h | sort -nr

10. Java服务端问题排查（OOM，CPU⾼，Load⾼，类冲突）？

a. 业务⽇志相关：

i. less或者moreii. grep

iii. tail -f filename

ps:切忌vim直接打开⼤⽇志⽂件，因为会直接加载到内存的

b. 数据库相关：

i. 登录线上库，show processlist查看数据库连接情况

c. jvm相关：

i. jps显示java进程

ii. jinfo实时查看和调整jvm参数

iii. jstat监控jvm各种运⾏状态信息；

iv. jstack(Stack Trace for Java)命令⽤于⽣成JVM进程当前时刻的线程的调⽤堆栈，可以⽤来定位线程间死锁、

锁等待、等待外部资源等

v. jmap(Memory Map for Java) 命令⽤于⽣成堆转储快照dump⽂件，除了这种⽅式还可以通过- XX:HeapDumpOnOutOfMemoryError参数，可以在虚拟机发⽣OOM的时候⾃动⽣成堆的dump⽂件，或者kill -3 命令发出进程退出信号"吓唬"⼀下虚拟机，也能拿到dump⽂件。

d. oom问题：

i. 配置了-XX:+HeapDumpOnOutOfMemoryError, 在发⽣OOM的时候会在-XX:HeapDumpPath⽣成堆的dump⽂件，结合MAT，可以对dump⽂件进⾏分析，查找出发⽣OOM的原因。

ii. 另外⼿动dump堆快照，可以使⽤命令jmap -dump:format=b,file=file_name pid 或者kill -3 pid

e. 死锁：

i. jps -v

ii. jstack -l pid

f. 线程block、线程数暴涨：

i. jstack -l pid |wc -l

ii. jstack -l pid |grep "BLOCKED"|wc -l

iii. jstack -l pid |grep "Waiting on condition"|wc -l

线程block问题⼀般是等待io、等待⽹络、等待监视器锁等造成，可能会导致请求超时、造成造成线程数暴涨导致系统502 等。

g. 服务器问题：

i. cpu：top

ii. 内存：

1. free -m -c10 -s1：

a. -m：以MB为单位显示，其他的有-k -g -b

b. -s: 间隔多少秒持续观察内存使⽤状况

c. -c:观察多少次

2. vmstat 1 10：1表示每隔1s输出⼀次,10 表示输出10次

a. r: 运⾏队列中进程数量，这个值也可以判断是否需要增加CPU。（⻓期⼤于1）

b. b: 等待IO的进程数量。

h. io：

i. iostat -m 1 10：

1. -m：某些使⽤block为单位的列强制使⽤MB为单位

2. 1 10：数据显示每隔1秒刷新⼀次，共显示10次

i. ⽹络：

i. netstat -antp：

1. -a (all)显示所有选项，默认不显示LISTEN相关

2. -t (tcp)仅显示tcp相关选项

3. -u (udp)仅显示udp相关选项

4. -n 拒绝显示别名，能显示数字的全部转化成数字。

5. -l 仅列出有在 Listen (监听) 的服服务状态

6. -p 显示建⽴相关链接的程序名

11. Java常⽤问题排查⼯具及⽤法（top,iostat,vmstat,sar,tcpdump,jvisualvm,jmap,jconsole）

12. Thread dump⽂件如何分析（Runnable，锁，代码栈，操作系统线程id关联）

a. Thread Dump 能诊断的问题

i. 查找内存泄露，常⻅的是程序⾥load⼤量的数据到缓存；

ii. 发现死锁线程；

b. 如何抓取Thread Dump信息：

i. ⼀般当服务器挂起,崩溃或者性能底下时,就需要抓取服务器的线程堆栈(Thread Dump)⽤于后续的分析。 在实际 运⾏中，往往⼀次 dump的信息，还不⾜以确认问题。为了反映线程状态的动态变化，需要接连多次做 threaddump，每次间隔10-20s，建议⾄少产⽣三次 dump信息，如果每次 dump都指向同⼀个问题，我们才确 定问题的典型性。

ii. linux命令获取：

ps –ef | grep java

kill -

<pid>

iii. jdk⾃带⼯具获取：

jps 或 ps –ef|grepjava (获取PID)

jstack [-l ]<pid> | tee -a jstack.log (获取ThreadDump)

13. 如何查看Java应⽤的线程信息？

通过top命令拿到线程的pid后使⽤jstack命令

14. 计数？

1 wc-1

1

如何看当前Linux系统有几颗物理CPU和每颗CPU的核数？

答：

[root@centos6 ~ 10:55 #35]# cat /proc/cpuinfo|grep -c 'physical id'
4
[root@centos6 ~ 10:56 #36]# cat /proc/cpuinfo|grep -c 'processor'
4
2

查看系统负载有两个常用的命令，是哪两个？这三个数值表示什么含义呢？

答：

[root@centos6 ~ 10:56 #37]# w
10:57:38 up 14 min,  1 user,  load average: 0.00, 0.00, 0.00
USER     TTY      FROM              LOGIN@   IDLE   JCPU   PCPU WHAT
root     pts/0    192.168.147.1    18:44    0.00s  0.10s  0.00s w
[root@centos6 ~ 10:57 #38]# uptime
10:57:47 up 14 min,  1 user,  load average: 0.00, 0.00, 0.00
其中load average即系统负载，三个数值分别表示一分钟、五分钟、十五分钟内系统的平均负载，即平均任务数。

3

vmstat r, b, si, so, bi, bo 这几列表示什么含义呢？

答：

[root@centos6 ~ 10:57 #39]# vmstat
procs -----------memory---------- ---swap-- -----io---- --system-- -----cpu-----
r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
0  0      0 1783964  13172 106056    0    0    29     7   15   11  0  0 99  0  0
r即running，表示正在跑的任务数
b即blocked，表示被阻塞的任务数
si表示有多少数据从交换分区读入内存
so表示有多少数据从内存写入交换分区
bi表示有多少数据从磁盘读入内存
bo表示有多少数据从内存写入磁盘
简记：

i --input，进入内存
o --output，从内存出去
s --swap，交换分区
b --block，块设备，磁盘
单位都是KB

4

linux系统里，您知道buffer和cache如何区分吗？

答：

buffer和cache都是内存中的一块区域，当CPU需要写数据到磁盘时，由于磁盘速度比较慢，所以CPU先把数据存进buffer，然后CPU去执行其他任务，buffer中的数据会定期写入磁盘；当CPU需要从磁盘读入数据时，由于磁盘速度比较慢，可以把即将用到的数据提前存入cache，CPU直接从Cache中拿数据要快的多。

5

使用top查看系统资源占用情况时，哪一列表示内存占用呢？

答：

PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
301 root      20   0     0    0    0 S  0.3  0.0   0:00.08 jbd2/sda3-8
1 root      20   0  2900 1428 1216 S  0.0  0.1   0:01.28 init
2 root      20   0     0    0    0 S  0.0  0.0   0:00.00 kthreadd
3 root      RT   0     0    0    0 S  0.0  0.0   0:00.00 migration/0
VIRT虚拟内存用量
RES物理内存用量
SHR共享内存用量
%MEM内存用量
6

如何实时查看网卡流量为多少？如何查看历史网卡流量？

答：

安装sysstat包，使用sar命令查看。

yum install -y sysstat#安装sysstat包，获得sar命令
sar -n DEV#查看网卡流量，默认10分钟更新一次
sar -n DEV 1 10#一秒显示一次，一共显示10次
sar -n DEV -f /var/log/sa/sa22#查看指定日期的流量日志
7

如何查看当前系统都有哪些进程？

答：

ps -aux 或者ps -elf

[root@centos6 ~ 13:20 #56]# ps -aux
Warning: bad syntax, perhaps a bogus '-'? See /usr/share/doc/procps-3.2.8/FAQ
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         1  0.0  0.0   2900  1428 ?        Ss   10:43   0:01 /sbin/init
root         2  0.0  0.0      0     0 ?        S    10:43   0:00 [kthreadd]
root         3  0.0  0.0      0     0 ?        S    10:43   0:00 [migration/0]
root         4  0.0  0.0      0     0 ?        S    10:43   0:00 [ksoftirqd/0]
……
[root@centos6 ~ 13:21 #57]# ps -elf
F S UID        PID  PPID  C PRI  NI ADDR SZ WCHAN  STIME TTY          TIME CMD
4 S root         1     0  0  80   0 -   725 -      10:43 ?        00:00:01 /sbin/init
1 S root         2     0  0  80   0 -     0 -      10:43 ?        00:00:00 [kthreadd]
1 S root         3     2  0 -40   - -     0 -      10:43 ?        00:00:00 [migration/0]
1 S root         4     2  0  80   0 -     0 -      10:43 ?        00:00:00 [ksoftirqd/0]
1 S root         5     2  0 -40   - -     0 -      10:43 ?        00:00:00 [migration/0]
8

ps 查看系统进程时，有一列为STAT， 如果当前进程的stat为Ss 表示什么含义？如果为Z表示什么含义？

答：S表示正在休眠；s表示主进程；Z表示僵尸进程。

9

如何查看系统都开启了哪些端口？

答：

[root@centos6 ~ 13:20 #55]# netstat -lnp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address               Foreign Address             State       PID/Program name
tcp        0      0 0.0.0.0:22                  0.0.0.0:*                   LISTEN      1035/sshd
tcp        0      0 :::22                       :::*                        LISTEN      1035/sshd
udp        0      0 0.0.0.0:68                  0.0.0.0:*                               931/dhclient
Active UNIX domain sockets (only servers)
Proto RefCnt Flags       Type       State         I-Node PID/Program name    Path
unix  2      [ ACC ]     STREAM     LISTENING     6825   1/init              @/com/ubuntu/upstart
unix  2      [ ACC ]     STREAM     LISTENING     8429   1003/dbus-daemon    /var/run/dbus/system_bus_socket
10

如何查看网络连接状况？

答：

[root@centos6 ~ 13:22 #58]# netstat -an
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address               Foreign Address             State
tcp        0      0 0.0.0.0:22                  0.0.0.0:*                   LISTEN
tcp        0      0 192.168.147.130:22          192.168.147.1:23893         ESTABLISHED
tcp        0      0 :::22                       :::*                        LISTEN
udp        0      0 0.0.0.0:68                  0.0.0.0:*
……
11

想修改ip，需要编辑哪个配置文件，修改完配置文件后，如何重启网卡，使配置生效？

答：使用vi或者vim编辑器编辑网卡配置文件/etc/sysconfig/network-scripts/ifcft-eth0（如果是eth1文件名为ifcft-eth1），内容如下：

DEVICE=eth0
HWADDR=00:0C:29:06:37:BA
TYPE=Ethernet
UUID=0eea1820-1fe8-4a80-a6f0-39b3d314f8da
ONBOOT=yes
NM_CONTROLLED=yes
BOOTPROTO=static
IPADDR=192.168.147.130
NETMASK=255.255.255.0
GATEWAY=192.168.147.2
DNS1=192.168.147.2
DNS2=8.8.8.8
修改网卡后，可以使用命令重启网卡：

ifdown eth0
ifup eth0
也可以重启网络服务：

service network restart
12

能否给一个网卡配置多个IP? 如果能，怎么配置？

答：可以给一个网卡配置多个IP，配置步骤如下：

cat /etc/sysconfig/network-scripts/ifcfg-eth0#查看eth0的配置
DEVICE=eth0
HWADDR=00:0C:29:06:37:BA
TYPE=Ethernet
UUID=0eea1820-1fe8-4a80-a6f0-39b3d314f8da
ONBOOT=yes
NM_CONTROLLED=yes
BOOTPROTO=static
IPADDR=192.168.147.130
NETMASK=255.255.255.0
GATEWAY=192.168.147.2
DNS1=192.168.147.2
DNS2=8.8.8.8
（1）新建一个ifcfg-eth0:1文件

cp /etc/sysconfig/network-scripts/ifcfg-eth0 /etc/sysconfig/network-scripts/ifcfg-eth0:1
（2）修改其内容如下：

vim /etc/sysconfig/network-scripts/ifcfg-eth0:1
DEVICE=eth0:1
HWADDR=00:0C:29:06:37:BA
TYPE=Ethernet
UUID=0eea1820-1fe8-4a80-a6f0-39b3d314f8da
ONBOOT=yes
NM_CONTROLLED=yes
BOOTPROTO=static
IPADDR=192.168.147.133
NETMASK=255.255.255.0
GATEWAY=192.168.147.2
DNS1=192.168.147.2
DNS2=8.8.8.8
（3）重启网络服务：

service network restart
13

如何查看某个网卡是否连接着交换机？

答：mii-tool eth0 或者 mii-tool eth1

14

如何查看当前主机的主机名，如何修改主机名？要想重启后依旧生效，需要修改哪个配 置文件呢？

答：查看主机名：

hostname
centos6.5
修改主机名：

hostname centos6.5-1
永久生效需要修改配置文件：

vim /etc/sysconfig/network
NETWORKING=yes
HOSTNAME=centos6.5-1
15

设置DNS需要修改哪个配置文件？

答：（1）在文件 /etc/resolv.conf 中设置DNS

（2）在文件 /etc/sysconfig/network-scripts/ifcfg-eth0 中设置DNS

16

使用iptables 写一条规则：把来源IP为192.168.1.101访问本机80端口的包直接拒绝

答：

iptables -I INPUT -s 192.168.1.101 -p tcp --dport 80 -j REJECT
17

要想把iptable的规则保存到一个文件中如何做？如何恢复？

答：使用iptables-save重定向到文件中：

iptables-save > 1.ipt
使用iptables-restore反重定向回来：

iptables-restore < 1.ipt
18

如何备份某个用户的任务计划？

答：将/var/spool/cron/目录下指定用户的任务计划拷贝到备份目录cron_bak/下即可

cp /var/spool/cron/rachy /tmp/bak/cron_bak/
19

任务计划格式中，前面5个数字分表表示什么含义？

答：依次表示：分、时、日、月、周

20

如何可以把系统中不用的服务关掉？

答：（1）使用可视化工具：ntsysv

（2）使用命令：

chkconfig servicename off
21

如何让某个服务（假如服务名为 nginx）只在3,5两个运行级别开启，其他级别关闭？

答：先关闭所有运行级别：

chkconfig nginx off
然后打开35运行级别：

chkconfig --level 35 nginx on
22

rsync 同步命令中，下面两种方式有什么不同呢？

(1) rsync -av  /dira/  ip:/dirb/
(2) rsync -av  /dira/  ip::dirb
答：(1)前者是通过ssh方式同步的

(2)后者是通过rsync服务的方式同步的

23

rsync 同步时，如果要同步的源中有软连接，如何把软连接的目标文件或者目录同步？

答：同步源文件需要加-L选项

24

某个账号登陆linux后，系统会在哪些日志文件中记录相关信息？

答：用户身份验证过程记录在/var/log/secure中，登录成功的信息记录在/var/log/wtmp。

25

网卡或者硬盘有问题时，我们可以通过使用哪个命令查看相关信息？

答：使用命令dmesg

26

分别使用xargs和exec实现这样的需求，把当前目录下所有后缀名为.txt的文件的权限修改为777

答：

（1）find ./ -type f -name "*.txt" |xargs chmod 777
（2）find ./ -type f -name "*.txt" -exec chmod 777 {} ;
27

有一个脚本运行时间可能超过2天，如何做才能使其不间断的运行，而且还可以随时观察脚本运行时的输出信息？

答：使用screen工具

28

在Linux系统下如何按照下面要求抓包：只过滤出访问http服务的，目标ip为192.168.0.111，一共抓1000个包，并且保存到1.cap文件中？

答：

tcpdump -nn -s0 host 192.168.0.111 and port 80 -c 1000 -w 1.cap
29

rsync 同步数据时，如何过滤出所有.txt的文件不同步？

答：加上--exclude选项：

--exclude=“*.txt”
30

rsync同步数据时，如果目标文件比源文件还新，则忽略该文件，如何做？

答：保留更新使用-u或者--update选项

31

想在Linux命令行下访问某个网站，并且该网站域名还没有解析，如何做？

答：在/etc/hosts文件中增加一条从该网站域名到其IP的解析记录即可，或者使用curl -x

32

自定义解析域名的时候，我们可以编辑哪个文件？是否可以一个ip对应多个域名？是否一个域名对应多个ip？

答：编辑 /etc/hosts ,可以一个ip对应多个域名，不可以一个域名对多个ip

33

我们可以使用哪个命令查看系统的历史负载（比如说两天前的）？

答：

sar -q -f /var/log/sa/sa22  #查看22号的系统负载
34

在Linux下如何指定dns服务器，来解析某个域名？

答：使用dig命令：dig @DNSip http://domain.com

如：

dig @8.8.8.8 www.baidu.com#使用谷歌DNS解析百度
35

使用rsync同步数据时，假如我们采用的是ssh方式，并且目标机器的sshd端口并不是默认的22端口，那我们如何做？

答：

rsync "--rsh=ssh -p 10022"或者rsync -e "ssh -p 10022"
36

rsync同步时，如何删除目标数据多出来的数据，即源上不存在，但目标却存在的文件或者目录？

答：加上--delete选项

37

使用free查看内存使用情况时，哪个数值表示真正可用的内存量？

答：free列第二行的值

38

有一天你突然发现公司网站访问速度变的很慢很慢，你该怎么办呢？

（服务器可以登陆，提示：你可以从系统负载和网卡流量入手）

答：可以从两个方面入手分析：分析系统负载，使用w命令或者uptime命令查看系统负载，如果负载很高，则使用top命令查看CPU，MEM等占用情况，要么是CPU繁忙，要么是内存不够，如果这二者都正常，再去使用sar命令分析网卡流量，分析是不是遭到了攻击。一旦分析出问题的原因，采取对应的措施解决，如决定要不要杀死一些进程，或者禁止一些访问等。

39

rsync使用服务模式时，如果我们指定了一个密码文件，那么这个密码文件的权限应该设置成多少才可以？

答：600或400

