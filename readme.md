# 足球竞彩工具：
  18年世界杯时期，自己写了一个分析足球竞彩的小工具。工具虽然简单，但是可以增加购彩的乐趣。这段代码曾经差一点就丢了，现在拿出来给分享吧，这样就不用担心找不着了。作为一个普通球迷，边看球边买足彩也算是一种非常快乐的事情，我玩的最多的是足球竞彩胜平负， 当时的想法就是想分析一下如果参与合买，想分析一下资金如何按照一个可靠的比例购入不同赔率来提高中奖几率，于是开发了这款小工具。

# 要解决什么问题
拿2串一举例，2串1有9种组合，有100元可以买50注，买彩票的时候能不能把这9种组合全买了，如果购买的彩票合理分配资金比例，让最低中奖的资金大于自己购彩投资的钱数就能100%中奖（当然这种可能性为0。。。）。
我这款小工具当时就是要解决这种问题才开始做的，即使不能保证100%中奖，但是也能分析出资金不同配比所带来的收益，这样就能间接分析竞彩足球的数据（当然结果玩多了，以后可能会减少自己购彩的欲望。。）并提高中间几率。万一出现各种爆冷，我们在有些数据条件下确实也可以赚大了。

# 使用流程，一共3个文件
oddfile ：配置文件，输入要计算的赔率信息。赔率值要使用#来连接，如下图所示：
第一行, 赔率胜平负值使用#号连接 ，如果只配置一行就进行单关计算，同理如果有2行赔率值，则计算2串一，三行就计算3串1，下图是2行能计算2串1。

config_log.txt ： 进程输出的log信息，不用修改正常运行会生成一个log文件件存放log文件，log文件中有分析的数据信息还是比较重要的

football_ana.exe  二进制进程信息，目前只在windows下用mingw编译，理论上可以跨所有平台编译

# 使用方式，windows下可以使用命令行下执行：
football_ana.exe 

# 结果展示：
举个列子，站在合买的角度，如果自己只买20注，都去投资一场比赛，这20注该如何分配比例来提高自己中奖几率呢， 2串1有9种组合，我们最大的愿望就是20注分配到所有组合上并且最低的收益都能回本，这样中奖几率就是100%了(这种可能性是没有的),但是尽量提高组合中的中奖次数，这就是该工具的意义。结果看下图，总共买20注，按照下面比例购彩，其中有7种组合实现盈利。
