scrcpy是一款性能极佳的android screen mirror软件. 考虑到源代码量不算大,所以作为个人的C语言入门项目来阅读.在此简记.

源项目repo: https://github.com/Genymobile/scrcpy

## 1. 说明

项目目前共两个分支:

- master:对源仓库进行裁剪后的版本(app+server) + 个人复刻的版本(appcopy)
- pure-complete: 对源仓库进行裁剪后的版本(app+server)

为了方便阅读scrcpy的源码,个人对源代码进行了裁剪:

1.  将源码回滚到了 tag: hidpiscale版本.
2. 将meson构建系统转为cmake构建.
3. 删除手机端所需要的所有java代码,仅保留可运行jar包. (由于只分析c语言,所以不想看再分析java的相关代码).
4. 删除所有keyevent和mouseevent控制, 仅保留镜像功能.

## 2. 架构

scrcpy本质上就是一个socket通信,所有有server端和client端,但是为了避免混淆,下面用pc端和phone端来解释.

总架构:

![](https://raw.githubusercontent.com/ravenxrz/phonemirror/master/pic/0.png)

Phone端会不停地采集屏幕信息然后将其编码并发送给pc端.

pc端则是不停地接收socket中的frame,解码后回显到屏幕上.

那这个过程涉及到哪些技术呢?

简单说来,就以下几个.

- 编码,解码 --> ffmpeg.
- UI展示 --> SDL2
- 多线程
- 网络编程

## 3. 流程

ok,让我们再深入一点.

### phone端

为什么pc中scrcpy命令一键入后,phone会自动推流? phone中并没有安装任何软件?

看看源码中server目录下是否有一个jar包? 

![](https://raw.githubusercontent.com/ravenxrz/phonemirror/master/pic/1.png)

没错,就是它在起作用呢. 每次scrcpy程序启动后,第一件事做的就是将这个jar包push到phone中,然后将它启动,就是它负责采集phone端屏幕信息, 编码, 推流的.

至于这个jar包具体怎么做的,那就要去看server端的java源码了. 我不关心java端,所以不做分析.

### pc端

现在回到pc端, 下图展示了整个交互流程.

官方图:

```
                     HOST              DEVICE
listen on port        |                  |
push/start the server |----------------->||app_process loads the jar
init SDL             ||                  ||
                     ||                  ||
                     ||                  ||
                     ||                  ||
                     ||                  ||
                     ||                  ||
accept the connection .                  ||
                      .                  X execution of our java main
connection accepted   |<-----------------| connect to the host
init window/renderer  |                  |
display frames        |<-----------------| send frames
                      |<-----------------|

```

![](https://raw.githubusercontent.com/ravenxrz/phonemirror/master/pic/2.png)

ok,更多的就去看源码吧.

