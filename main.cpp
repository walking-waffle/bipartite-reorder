#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <iomanip>
#include "Reorder.h"
#include "Graph.h"

using namespace std;

void recordReorder( vector<string> reorderNameList, vector<double> timeLogList ) {
    ofstream outputFile( "log.txt" );
    outputFile << "                Reorder " << "|" << "         Time\n";
    outputFile << "---------------------------------------\n";

    for ( int i = 0; i < timeLogList.size(); i++ ) {
        outputFile << setw(25) << reorderNameList.at(i) << setw(10) << timeLogList.at(i) << " ms\n";
        outputFile << "---------------------------------------\n";
    } // for

    outputFile.close();
} // recordReorder

void recordCSR( vector<string> fileNameList, vector<long long int> disLogList ) {
    ofstream outputFile( "log.txt" );
    outputFile << "                    CSR " << "|" << "         Time\n";
    outputFile << "---------------------------------------\n";

    for ( int i = 0; i < disLogList.size(); i++ ) {
        outputFile << setw(25) << fileNameList.at(i) << setw(10) << disLogList.at(i) << " ms\n";
        outputFile << "---------------------------------------\n";
    } // for

    outputFile.close();
} // recordCSR

void processCSR( string fileName ) {
    int leftSize = 0, rightSize = 0;
    vector<Edge> edgeList;
    vector<int> OA, EA;

    readEdgeList( fileName, edgeList, leftSize, rightSize );
    edgeListForUnDIR( edgeList );
    convertToCSR( edgeList, leftSize+rightSize, OA, EA );
    writeCSRFile( fileName, OA, EA );
    long long int avgDistance = countDistance( OA, EA );
    cout << "dis: " << avgDistance << "\n";
} // processCSR

void processReorder( string fileName ) {
    clock_t start, end;
    vector<double> timeLogList;
    vector<string> reorderNameList;

    int leftSize = 0, rightSize = 0;
    vector<Edge> edgeList, newOrder;
    readEdgeList( fileName, edgeList, leftSize, rightSize );
    cout << "Read edgeList file finish.\n";

    start = clock();
    newOrder = separateDegreeOrder( edgeList, leftSize, rightSize );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "Separate Degree Order |" );
    sortEdgeList( newOrder );
    writeEdgeListFile( fileName, newOrder, leftSize, rightSize, "_SDO" );
/*
    start = clock();
    newOrder = hubSort( edgeList, leftSize, rightSize );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "Hub Sort |" );
    sortEdgeList( newOrder );
    writeEdgeListFile( fileName, newOrder, leftSize, rightSize, "_HS" );

    start = clock();
    newOrder = hubCluster( edgeList, leftSize, rightSize );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "Hub Cluster |" );
    sortEdgeList( newOrder );
    writeEdgeListFile( fileName, newOrder, leftSize, rightSize, "_HC" );
*/
    start = clock();
    newOrder = randomOrder( edgeList, leftSize, rightSize );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "Ramdom Order |" );
    sortEdgeList( newOrder );
    writeEdgeListFile( fileName, newOrder, leftSize, rightSize, "_RO" );

    start = clock();
    newOrder = myOrder( edgeList, leftSize, rightSize );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "My Order |" );
    sortEdgeList( newOrder );
    writeEdgeListFile( fileName, newOrder, leftSize, rightSize, "_MO" );

    start = clock();
    newOrder = NCOrder( edgeList, leftSize, rightSize );
    end = clock();
    timeLogList.push_back( (1000.0)*(double)(end-start)/CLOCKS_PER_SEC );
    reorderNameList.push_back( "NC Order |" );
    sortEdgeList( newOrder );
    writeEdgeListFile( fileName, newOrder, leftSize, rightSize, "_NO" );

    recordReorder( reorderNameList, timeLogList );
} // processReorder

int main( int argc, char *argv[] ) {
    if ( argc != 3 )
        cout << "----------Parameter Error----------\n"; 
    else {
        if ( strcmp( "init", argv[1] ) == 0 ) {
            string argv2(argv[2]);
            init( argv2 );
            cout << "init success!\n";
        } // if

        else if ( strcmp( "csr", argv[1] ) == 0 ) {
            string argv2(argv[2]);
            processCSR( argv2 );
        } // else if

        else if ( strcmp( "reorder", argv[1] ) == 0 ) {
            string argv2( argv[2] );
            processReorder( argv2 );
        } // else if

        else
            cout << "----------Error----------\n";

    } // else
} // main()