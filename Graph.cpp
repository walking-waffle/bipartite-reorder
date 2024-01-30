#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>
#include "Graph.h"

using namespace std;

vector<Edge> makeStart0 ( vector<Edge> edgeList ) {
    if ( edgeList.empty() )
        return edgeList;

    int leftMinID = edgeList.at(0).src.id;
    int rightMinID = edgeList.at(0).dst.id;

    for ( int i = 1; i < edgeList.size(); i++ ) {
        leftMinID = min( leftMinID, edgeList.at(i).src.id );
        rightMinID = min( rightMinID, edgeList.at(i).dst.id );
    } // for

    for ( int i = 0; i < edgeList.size(); i++ ) {
        edgeList.at(i).src.id = edgeList.at(i).src.id - leftMinID;
        edgeList.at(i).dst.id = edgeList.at(i).dst.id - rightMinID;
    } // for

    return edgeList;
} // makeStart0

void findNumOfNodes( vector<Edge> input, int & leftSize, int & rightSize) {
    leftSize = 0;
    rightSize = 0;

    for ( Edge & edge : input) {
        leftSize = max( leftSize, edge.src.id );
        rightSize = max( rightSize, edge.dst.id );
    } // for

    leftSize++;
    rightSize++;
} // findNumOfNodes

// 把二分圖的 edgeList 轉換成不重複ID 的二分圖 edgeList
// 假設左集合有3個節點，則右集合的節點 ID 全部+3
vector<Edge> uuidEdgeList( vector<Edge> input, int leftSize ) {
    for ( int i = 0; i < input.size(); i++ )
        input.at(i).dst.id = input.at(i).dst.id + leftSize;
    
    return input;
} // uuidEdgeList

// 把 edgeList 寫入檔案
void writeEdgeListFile( string fileName, vector<Edge> edgeList, int leftSize, int rightSize, string oper ) {
    ofstream outputFile(fileName.substr(0, fileName.find(".")) + oper + ".txt");

    outputFile << "% " << edgeList.size() << " " << leftSize << " " << rightSize << "\n";
    for (const auto & edge : edgeList)
        outputFile << edge.src.id << " " << edge.dst.id << "\n";

    outputFile.close();
} // writeEdgeListFile

void init( string fileName ) {
    vector<Edge> input;
    ifstream inputFile( fileName );
    if ( !inputFile.is_open() ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(EXIT_FAILURE);
    } // if

    // for konect 資料集
    // Skip lines starting with '%'
    while ( inputFile.peek() == '%' )
        inputFile.ignore(numeric_limits<streamsize>::max(), '\n');

    Node node1, node2;
    int numOfEdges = 0;

    // 讀取邊緣列表數據
    while ( inputFile >> node1.id >> node2.id ) {
        input.push_back({node1, node2});
        numOfEdges++;
    } // while

    inputFile.close();

    // 讓節點 ID 從0開始
    input = makeStart0 ( input );

    // 計算出左右集合節點各自數量
    int leftSize = 0, rightSize = 0;
    findNumOfNodes( input, leftSize, rightSize );

    // 讓左右節點只用唯一ID
    input = uuidEdgeList( input, leftSize );

    writeEdgeListFile( fileName, input, leftSize, rightSize, "" );
} // init

// 只把檔案讀進edgeList
void readEdgeList( string fileName, vector<Edge> & edgeList, int & leftSize, int & rightSize ) {
    ifstream inputFile( fileName );
    if ( !inputFile.is_open() ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(EXIT_FAILURE);
    } // if

    int numOfEdges = 0;
    inputFile.get(); // 讀掉%
    inputFile >> numOfEdges >> leftSize >> rightSize;

    Node node1, node2;

    // 讀取邊緣列表數據
    while ( inputFile >> node1.id >> node2.id ) {
        node1.set = leftSide;
        node2.set = rightSide;
        edgeList.push_back({node1, node2});
    } // while

    cout << "Left set size: " << leftSize << endl;
    cout << "Right set size: " << rightSize << endl;
    cout << "Edge size: " << numOfEdges << endl;

    inputFile.close();

} // readEdgeList

// 把無向圖的 edgeList 轉換成有向圖的 edgeList( 兩倍大 )
void edgeListForUnDIR( vector<Edge> & edgeList ) {
    int numOfEdges = edgeList.size();
    for ( int i = 0; i < numOfEdges; i++ )
        edgeList.push_back({edgeList.at(i).dst, edgeList.at(i).src});
} // edgeListForUnDIR

// 將圖的edge list格式轉換為CSR格式
void convertToCSR( vector<Edge> & edgeList, int numOfNodes, vector<int> & OA, vector<int> & EA ) {

    OA.resize( numOfNodes + 1, 0 );

    // 計算每個節點的鄰居數量
    for ( int i = 0; i < edgeList.size(); i++ )
        OA.at(edgeList.at(i).src.id + 1)++;

    // 累積計算每個節點的起始位置
    for ( int i = 1; i <= numOfNodes; i++ )
        OA.at(i) += OA.at(i - 1);

    EA.resize( edgeList.size(), 0 );

    // 將邊緣列表中的節點添加到對應的位置
    vector<int> nextIndex( numOfNodes, 0 );

    for ( int i = 0; i < edgeList.size(); i++ ) {
        int node1 = edgeList.at(i).src.id;
        int node2 = edgeList.at(i).dst.id;
        int idx = OA.at(node1) + nextIndex.at(node1);
        EA.at(idx) = node2;
        nextIndex.at(node1)++;
    } // for

} // convertToCSR

long long int countDistance( vector<int> OA, vector<int> EA ) {
    long long int idDistance = 0;

    for ( int i = 0; i < OA.size()-1; i++ ) {
        int startIndex = OA.at(i);
        int lastIndex = OA.at(i+1)-1;

        int minID = EA.at(startIndex);
        int maxID = EA.at(startIndex);

        startIndex++;
        while ( startIndex <= lastIndex ) {
            int currentID = EA.at(startIndex);

            minID = min(minID, currentID);
            maxID = max(maxID, currentID);

            startIndex++;
        } // while

        idDistance = idDistance + ( maxID - minID );
    } // for

    idDistance = idDistance / (OA.size()-1);
    return idDistance;
} // countDistance

// 把 CSR 寫入檔案
void writeCSRFile( string fileName, vector<int> OA, vector<int> EA ) {
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + "CSR" );

    outputFile << "AdjacencyGraph\n";
    outputFile << OA.size() << "\n";
    outputFile << EA.size() << "\n";

    for ( int val : OA )
        outputFile << val << "\n";

    for ( int val : EA )
        outputFile << val << "\n";

    outputFile.close();
} // writeCSRFile

void sortEdgeList( vector<Edge> & edgeList ) {
    sort(edgeList.begin(), edgeList.end(), [](const Edge & a, const Edge & b)->bool{
		if ( a.src.id < b.src.id )
			return true;
		else if ( a.src.id > b.src.id )
			return false;
		else {
			if( a.dst.id <= b.dst.id )
				return true;
			else
				return false;
		} // else
	});
} // sortEdgeList
