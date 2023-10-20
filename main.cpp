#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>
#include <algorithm>

using namespace std;

enum Set {
    leftSide,
    rightSide
};

struct Node {
    int id;
    Set set;
    int degree;
};

struct Edge {
    Node src;
    Node dst;
};

// 把二分圖的 edgeList 轉換成不重複ID 的二分圖 edgeList
// 假設左集合有3個節點，則右集合的節點 ID 全部+3
void uuidEdgeList( vector<vector<int>> & input, int leftSize ) {
    for ( int i = 0; i < input.size(); i++ )
        input.at(i).at(1) = input.at(i).at(1) + leftSize;
} // uuidEdgeList

void init( string fileName, vector<vector<int>> input ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    // konect 資料集中，只需留下邊數量、src、dst 數量
    int numOfEdges, leftSize, rightSize;
    string temp;
    temp = inputFile.peek();
    if ( temp == "%" )
        getline(inputFile, temp); // 讀掉開頭

    temp = inputFile.peek();
    if ( temp == "%" )
        inputFile.get(); // 讀掉第二行的'%'

    inputFile >> numOfEdges >> leftSize >> rightSize;

    int node1, node2;
    int startID = 0;

    inputFile >> node1 >> node2;

    input.push_back({node1, node2});
    if ( node1 <= node2 )
        startID = node1;
    else
        startID = node2;

    // 讀取邊緣列表數據
    while ( inputFile >> node1 >> node2 ) {
        input.push_back({node1, node2});
        if ( node1 < startID )
            startID = node1;
        if ( node2 < startID )
            startID = node2;
    } // while

    if ( startID != 0 ) {
        for ( int i = 0; i < input.size(); i++ ) {
            input.at(i).at(0) = input.at(i).at(0) - startID;
            input.at(i).at(1) = input.at(i).at(1) - startID;
        } // for
    } // if

    inputFile.close();

    uuidEdgeList( input, leftSize );

    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + ".txt" );
    outputFile <<  numOfEdges << " " << leftSize << " " << rightSize << "\n";
    for ( int i = 0; i < input.size(); i++ ) {
        outputFile << input.at(i).at(0) << " ";
        outputFile << input.at(i).at(1) << "\n";
    } // for
    outputFile.close();
} // init

