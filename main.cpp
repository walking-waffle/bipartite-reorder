#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

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

vector<Edge> makeStart0 ( vector<Edge> edgeList ) {
    int leftMinID = edgeList.at(0).src.id;
    int rightMinID = edgeList.at(0).dst.id;

    for ( int i = 1; i < edgeList.size(); i++ ) {
        if ( edgeList.at(i).src.id < leftMinID )
            leftMinID = edgeList.at(i).src.id;
        if ( edgeList.at(i).dst.id < rightMinID )
            rightMinID = edgeList.at(i).dst.id;
    } // for

    for ( int i = 0; i < edgeList.size(); i++ ) {
        edgeList.at(i).src.id = edgeList.at(i).src.id - leftMinID;
        edgeList.at(i).dst.id = edgeList.at(i).dst.id - rightMinID;
    } // for

    return edgeList;
} // makeStart0

void findNumOfNodes( vector<Edge> input, int & leftSize, int & rightSize ) {
    leftSize = 0;
    rightSize = 0;
    for ( int i = 0; i < input.size(); i++ ) {
        if ( input.at(i).src.id > leftSize )
            leftSize = input.at(i).src.id;
        if ( input.at(i).dst.id > rightSize )
            rightSize = input.at(i).dst.id;
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
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + oper + ".txt" );

    outputFile << "% " << edgeList.size() << " " << leftSize << " " << rightSize << "\n";
    for ( int i = 0; i < edgeList.size(); i++ ) {
        outputFile << edgeList.at(i).src.id;
        outputFile << " ";
        outputFile << edgeList.at(i).dst.id;
        outputFile << " \n";
    } // for

    outputFile.close();
} // writeEdgeListFile

void init( string fileName, vector<Edge> input ) {
    ifstream inputFile( fileName );
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
    } // if

    // for konect 資料集
    string temp;
    temp = inputFile.peek();
    while ( temp == "%" ) {
        getline(inputFile, temp);
        temp = inputFile.peek();
    } // while

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
    if ( !inputFile ) {
        cerr << "Error: Unable to open input file." << endl;
        exit(1);
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
void convertToCSR( vector<Edge> & edgeList, int numOfNodes, vector<int> & csrOffsetArray, vector<int> & csrEdgeArray ) {

    csrOffsetArray.resize( numOfNodes + 1, 0 );

    // 計算每個節點的鄰居數量
    for ( int i = 0; i < edgeList.size(); i++ )
        csrOffsetArray.at(edgeList.at(i).src.id + 1)++;

    // 累積計算每個節點的起始位置
    for ( int i = 1; i <= numOfNodes; i++ )
        csrOffsetArray.at(i) += csrOffsetArray.at(i - 1);

    csrEdgeArray.resize( edgeList.size(), 0 );

    // 將邊緣列表中的節點添加到對應的位置
    vector<int> nextIndex( numOfNodes, 0 );

    for ( int i = 0; i < edgeList.size(); i++ ) {
        int node1 = edgeList.at(i).src.id;
        int node2 = edgeList.at(i).dst.id;
        int idx = csrOffsetArray.at(node1) + nextIndex.at(node1);
        csrEdgeArray.at(idx) = node2;
        nextIndex.at(node1)++;
    } // for

} // convertToCSR

bool greaterThan ( Node i, Node j ) { return ( i.degree > j.degree ); }

vector<int> findIndex( vector<Node> list ) {
    int len = list.size();
    vector<int> invertedList( len, 0 );
    for ( int i = 0; i < len; i++ )
        invertedList.at(list.at(i).id) = i;

    return invertedList;
} // findIndex

void separateDegreeOrder( vector<Edge> & edgeList, int leftSize, int rightSize ) {
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
} // separateDegreeOrder

void jointDegreeOrder( vector<Edge> & edgeList, int leftSize, int rightSize ) {
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
} // jointDegreeOrder

vector <int> shuffleList( int numOfNodes ) {
    vector <int> v;
    for ( int i = 0; i < numOfNodes; i++ )
        v.push_back(i);

    unsigned seed = 0;
    shuffle( v.begin(), v.end(), default_random_engine(seed));

    return v;
} // shuffleList

void randomOrder( vector<Edge> & edgeList, int numOfNodes ) {
    vector <int> randomList;
    for ( int i = 0; i < numOfNodes; i++ )
        randomList.push_back(i);

    unsigned seed = 0;
    shuffle( randomList.begin(), randomList.end(), default_random_engine(seed));

    int temp = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        temp = edgeList.at(i).src.id;
        edgeList.at(i).src.id = randomList.at(temp);
        temp = edgeList.at(i).dst.id;
        edgeList.at(i).dst.id = randomList.at(temp);
    } // for
} // randomOrder

