# Hi3861 OpenHarmony 嵌入式技术及应用

![OS](https://img.shields.io/badge/OS-OpenHarmony-brightgreen?style=for-the-badge) ![platform](https://img.shields.io/badge/platform-Hi3861V100-brightgreen?logo=huawei&style=for-the-badge) ![license](https://img.shields.io/badge/license-Apache-brightgreen?style=for-the-badge)

## 介绍

使用Hi3861V100实现简易版贪吃蛇.

## 硬件说明

这是一个嵌入式软件项目,需要有一块Hi3861V100的开发板及OLED显示屏. 

## 快速上手

可以在Windows环境或"Windows+Linux虚拟机"环境下使用本项目的代码.

### Windows IDE环境搭建

如果在Windows下搭建编译开发环境（目前提供两种编译方式，第一种新建工程，第二种导入工程，任选其一即可实现Hi3861V100编译）, 我们推荐Windows 10 64位系统或以上版本, 简要步骤如下(详细内容参考doc目录下<物联网技术及应用实验指导手册>):
#### 新建工程
1. 下载并安装Windows版本的HUAWEI DevEco Device Tool(devicetool-windows-tool-3.1.0.500.zip)：https://device.harmonyos.com/cn/develop/ide#download

2. 新建工程: 打开已安装DevEco Decive Tool插件的VSCode, 在DevEco Device Tool主页点击"导入工程", 弹窗中选择SDK代码目录, 点击“新建工程”.

3. 后续弹窗"SOC"选择"HI3861", 开发板选择"hi3861", 工程名选择"用户自定义"，工程路径选择”用户自定义“，SDK显示”hi3861_hdu_iot@1.0.0(uninstalled)“,点击"下载".
> **注意：由于windows自身限制，路径不能超过260个字符，在git下载和解压Hi3861 SDK代码时尽量放在磁盘根目录下，防止导致的编译错误问题**
4. 编译: 点击左侧“build”.

5. 烧录: 硬件连接电脑, 如电脑未安装CH340G驱动, 先安装DevTools_Hi3861V100_v1.0/usb_serial_driver路径下的CH341SER.EXE串口驱动. 然后点击左侧“工程配置”, 找到“upload_port”选项, 选择开发板对应的烧录串口进行烧录. 

6. 按一下复位键, 现在, 你的第一个OpenHarmony程序已经在你的开发板上运行起来了. :thumbsup:
#### 导入工程
   1. 下载并解压Hi3861V100编译工具链：
      https://hispark.obs.cn-east-3.myhuaweicloud.com/DevTools_Hi3861V100_v1.0.zip

   2. 拉取本项目的SDK代码到本地：

      ```bash
      git clone git@github.com:DylanLi0720/Greedy-Snake-LiteOS.git
      ```

      > **注意：由于windows自身限制，路径不能超过260个字符，在git下载和解压Hi3861 SDK代码时尽量放在磁盘根目录下，防止导致的编译错误问题**

   3. 下载并安装Windows版本的HUAWEI DevEco Device Tool(devicetool-windows-tool-3.1.0.500.zip)：https://device.harmonyos.com/cn/develop/ide#download

   4. 导入SDK: 打开已安装DevEco Decive Tool插件的VSCode, 在DevEco Device Tool主页点击"导入工程", 弹窗中选择SDK代码目录, 点击“导入”.

   5. 后续弹窗"SOC"选择"HI3861", 开发板选择"hi3861", 点击"导入".

   6. 配置编译工具链路径: 点击左侧的“工程配置”, 在右侧窗口找到“compiler_bin_path”, 选择到之前下载的开发工具路径, 选择`env_set.py`文件所在的目录层级.

   7. 编译: 点击左侧“build”.

   8. 烧录: 硬件连接电脑, 如电脑未安装CH340G驱动, 先安装DevTools_Hi3861V100_v1.0/usb_serial_driver路径下的CH341SER.EXE串口驱动. 然后点击左侧“工程配置”, 找到“upload_port”选项, 选择开发板对应的烧录串口进行烧录. 

   9. 按一下复位键, 现在, 你的第一个OpenHarmony程序已经在你的开发板上运行起来了. :thumbsup:

### Windows命令行编译环境搭建

为了方便习惯命令行编译的开发者使用, 我们同时也支持在Windows命令行环境编译方式:
前两步操作与IDE环境搭建方式相同, 即下载并解压Hi3861V100编译工具链和拉取SDK代码到本地. 然后:
1. 进入DevTools_Hi3861V100_v1.0.zip解压后目录, 双击运行`env_start.bat`, 则将在完成首次配置后, 进入一个转为编译Hi3861V100而配置的命令行环境;
2. 在命令行窗口中切换到SDK所在的src目录:
    ```
    [DevTools] D:\DevTools_Hi3861V100_v1.0>cd d:\Greedy-Snake-LiteOS\src
    ```
3. 执行命令 `hb set`, 直接回车选择当前缺省选项, 执行命令`hb build`:
    ```
    [DevTools] D:\Greedy-Snake-LiteOS\src>hb set
    [DevTools] D:\Greedy-Snake-LiteOS\src>hb build
    ```
    即完成编译. 
    
    > **如有提示`account_related_group_manager_mock.c: No such file or directory`之类报错信息, 是Windows系统文件路径不能超过260字符的限制所致, 请尝试将SDK代码仓放置于较浅层的磁盘目录下重新尝试**
4. 编译后镜像文件位于 out/hispark_pegasus/wifiiot_hispark_pegasus/Hi3861_wifiiot_app_allinone.bin, 使用DevTools_Hi3861V100_v1.0/burntool/BurnTool.exe完成烧录. (如果缺少USB转串口驱动, 则执行usb_serial_driver\CH341SER.EXE安装)

### Linux环境搭建

如果在Linux下搭建编译开发环境, 我们推荐的虚拟机系统配置为VirtualBox 6.0 + Ubuntu20.04, 推荐虚拟机内存2G以上, 虚拟机硬盘20G以上. 你可以通过搜索学习相关的网络文章实现安装虚拟机Linux.

我们推荐使用两块虚拟机网卡, 一块设置成NAT方式, 用于虚拟机连接外部网络, 一块使用Host Only模式, 用于宿主机连接虚拟机, 这样你会遇到最少的问题.

装好虚拟机Linux后, 你可以参考doc目录下的教程手动安装所需的Linux软件, 搭建所有的软件编译环境. 如果你觉得自己从头搭建环境对你来说太复杂, 或者担心新装软件会与原本系统里的一些软件冲突, 又或者你只是想小试一下OpenHarmony的开发体验, 或者你单纯就是懒的话!😶, 我们推荐你使用我们已经封装好的Docker, 因为它是如此的方便! 你只要按照以下的指导一步一步输入命令就可以:

1. 安装docker(如果你的Ubuntu系统没有docker的话)

   ```bash
   sudo apt install docker.io -y
   ```

   或者

   ```bash
   curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
   ```

2. 拉取我们封装好的Docker镜像到本地

   ```bash
   docker pull hispark/hi3861_hdu_iot_application:1.0
   ```

3. 新建一个容器命名为openharmony, 映射你的用户目录~到容器内目录/home/hispark, 同时把容器端口22映射为外部端口2222

   ```bash
   docker run -itd -p 2222:22 -v ~/code:/home/hispark --name openharmony hispark/hi3861_hdu_iot_application:1.0
   ```

4. 进入容器

   ```bash
   docker exec -it openharmony /bin/bash
   ```
   >  现在你已经有了一个专门用来编译代码的Docker容器环境了. 你每次可以在虚拟机linux中执行命令行`docker exec -it openharmony /bin/bash`进入这个Docker容器环境, 也可以在Windows中通过ssh软件(推荐MobaXTerm)连接虚拟机的2222端口进入(账户名root, 密码123456)

5. 进入目录拉取代码
    通过命令行或ssh进入容器内部环境中, 执行
    ```
    cd /home/hispark
    git clone git@github.com:DylanLi0720/Greedy-Snake-LiteOS.git
    ```
    > **我们建议你把所有代码工作都保存在/home/hispark这个映射目录中, 这是因为Docker的容器环境是临时性的, 当Docker容器销毁时, 内部所有数据信息都会被删除而且无法恢复, 这就是为什么我们强烈建议你把代码工作保存在映射的用户目录中, 因为这里是你真实的用户存储空间, 不会随Docker容器销毁而消失.**

6. 编译: 进入src目录, 执行命令`hb set`, 回车两次, 配置OpenHarmony信息
    ```bash
    cd Greedy-Snake-LiteOS/src
    hb set
    hb build -f
    ```

    > 编译完成后的固件镜像在src/out目录中. 编译后的镜像名为Hi3861_loader_signed.bin和Hi3861_wifiiot_app_burn.bin

7. 烧录: 编译后的镜像文件copy到Windows中(通过samba或ssh), 然后运行HiBurn([下载](https://ost.51cto.com/resource/29)), 将镜像下载到板上运行. 这里我还是推荐你用命令行的方式运行: 在Windows中建立这样一个脚本, 并命名为例如fast_burn.bat之类的名字, 复制以下内容, 并将大括号{}部分替换为你的实际信息, 修改并保存.

    ```bat
    @ fast_burn.bat
    copy
    \\{samba路径}\Greedy-Snake-LiteOS\src\out\hispark_pegasus\wifiiot_hispark_
    pegasus\Hi3861_loader_signed.bin .
    copy
    \\{samba路径}\Greedy-Snake-LiteOS\src\out\hispark_pegasus\wifiiot_hispark_
    pegasus\Hi3861_wifiiot_app_burn.bin .
    {HiBurn路径} -com:{串口端口号} -bin:Hi3861_wifiiot_app_burn.bin -signalbaud:2000000 -2ms -
    loader:Hi3861_loader_signed.bin
    ```
    
    比如, 我这里的虚拟机网卡IP是192.168.101.56, 我在Win10中通过samba去访问我的代码路径是\\192.168.101.56\share\code, hiburn存放在我电脑的d:\hispark\util目录下, 开发板接入我的电脑, 设备管理器里查看串口号为4, 所以我这里的fast_burn.bat是这样的

    ```bat
    @ fast_burn.bat
    copy
    \\192.168.101.56\share\code\Greedy-Snake-LiteOS\src\out\hispark_pegasus\wifiiot_hispark_
    pegasus\Hi3861_loader_signed.bin .
    copy
    \\192.168.101.56\share\code\Greedy-Snake-LiteOS\src\out\hispark_pegasus\wifiiot_hispark_
    pegasus\Hi3861_wifiiot_app_burn.bin .
    d:\hispark\util\hiburn.exe -com:4 -bin:Hi3861_wifiiot_app_burn.bin -signalbaud:2000000 -2ms -
    loader:Hi3861_loader_signed.bin
    ```
    假设上述一切顺利的话, 现在双击这个fast_burn.bat, 将会跳出一个命令行窗口, 并提示你按一下板子的复位按键. 按开发板的复位键后将会自动进入固件烧录过程, 烧录完毕后窗口会自动关闭.

8. 最后再按一下复位键

### HiHope Pegesus, BearPI Nano和华清远见FS-Hi3861

HiHope Pegesus, BearPI Nano, 华清远见Fs-Hi3861等Vendor的Demo, 请分别参阅
+ [Hihope Pegesus的参考文档](src/vendor/hihope/hispark_pegasus/Hihope-hispark_pegasus-十分钟上手.md)
+ [BearPI Nano的参考文档](src/vendor/bearpi/bearpi_hm_nano/doc/BearPi-HM_Nano开发指导.md)
+ [华清远见FS-Hi3861的参考文档](<src/vendor/hqyj/fs_hi3861/doc/华清远见 FS_Hi3861开发指导.md>)


## 参考

- [HarmonyOS物联网开发课程](https://developer.huawei.com/consumer/cn/training/course/mooc/C101641968823265204?refresh=1669428623989)

