#include <iostream>
#include <cstdlib>
#include <ctime>
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

void myOrderLeft( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    Node node;
    node.degree = 0;
    node.avgNeighborID = 0;
    vector<Node> avgNeighborIDList( leftSize, node );

    // init
    int srcIdx = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        srcIdx = edgeList.at(i).src.id;
        avgNeighborIDList.at(srcIdx).id = srcIdx;
        avgNeighborIDList.at(srcIdx).degree++;
        avgNeighborIDList.at(srcIdx).avgNeighborID = avgNeighborIDList.at(srcIdx).avgNeighborID + float(edgeList.at(i).dst.id);
    } // for

    // 計算鄰居 ID 平均
    for ( int i = 0; i < avgNeighborIDList.size(); i++ )
        avgNeighborIDList.at(i).avgNeighborID = avgNeighborIDList.at(i).avgNeighborID/avgNeighborIDList.at(i).degree;

    sort( avgNeighborIDList.begin(), avgNeighborIDList.end(), lessThan );
    vector<int> invertedNewLeft = findIndex( avgNeighborIDList );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewLeft.at(originID);
    } // for

} // myOrderLeft

void myOrderRight( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    Node node;
    node.degree = 0;
    node.avgNeighborID = 0;
    vector<Node> avgNeighborIDList( rightSize, node );

    // init
    int dstIdx = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        dstIdx = edgeList.at(i).dst.id - leftSize;
        avgNeighborIDList.at(dstIdx).id = dstIdx;
        avgNeighborIDList.at(dstIdx).degree++;
        avgNeighborIDList.at(dstIdx).avgNeighborID = avgNeighborIDList.at(dstIdx).avgNeighborID + float(edgeList.at(i).src.id);
    } // for

    // 計算鄰居 ID 平均
    for ( int i = 0; i < avgNeighborIDList.size(); i++ )
        avgNeighborIDList.at(i).avgNeighborID = avgNeighborIDList.at(i).avgNeighborID/avgNeighborIDList.at(i).degree;

    sort( avgNeighborIDList.begin(), avgNeighborIDList.end(), lessThan );
    vector<int> invertedNewRight = findIndex( avgNeighborIDList );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).dst.id - leftSize;
        edgeList.at(i).dst.id = invertedNewRight.at(originID) + leftSize;
    } // for
} // myOrderRight

void mix( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    int hop = 0;
    int id = 0;
    int p = 0;

    if ( leftSize > rightSize ) {
        hop = leftSize / rightSize;
        for ( int i = 0; i < edgeList.size(); i++ ) {

            id = edgeList.at(i).src.id;
            if ( id < leftSize ) {
                p = id / hop;
                if ( p > rightSize )
                    id = id + rightSize;
                else
                    id = id + p;
            } // if
            else
                id = ( id+1-leftSize ) * ( hop+1 ) - 1;

            edgeList.at(i).src.id = id;

            id = edgeList.at(i).dst.id;
            if ( id < leftSize ) {
                p = id / hop;
                if ( p > rightSize )
                    id = id + rightSize;
                else
                    id = id + p;
            } // if
            else
                id = ( id+1-leftSize ) * ( hop+1 ) - 1;

            edgeList.at(i).dst.id = id;
        } // for
    } // if
    else {
        hop = rightSize / leftSize;
        for ( int i = 0; i < edgeList.size(); i++ ) {
            id = edgeList.at(i).src.id;
            if ( id < leftSize )
                id = ( id+1 ) * ( hop+1 ) - 1;
            else {
                id = id - leftSize;
                p = id / hop;
                if ( p > leftSize )
                    id = id + leftSize;
                else
                    id = id + p;
            } // else

            edgeList.at(i).src.id = id;

            id = edgeList.at(i).dst.id;
            if ( id < leftSize )
                id = ( id+1 ) * ( hop+1 ) - 1;
            else {
                id = id - leftSize;
                p = id / hop;
                if ( p > leftSize )
                    id = id + leftSize;
                else
                    id = id + p;
            } // else

            edgeList.at(i).dst.id = id;
        } // for
    } // else
    
} // mix

void myOrderBalance( vector<Edge> & edgeList, int leftSize, int rightSize ) {
    myOrderLeft( edgeList, leftSize, rightSize );
    myOrderRight( edgeList, leftSize, rightSize );
    mix( edgeList, leftSize, rightSize );
} // myOrderBalance