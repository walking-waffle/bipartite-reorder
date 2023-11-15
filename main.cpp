#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>
#include "Reorder.h"
#include "Graph.h"

using namespace std;

int getCommand() {
    cout << "========================" << endl;
    cout << "init graph             0" << endl;
    cout << "Tranfer to CSR         1" << endl;
    cout << "Joint Degree Order     2" << endl;
    cout << "Separate Degree Order  3" << endl;
    cout << "HubSort                4" << endl;
    cout << "HubCluster             5" << endl;
    cout << "Random Order           6" << endl;
    cout << "My Order               7" << endl;
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

        // 讀檔，且檔案是 init 過的
        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";

        // 程式無法判斷輸入圖為有向或無向，所以他能把無向圖轉成有向圖的形式
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
        cout << "< HubSort >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";

        start = clock();
        hubSort( edgeList, leftSize, rightSize );
        end = clock();
        cout << "HubSort finish.\n";
        cout << "Time Cost: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << "ms" << endl;

        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_HS" );
        break;
    } // case 4

    case 5:{
        cout << "< HubCluster >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";

        start = clock();
        hubCluster( edgeList, leftSize, rightSize );
        end = clock();
        cout << "HubCluster finish.\n";
        cout << "Time Cost: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << "ms" << endl;

        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_HC" );
        break;
    } // case 5

    case 6:{
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
    } // case 6

    case 7:{
        cout << "< My Order >" << endl;
        cout << "Please input the file: ";
        string fileName = "";
        cin >> fileName;

        int leftSize = 0, rightSize = 0;
        vector<Edge> edgeList;

        readEdgeList( fileName, edgeList, leftSize, rightSize );
        cout << "Read edgeList file finish.\n";

        start = clock();
        myOrder( edgeList, leftSize, rightSize );
        end = clock();
        cout << "my order finish.\n";
        cout << "Time Cost: " << (1000.0)*(double)(end-start)/CLOCKS_PER_SEC << "ms" << endl;

        writeEdgeListFile( fileName, edgeList, leftSize, rightSize, "_MO" );
        break;
    } // case 7

    default:
        cout << "command error!\n";
        break;
    } // switch

} // main()
