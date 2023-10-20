# bipartite-reorder
my master experiment  
對 main.cpp 編譯後執行即可，實驗用 code 所以寫得很隨便  

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
0 6  
0 7  
0 8  
...  
皆以ID為0開始，左右集合使用唯一ID，右集合新ID為舊ID+左集合節點數量  
檔案保留開頭邊數量、左集合節點數量、右集合節點數量  

### command 1
輸入 init 後的 edgeList 二分圖，會讓左集合的點依照 degree 排序，按照 degree 大到小重新分配 ID ( reorder )  
右集合做一樣的事情，兩邊集合各自進行 reorder，最後輸出新的 edgeList 二分圖  

### command 2
輸入 init 後的 edgeList 二分圖，會讓兩邊集合的點依照 degree 排序，按照 degree 大到小重新分配 ID ( reorder )  
兩邊集合一起進行 reorder，最後輸出新的 edgeList 二分圖

### command 3
輸入 init 後的 edgeList 二分圖，轉換成 CSR 的 rMat 格式( 可直接給ligra執行 )  
AdjacencyGraph  
10  
22  
0  
2  
5  
...  
第1行可讀掉，第2行為 CSR offset array 大小，第3行為 CSR edge array 大小  
以上面為例，offset array 大小為10，edge array 大小為22  
第4到13行是 offset array 的連續元素，第14到35行是 edge array 的連續元素  
