#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include "Reorder.h"

using namespace std;

bool greaterThan ( Node i, Node j ) { return ( i.degree > j.degree ); }

vector<int> findIndex( vector<Node> originList ) {
    int len = originList.size();
    vector<int> invertedList( len, 0 );
    for ( int i = 0; i < len; i++ )
        invertedList.at(originList.at(i).id) = i;

    return invertedList;
} // findIndex

void separateDegreeOrder( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    Node node;
    node.degree = 0;
    vector<Node> leftDegreeList( leftSize, node );
    vector<Node> rightDegreeList( rightSize, node );

    int srcID = 0, dstID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        srcID = edgeList.at(i).src.id;
        leftDegreeList.at(srcID).id = srcID;
        leftDegreeList.at(srcID).degree++;

        dstID = edgeList.at(i).dst.id-leftSize;
        rightDegreeList.at(dstID).id = dstID;
        rightDegreeList.at(dstID).degree++;
    } // for

    sort( leftDegreeList.begin(), leftDegreeList.end(), greaterThan );
    sort( rightDegreeList.begin(), rightDegreeList.end(), greaterThan );

    vector<int> invertedNewLeft = findIndex( leftDegreeList );
    vector<int> invertedNewRight = findIndex( rightDegreeList );

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
    int srcID = 0, dstID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        srcID = edgeList.at(i).src.id;
        degreeList.at(srcID).id = srcID;
        degreeList.at(srcID).degree++;

        dstID = edgeList.at(i).dst.id;
        degreeList.at(dstID).id = dstID;
        degreeList.at(dstID).degree++;
    } // for

    sort( degreeList.begin(), degreeList.end(), greaterThan );

    vector<int> invertedNewOrder = findIndex( degreeList );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewOrder.at(originID);
        originID = edgeList.at(i).dst.id;
        edgeList.at(i).dst.id = invertedNewOrder.at(originID);
    } // for
} // jointDegreeOrder

void hubSort( vector<Edge> & edgeList, int leftSize, int rightSize ) {

    Node node;
    node.degree = 0;
    vector<Node> leftDegreeList( leftSize, node );
    vector<Node> rightDegreeList( rightSize, node );

    int srcID = 0, dstID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        srcID = edgeList.at(i).src.id;
        leftDegreeList.at(srcID).id = srcID;
        leftDegreeList.at(srcID).degree++;

        dstID = edgeList.at(i).dst.id-leftSize;
        rightDegreeList.at(dstID).id = dstID;
        rightDegreeList.at(dstID).degree++;
    } // for

    int avgDegree = edgeList.size() * 2 / ( leftSize+rightSize );

    vector<Node> hotLeft;
    vector<Node> coldLeft;
    for ( int i = 0; i < leftDegreeList.size(); i++ ) {
        if ( leftDegreeList.at(i).degree > avgDegree )
            hotLeft.push_back( leftDegreeList.at(i) );
        else
            coldLeft.push_back( leftDegreeList.at(i) );
    } // for

    vector<Node> hotRight;
    vector<Node> coldRight;
    for ( int i = 0; i < rightDegreeList.size(); i++ ) {
        if ( rightDegreeList.at(i).degree > avgDegree )
            hotRight.push_back( rightDegreeList.at(i) );
        else
            coldRight.push_back( rightDegreeList.at(i) );
    } // for

    sort( hotLeft.begin(), hotLeft.end(), greaterThan );
    sort( hotRight.begin(), hotRight.end(), greaterThan );

    hotLeft.insert( hotLeft.end(), coldLeft.begin(), coldLeft.end() );
    hotRight.insert( hotRight.end(), coldRight.begin(), coldRight.end() );

    vector<int> invertedNewLeft = findIndex( hotLeft );
    vector<int> invertedNewRight = findIndex( hotRight );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewLeft.at(originID);
        originID = edgeList.at(i).dst.id - leftSize;
        edgeList.at(i).dst.id = invertedNewRight.at(originID) + leftSize;
    } // for
} // hubSort