int countDistance( vector<int> csrOffsetArray, vector<int> csrEdgeArray ) {
    long long int idDistance = 0;
    int startIndex = 0, lastIndex = 0;
    int maxID = 0, minID = 0;
    for ( int i = 0; i < csrOffsetArray.size()-1; i++ ) {
        startIndex = csrOffsetArray.at(i);
        lastIndex = csrOffsetArray.at(i+1)-1;
        minID = csrEdgeArray.at(startIndex);
        maxID = csrEdgeArray.at(startIndex);
        startIndex++;
        while ( startIndex <= lastIndex ) {
            if ( csrEdgeArray.at(startIndex) < minID )
                minID = csrEdgeArray.at(startIndex);
            if ( csrEdgeArray.at(startIndex) > maxID )
                maxID = csrEdgeArray.at(startIndex);

            startIndex++;
        } // while

        idDistance = idDistance + ( maxID - minID );
            
    } // for

    cout << "total distance: " << idDistance << endl;
    idDistance = idDistance / (csrOffsetArray.size()-1);
    return idDistance;
} // countDistance

// 把 CSR 寫入檔案
void writeCSRFile( string fileName, vector<int> csrOffsetArray, vector<int> csrEdgeArray ) {
    string name = fileName.substr( 0, fileName.find(".") );
    ofstream outputFile( name + "CSR" );

    outputFile << "AdjacencyGraph\n";
    outputFile << csrOffsetArray.size() << "\n";
    outputFile << csrEdgeArray.size() << "\n";

    for ( int val : csrOffsetArray )
        outputFile << val << "\n";

    for ( int val : csrEdgeArray )
        outputFile << val << "\n";

    outputFile.close();
} // writeCSRFile

int getCommand() {
    cout << "========================" << endl;
    cout << "init graph             0" << endl;
    cout << "Tranfer to CSR         1" << endl;
    cout << "Joint Degree Order     2" << endl;
    cout << "Separate Degree Order  3" << endl;
    cout << "Random Order           4" << endl;
    cout << "========================" << endl;
    cout << "Please input the command: ";
    int command = 0;
    cin >> command;
    return command;
} // getCommand

int main() {
    clock_t start, end;

    int command = getCommand();
    switch ( command ) {
    // 把輸入圖一律變成 ID 從 0 開始，第一行存"% 邊數量、src數量、dst數量"，並讓所有ID當唯一
    case 0:{
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;
        vector<Edge> input;
        init( fileName, input );
        cout << "init success!\n";
        break;
    } // case 0

    case 1:{
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;
        vector<int> csrOffsetArray, csrEdgeArray;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";
        edgeListForUnDIR( edgeList );
        convertToCSR( edgeList, leftSize+rightSize, csrOffsetArray, csrEdgeArray );
        cout << "Convert to CSR finish.\n";
        writeCSRFile( fileName, csrOffsetArray, csrEdgeArray );

        int avgDistance = countDistance( csrOffsetArray, csrEdgeArray );
        cout << "Avg ID distance: " << avgDistance << endl;

        break;
    } // case 1

    case 2:{
        cout << "< Joint Degree Order >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";

        start = clock();
        jointDegreeOrder( edgeList, leftSize, rightSize );
        end = clock();
        cout << "Joint degree order finish.\n";
        cout << "Time Cost: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << "ms" << endl;

        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_JDO" );
        break;
    } // case 2

    case 3:{
        cout << "< Separate Degree Order >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";

        start = clock();
        separateDegreeOrder( edgeList, leftSize, rightSize );
        end = clock();
        cout << "Separate degree order finish.\n";
        cout << "Time Cost: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << "ms" << endl;

        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_SDO" );
        break;
    } // case 3

    case 4:{
        cout << "< Random Order >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";

        start = clock();
        randomOrder( edgeList, leftSize+rightSize );
        end = clock();
        cout << "Random order finish.\n";
        cout << "Time Cost: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << "ms" << endl;

        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_RO" );
        break;
    } // case 3

    default:
        cout << "command error!\n";
        break;
    } // switch

} // main()