// 只把檔案讀進edgeList
void readEdgeList( string fileName, vector<Edge> & edgeList, int & leftSize, int & rightSize ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    int numOfEdges = 0;
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

void readCSR( string fileName, vector<int> & csrOffsetArray, vector<int> & csrEdgeArray ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    int input = 0;
    int csrOffsetArraySize = 0;
    int csrEdgeArraySize = 0;

    inputFile >> csrOffsetArraySize;
    inputFile >> csrEdgeArraySize;
    for ( int i = 0; i < csrOffsetArraySize; i++ ) {
        inputFile >> input;
        csrOffsetArray.push_back(input);
    } // for

    for ( int i = 0; i < csrEdgeArraySize; i++ ) {
        inputFile >> input;
        csrEdgeArray.push_back(input);
    } // for

    inputFile.close();
} // readCSR

// 將圖的edge list格式轉換為CSR格式
void convertToCSR( vector<Edge> & edgeList, int numOfNodes, vector<Node> & csrOffsetArray, vector<Node> & csrEdgeArray ) {

    Node node;
    csrOffsetArray.resize( numOfNodes + 1, node );

    // 計算每個節點的鄰居數量
    for ( int i = 0; i < edgeList.size(); i++ )
        csrOffsetArray.at(edgeList.at(i).src.id + 1).id++;

    // 累積計算每個節點的起始位置
    for ( int i = 1; i <= numOfNodes; i++ )
        csrOffsetArray.at(i).id += csrOffsetArray.at(i - 1).id;

    csrEdgeArray.resize( edgeList.size(), node );
    
    // 將邊緣列表中的節點添加到對應的位置
    vector<int> nextIndex( numOfNodes, 0 );

    for ( int i = 0; i < edgeList.size(); i++ ) {
        Node node1 = edgeList.at(i).src;
        Node node2 = edgeList.at(i).dst;
        int idx = csrOffsetArray.at(node1.id).id + nextIndex.at(node1.id);
        csrEdgeArray.at(idx).id = node2.id;
        nextIndex.at(node1.id)++;
    } // for

    for ( int i = 0; i < csrOffsetArray.size(); i++ )
        cout << csrOffsetArray.at(i).id << " ";
    cout << endl;
    for ( int i = 0; i < csrEdgeArray.size(); i++ )
        cout << csrEdgeArray.at(i).id << " ";
    cout << endl;
} // convertToCSR

bool greaterThan ( Node i, Node j ) { return ( i.degree > j.degree ); }

vector<int> findIndex( vector<Node> list ) {
    int len = list.size();
    vector<int> invertedList( len, 0 );
    for ( int i = 0; i < len; i++ )
        invertedList.at(list.at(i).id) = i;

    return invertedList;
} // findIndex

void individualDegreeOrder( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    Node node;
    node.degree = 0;
    vector<Node> leftDegreeList( leftSize, node );
    vector<Node> rightDegreeList( rightSize, node );

    for ( int i = 0; i < edgeList.size(); i++ ) {
        leftDegreeList.at(edgeList.at(i).src.id).id = edgeList.at(i).src.id;
        leftDegreeList.at(edgeList.at(i).src.id).degree++;
        rightDegreeList.at(edgeList.at(i).dst.id-leftSize).id = edgeList.at(i).dst.id-leftSize;
        rightDegreeList.at(edgeList.at(i).dst.id-leftSize).degree++;
    } // for

    vector<Node> newLeft = leftDegreeList;
    vector<Node> newRight = rightDegreeList;
    sort( newLeft.begin(), newLeft.end(), greaterThan );
    sort( newRight.begin(), newRight.end(), greaterThan );

    vector<int> invertedNewLeft = findIndex( newLeft );
    vector<int> invertedNewRight = findIndex( newRight );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewLeft.at(originID);
        originID = edgeList.at(i).dst.id - leftSize;
        edgeList.at(i).dst.id = invertedNewRight.at(originID) + leftSize;
    } // for
} // individualDegreeOrder

void togetherDegreeOrder( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    Node node;
    node.degree = 0;
    vector<Node> degreeList( leftSize + rightSize, node );

    // init
    for ( int i = 0; i < edgeList.size(); i++ ) {
        degreeList.at(edgeList.at(i).src.id).id = edgeList.at(i).src.id;
        degreeList.at(edgeList.at(i).src.id).degree++;

        degreeList.at(edgeList.at(i).dst.id).id = edgeList.at(i).dst.id;
        degreeList.at(edgeList.at(i).dst.id).degree++;
    } // for

    vector<Node> newOrder = degreeList;
    sort( newOrder.begin(), newOrder.end(), greaterThan );

    vector<int> invertedNewOrder = findIndex( newOrder );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewOrder.at(originID);
        originID = edgeList.at(i).dst.id;
        edgeList.at(i).dst.id = invertedNewOrder.at(originID);
    } // for
} // togetherDegreeOrder

// 輸入 CSR 圖的 offset array，輸出最大 degree 的 index
int findMaxDegreeIndex( vector<Node> csrOffsetArray ) {
    int max = 0;
    int numNeighbor = 0;
    int index = -1;
    for ( int i = 1; i < csrOffsetArray.size(); i++ ) {
        numNeighbor = csrOffsetArray.at(i).id - csrOffsetArray.at(i-1).id;
        if ( max < numNeighbor ) {
            max = numNeighbor;
            index = i-1;
        } // if
    } // for

    cout << "maximum degree index: " << index << endl;
    cout << "maximum degree: " << max << endl;
    return index;
} // findMaxDegreeIndex

