##### TCP

. 报文格式

. 状态转换

<img src="https://mmbiz.qpic.cn/mmbiz_png/j3gficicyOvasF6pzMRVlOnbpMNic3XqHBPtBWP3YibK8x5FYPxNxZO72LkOkBvguP62XVicz1z4naf6LgiaFJSw65ibA/640?wx_fmt=png&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

. 报文处理

```
粘包
connect reset by peer，这里顺便说下 Broken pipe，
```

. 协议特点

. 如何保证可靠性

. 三次握手

```
目的:
    分配资源,初始化序列号
原因:
    服务端(被动端)发送ACK和SYN包合并在一个包中发送
    并非一定是三次握手,对于支持同时连接的设备有可能是四次握手
```
<img src="https://mmbiz.qpic.cn/mmbiz_jpg/j3gficicyOvasF6pzMRVlOnbpMNic3XqHBPBQInmfEBgvdVd8WDPLodCiaCIHy1ib2mSLCMUGrf3pTric60MJeIzHlbg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&wx_co=1" style="zoom:80%;transform:rotate(0deg)">

. 四次挥手
```
超时设置是 2*MSL，RFC793 定义了 MSL 为 2 分钟，Linux 设置成了 30s
```
三次握手四次挥手对应的TCP库函数

. 安全性问题
```
SYN 泛洪
```

. 多路复用
```
```
https://mp.weixin.qq.com/s/1B0B8PFKE346jqeCDe2MiA
https://mp.weixin.qq.com/s/5XSM-yLwLc-HyoDdCCNyCA
https://mp.weixin.qq.com/s/Hay2D6zHMUaS4NiSKyWhZA
https://mp.weixin.qq.com/s/OnUIVko6jlvkfW0cJ0IcIw
##### HTTP

浏览器输入url的过程
浏览器错误号
##### UDP

udp如何实现可靠性