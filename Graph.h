#ifndef _Graph_H
#define _Graph_H
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

enum Set {
    leftSide,
    rightSide
};

struct Node {
    int id;
    Set set;
    int degree;
    float avgNeighborID;
};

struct Edge {
    Node src;
    Node dst;
};

void init( string fileName, vector<Edge> input );

// 把無向圖的 edgeList 轉換成有向圖的 edgeList( 兩倍大 )
void edgeListForUnDIR( vector<Edge> & edgeList );

// 只把檔案讀進edgeList
void readEdgeList( string fileName, vector<Edge> & edgeList, int & leftSize, int & rightSize );

// 將圖的edge list格式轉換為CSR格式
void convertToCSR( vector<Edge> & edgeList, int numOfNodes, vector<int> & csrOffsetArray, vector<int> & csrEdgeArray );

// 把 edge list 寫入檔案
void writeEdgeListFile( string fileName, vector<Edge> edgeList, int leftSize, int rightSize, string oper );

// 
int countDistance( vector<int> csrOffsetArray, vector<int> csrEdgeArray );

// 把 CSR 寫入檔案
void writeCSRFile( string fileName, vector<int> csrOffsetArray, vector<int> csrEdgeArray );

#endif
