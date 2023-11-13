#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include "Reorder.h"

using namespace std;

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

void hubSort( vector<Edge> & edgeList, int leftSize, int rightSize ) {
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

    int avgDegree = edgeList.size() / ( leftSize+rightSize ) * 2;

    vector<Node> hotLeft;
    vector<Node> coldLeft;
    for ( int i = 0; i < leftDegreeList.size(); i++ ) {
        if ( leftDegreeList.at(i).id > avgDegree )
            hotLeft.push_back( leftDegreeList.at(i) );
        else
            coldLeft.push_back( leftDegreeList.at(i) );
    } // for

    vector<Node> hotRight;
    vector<Node> coldRight;
    for ( int i = 0; i < rightDegreeList.size(); i++ ) {
        if ( rightDegreeList.at(i).id > avgDegree )
            hotRight.push_back( rightDegreeList.at(i) );
        else
            coldRight.push_back( rightDegreeList.at(i) );
    } // for

    sort( hotLeft.begin(), hotLeft.end(), greaterThan );
    sort( hotRight.begin(), hotRight.end(), greaterThan );

    vector<Node> newLeft, newRight;
    newLeft.insert( hotLeft.end(), hotLeft.begin(), coldLeft.end() );
    newRight.insert( hotRight.end(), coldRight.begin(), coldRight.end() );

    vector<int> invertedNewLeft = findIndex( newLeft );
    vector<int> invertedNewRight = findIndex( newRight );

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

    for ( int i = 0; i < edgeList.size(); i++ ) {
        leftDegreeList.at(edgeList.at(i).src.id).id = edgeList.at(i).src.id;
        leftDegreeList.at(edgeList.at(i).src.id).degree++;
        rightDegreeList.at(edgeList.at(i).dst.id-leftSize).id = edgeList.at(i).dst.id-leftSize;
        rightDegreeList.at(edgeList.at(i).dst.id-leftSize).degree++;
    } // for

    int avgDegree = edgeList.size() / ( leftSize+rightSize ) * 2;

    vector<Node> hotLeft;
    vector<Node> coldLeft;
    for ( int i = 0; i < leftDegreeList.size(); i++ ) {
        if ( leftDegreeList.at(i).id > avgDegree )
            hotLeft.push_back( leftDegreeList.at(i) );
        else
            coldLeft.push_back( leftDegreeList.at(i) );
    } // for

    vector<Node> hotRight;
    vector<Node> coldRight;
    for ( int i = 0; i < rightDegreeList.size(); i++ ) {
        if ( rightDegreeList.at(i).id > avgDegree )
            hotRight.push_back( rightDegreeList.at(i) );
        else
            coldRight.push_back( rightDegreeList.at(i) );
    } // for

    vector<Node> newLeft, newRight;
    newLeft.insert( hotLeft.end(), hotLeft.begin(), coldLeft.end() );
    newRight.insert( hotRight.end(), coldRight.begin(), coldRight.end() );

    vector<int> invertedNewLeft = findIndex( newLeft );
    vector<int> invertedNewRight = findIndex( newRight );

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

    vector<Node> newLeft = avgNeighborIDLeftList;
    sort( newLeft.begin(), newLeft.end(), lessThan );
    vector<int> invertedNewLeft = findIndex( newLeft );

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

    vector<Node> newRight = avgNeighborIDRightList;
    sort( newRight.begin(), newRight.end(), lessThan );
    vector<int> invertedNewRight = findIndex( newRight );

    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).dst.id - leftSize;
        edgeList.at(i).dst.id = invertedNewRight.at(originID) + leftSize;
    } // for
} // myOrder