void hubCluster( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    Node node;
    node.degree = 0;
    vector<Node> leftDegreeList( leftSize, node );
    vector<Node> rightDegreeList( rightSize, node );

    int srcID = 0, dstID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        srcID = edgeList.at(i).src.id;
        leftDegreeList.at(srcID).id = srcID;
        leftDegreeList.at(srcID).degree++;

        dstID = edgeList.at(i).dst.id-leftSize;
        rightDegreeList.at(dstID).id = dstID;
        rightDegreeList.at(dstID).degree++;
    } // for

    int avgDegree = edgeList.size() * 2 / ( leftSize+rightSize );

    vector<Node> hotLeft;
    vector<Node> coldLeft;
    for ( int i = 0; i < leftDegreeList.size(); i++ ) {
        if ( leftDegreeList.at(i).degree > avgDegree )
            hotLeft.push_back( leftDegreeList.at(i) );
        else
            coldLeft.push_back( leftDegreeList.at(i) );
    } // for

    vector<Node> hotRight;
    vector<Node> coldRight;
    for ( int i = 0; i < rightDegreeList.size(); i++ ) {
        if ( rightDegreeList.at(i).degree > avgDegree )
            hotRight.push_back( rightDegreeList.at(i) );
        else
            coldRight.push_back( rightDegreeList.at(i) );
    } // for

    hotLeft.insert( hotLeft.end(), coldLeft.begin(), coldLeft.end() );
    hotRight.insert( hotRight.end(), coldRight.begin(), coldRight.end() );

    vector<int> invertedNewLeft = findIndex( hotLeft );
    vector<int> invertedNewRight = findIndex( hotRight );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewLeft.at(originID);
        originID = edgeList.at(i).dst.id - leftSize;
        edgeList.at(i).dst.id = invertedNewRight.at(originID) + leftSize;
    } // for
} // hubCluster

vector<int> shuffleList( int numOfNodes ) {
    vector<int> v;
    for ( int i = 0; i < numOfNodes; i++ )
        v.push_back(i);

    unsigned seed = 0;
    shuffle( v.begin(), v.end(), default_random_engine(seed));

    return v;
} // shuffleList

void randomOrder( vector<Edge> & edgeList, int numOfNodes ) {
    vector<int> randomList;
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

bool lessThan ( Node i, Node j ) { return ( i.avgNeighborID < j.avgNeighborID ); }

void myOrder( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    Node node;
    node.degree = 0;
    node.avgNeighborID = 0;
    vector<Node> avgNeighborIDLeftList( leftSize, node );
    vector<Node> avgNeighborIDRightList( rightSize, node );

    // init
    int srcIdx = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        srcIdx = edgeList.at(i).src.id;
        avgNeighborIDLeftList.at(srcIdx).id = srcIdx;
        avgNeighborIDLeftList.at(srcIdx).degree++;
        avgNeighborIDLeftList.at(srcIdx).avgNeighborID = avgNeighborIDLeftList.at(srcIdx).avgNeighborID + float(edgeList.at(i).dst.id);
    } // for

    // 計算鄰居 ID 平均
    for ( int i = 0; i < avgNeighborIDLeftList.size(); i++ )
        avgNeighborIDLeftList.at(i).avgNeighborID = avgNeighborIDLeftList.at(i).avgNeighborID/avgNeighborIDLeftList.at(i).degree;

    sort( avgNeighborIDLeftList.begin(), avgNeighborIDLeftList.end(), lessThan );
    vector<int> invertedNewLeft = findIndex( avgNeighborIDLeftList );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewLeft.at(originID);
    } // for

    // init
    int dstIdx = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        dstIdx = edgeList.at(i).dst.id - leftSize;
        avgNeighborIDRightList.at(dstIdx).id = dstIdx;
        avgNeighborIDRightList.at(dstIdx).degree++;
        avgNeighborIDRightList.at(dstIdx).avgNeighborID = avgNeighborIDRightList.at(dstIdx).avgNeighborID + float(edgeList.at(i).src.id);
    } // for

    // 計算鄰居 ID 平均
    for ( int i = 0; i < avgNeighborIDRightList.size(); i++ )
        avgNeighborIDRightList.at(i).avgNeighborID = avgNeighborIDRightList.at(i).avgNeighborID/avgNeighborIDRightList.at(i).degree;

    sort( avgNeighborIDRightList.begin(), avgNeighborIDRightList.end(), lessThan );
    vector<int> invertedNewRight = findIndex( avgNeighborIDRightList );

    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).dst.id - leftSize;
        edgeList.at(i).dst.id = invertedNewRight.at(originID) + leftSize;
    } // for
} // myOrder
