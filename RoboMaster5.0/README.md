# RoboMaster
##项目背景
RoboMaster全国大学生机器人大赛，一个为青年工程师打造的机器人竞技平台。作为国内首个激战类机器人比赛，RoboMaster大赛在其诞生伊始就凭借其颠覆传统的机器人比赛方式、震撼人心的视听冲击力、激烈硬朗的竞技风格，吸引到全国数百所高等院校、近千家高新科技企业以及数以万计的科技爱好者的深度关注。

参赛队员走出课堂，组成机甲战队，独立研发、制作机器人参与射击对抗。他们将通过大赛获得宝贵的实践技能和战略思维，将理论与实践相结合，在激烈的竞争中打造先进的智能机器人。
##团队成员
刘泽鹏(项目主要开发人员)
李开运(项目主要开发人员)
##功能需求
1. 识别对方机器人的装甲板
2. 识别自己的激光器
3. 识别手写体九宫格数字
4. 计算云台旋转角度
##工程目录
###img
---
训练和测试用的图片，以及视频
-->test_image
-->train_image
###inc
---
工程头文件目录
-->Auto_Attack_Mode.hpp
-->Identify.hpp
-->Rune_Mode.hpp
-->Standby_Mode.hpp
-->ToolKit.hpp
###lib
---
第三方库文件目录
###main
---
工程主程序入口　
-->Makefile
-->RoboMaster.cpp
###obj
---
编译产生的中间文件
###src
---
源代码目录
-->Auto_Attack_Mode.cpp
-->Identify.cpp
-->Makefile
-->Rune_Mode.cpp
-->Standby_Mode.cpp
-->ToolKit.cpp
###Makefile
---
工程编译规则
###READEME.md
---
markdown文件