// 廣度優先搜索 (BFS)
vector <int> bfs( vector<Node> csrOffsetArray, vector<Node> csrEdgeArray, int startNode ) {
    int numOfNodes = csrOffsetArray.size() - 1;
    vector <int> result;

    // 記錄節點是否被訪問過
    vector<bool> visited( numOfNodes, false );

    // 起始節點先 push 進 queue
    queue<int> q;
    q.push(startNode);
    visited.at(startNode) = true;

    while ( !q.empty() ) {
        int currentNode = q.front();
        q.pop();

        // 找到目標
        result.push_back(currentNode);

        // 將當前節點的鄰居節點加入 queue
        for ( int i = csrOffsetArray.at(currentNode).id; i < csrOffsetArray.at(currentNode + 1).id; i++ ) {
            int neighbor = csrEdgeArray.at(i).id;

            // 如果鄰居節點尚未被訪問過
            if ( !visited.at(neighbor) ) {
                q.push(neighbor);
                visited.at(neighbor) = true;
            } // if
        } // for
    } // while

    return result;
} // bfs

// 把 CSR 寫入檔案
void writeCSRFile( string fileName, vector<Node> csrOffsetArray, vector<Node> csrEdgeArray ) {
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + "CSR" );

    outputFile << "AdjacencyGraph\n";
    outputFile << csrOffsetArray.size() << "\n";
    outputFile << csrEdgeArray.size() << "\n";

    for ( Node val : csrOffsetArray )
        outputFile << val.id << "\n";

    for ( Node val : csrEdgeArray )
        outputFile << val.id << "\n";

    outputFile.close();
} // writeCSRFile

// 把 reordering 後的 edgeList 寫入檔案
void writeEdgeListFile( string fileName, vector<Edge> edgeList, int leftSize, int rightSize, string oper ) {
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + oper + ".txt" );

    outputFile << edgeList.size() << " " << leftSize << " " << rightSize << "\n";
    for ( int i = 0; i < edgeList.size(); i++ ) {
        outputFile << edgeList.at(i).src.id;
        outputFile << " ";
        outputFile << edgeList.at(i).dst.id;
        outputFile << " \n";
    } // for

    outputFile.close();
} // writeEdgeListFile

int getCommand() {
    cout << "===========================" << endl;
    cout << "init graph                0" << endl;
    cout << "Individual Degree Order   1" << endl;
    cout << "Together Degree Order     2" << endl;
    cout << "graphAlgo                 3" << endl;
    cout << "===========================" << endl;
    cout << "Please input the command: ";
    int command = 0;
    cin >> command;
    return command;
} // getCommand

int main() {

    int command = getCommand();
    switch ( command ) {
    // 把輸入圖一律變成 ID 從 0 開始，第一行存邊數量、src數量、dst數量，並讓所有ID當唯一
    case 0:{
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;
        vector<vector<int>> input;
        init( fileName, input );
        cout << "init success!\n";
        break;
    } // case 0

    case 1:{
        cout << "< Individual Degree Order >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";
        individualDegreeOrder( edgeList, leftSize, rightSize );
        cout << "Individual degree order finish.\n";
        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_IDO" );
        break;
    } // case 1

    case 2:{
        cout << "< Together Degree Order >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";
        togetherDegreeOrder( edgeList, leftSize, rightSize );
        cout << "Together degree order finish.\n";
        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_TDO" );
        break;
    } // case 2

    case 3:{
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;
        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;
        vector<Node> csrOffsetArray, csrEdgeArray;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";
        edgeListForUnDIR( edgeList );
        convertToCSR( edgeList, leftSize+rightSize, csrOffsetArray, csrEdgeArray );
        cout << "Convert to CSR finish.\n";
        writeCSRFile( fileName, csrOffsetArray, csrEdgeArray );

        srand(time(0));
        clock_t start, end;

        int maxDegreeIndex = findMaxDegreeIndex( csrOffsetArray );
        for( int i = 0; i < 3; i++ ) {
            start=clock();
            // BFS
            vector<int> bfsTravelList;
            bfsTravelList = bfs( csrOffsetArray, csrEdgeArray, maxDegreeIndex );
            end=clock();
            cout << "Time Cost: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << "ms\n";
        } // for

        break;
    } // case 3
        
    default:
        cout << "command error!\n";
        break;
    } // switch

} // main()
