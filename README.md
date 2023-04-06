# COMP90024CCC-Assignment1-large-tweet-analysis
### 现状
只完成了task2的串行部分，且只完成了最直接的位置辨认。已经完成阅读文件的不同位置，如指定阅读文件的前12.5%-前25%。
运行时间巨慢，仅task2的tiny就需要6s。

### 项目组成
请把task1，task3的主要实现逻辑写入另外的library，如task1.h&task1.cpp。
main.cpp读取每一条tweet的username和place full_name。task1，task2，task3函数使用这两个值做相应计算，并修改相应map。

### 项目运行
g++ -o app main.cpp task2.cpp [others.cpp]
./app
