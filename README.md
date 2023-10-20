# bipartite-reorder
my master experiment
對 main.cpp 編譯後執行即可

### 指令介紹
### command 0
初始化二分無向圖，資料集來源為 konect http://konect.cc/

大部分格式為  
% bip unweighted  
% 13 6 5 (少部分資料集無此行，不影響init)  
1 1  
1 2  
1 3  
...  
  
最後初始化格式為  
% 13 6 5  
0 0  
0 1  
0 2  
...  
皆以ID為0開始，保留邊數量、左集合節點數量、右集合節點數量  
