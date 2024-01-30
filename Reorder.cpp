#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <numeric>
#include "Reorder.h"

using namespace std;

// 把值和index互換
vector<int> findIndex( vector<Node> originList ) {
    vector<int> invertedList( originList.size(), 0 );
    for ( int i = 0; i < originList.size(); i++ )
        invertedList.at(originList.at(i).id) = i;

    return invertedList;
} // findIndex

// 把值和index互換
vector<int> findIndexI( vector<int> originList ) {
    vector<int> invertedList( originList.size(), 0 );
    for ( int i = 0; i < originList.size(); i++ )
        invertedList.at(originList.at(i)) = i;

    return invertedList;
} // findIndex

vector<Edge> separateDegreeOrder( vector<Edge> edgeList, int leftSize, int rightSize ) {
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

    sort(leftDegreeList.begin(), leftDegreeList.end(), [](const Node &i, const Node &j) {
        return i.degree > j.degree;
    });

    sort(rightDegreeList.begin(), rightDegreeList.end(), [](const Node &i, const Node &j) {
        return i.degree > j.degree;
    });

    vector<int> invertedNewLeft = findIndex( leftDegreeList );
    vector<int> invertedNewRight = findIndex( rightDegreeList );

    int originID = 0;
    for ( int i = 0; i < edgeList.size(); i++ ) {
        originID = edgeList.at(i).src.id;
        edgeList.at(i).src.id = invertedNewLeft.at(originID);
        originID = edgeList.at(i).dst.id - leftSize;
        edgeList.at(i).dst.id = invertedNewRight.at(originID) + leftSize;
    } // for

    return edgeList;
} // separateDegreeOrder

vector<Edge> hubSort( vector<Edge> edgeList, int leftSize, int rightSize ) {

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

    sort( hotLeft.begin(), hotLeft.end(), [](const Node &i, const Node &j) {
        return i.degree > j.degree;
    });

    sort( hotRight.begin(), hotRight.end(), [](const Node &i, const Node &j) {
        return i.degree > j.degree;
    });

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

    return edgeList;
} // hubSort

vector<Edge> hubCluster( vector<Edge> edgeList, int leftSize, int rightSize ) {
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

    return edgeList;
} // hubCluster

vector<Edge> randomOrder( vector<Edge> edgeList, int leftSize, int rightSize ) {
    vector<int> l( leftSize );
    vector<int> r( rightSize );

    // init
    iota( l.begin(), l.end(), 0 );
    iota( r.begin(), r.end(), 0 );

    random_device rd;
    mt19937 g(rd());

    shuffle( l.begin(), l.end(), g );
    shuffle( r.begin(), r.end(), g );

    for ( Edge & edge : edgeList ) {
        edge.src.id = l.at(edge.src.id);
        edge.dst.id = r.at(edge.dst.id-leftSize)+leftSize;
    } // for

    return edgeList;
} // randomOrder

vector<Edge> myOrderLeft( vector<Edge> edgeList, int leftSize, int rightSize ) {
    Node node{ 0, leftSide, 0, 0 };
    vector<Node> nodeList( leftSize, node );

    // Initialization
    int count = 0;
    for ( const Edge & edge : edgeList ) {
        int srcIdx = edge.src.id;
        nodeList.at(srcIdx).id = srcIdx;
        //if (count % 1 == 0) {
            nodeList.at(srcIdx).degree++;
            nodeList.at(srcIdx).avgNeighborID += float(edge.dst.id);
        //} // if

        count++;
    } // for

    // Calculate average neighbor ID
    for_each(nodeList.begin(), nodeList.end(), [](Node &node) {
        if ( node.degree != 0 )
            node.avgNeighborID /= node.degree;
    });

    // Sort nodeList
    sort(nodeList.begin(), nodeList.end(), [](const Node &a, const Node &b) {
        return a.avgNeighborID < b.avgNeighborID;
    });

    // Find index mapping
    vector<int> invertedList = findIndex( nodeList );

    // Update edgeList with new indices
    for ( Edge & edge : edgeList)
        edge.src.id = invertedList.at(edge.src.id);

    return edgeList;
} // myOrderLeft

vector<Edge> myOrderRight( vector<Edge> edgeList, int leftSize, int rightSize ) {
    Node node{ 0, rightSide, 0, 0 };
    vector<Node> nodeList( rightSize, node );

    // Initialization
    int count = 0;
    for ( const Edge & edge : edgeList ) {
        int dstIdx = edge.dst.id - leftSize;
        nodeList.at(dstIdx).id = dstIdx;
        //if (count % 1 == 0) {
            nodeList.at(dstIdx).degree++;
            nodeList.at(dstIdx).avgNeighborID += float(edge.src.id);
        //} // if

        count++;
    } // for

    // Calculate average neighbor ID
    for_each(nodeList.begin(), nodeList.end(), [](Node &node) {
        if ( node.degree != 0 )
            node.avgNeighborID /= node.degree;
    });

    // Sort nodeList
    sort(nodeList.begin(), nodeList.end(), [](const Node &a, const Node &b) {
        return a.avgNeighborID < b.avgNeighborID;
    });

    // Find index mapping
    vector<int> invertedList = findIndex( nodeList );

    // Update edgeList with new indices
    for ( Edge & edge : edgeList)
        edge.dst.id = invertedList.at(edge.dst.id - leftSize) + leftSize;

    return edgeList;
} // myOrderRight

vector<Edge> mix64k( vector<Edge> edgeList, int leftSize, int rightSize ) {
    vector<int> newOrderL, newOrderR;
    
    bool turnL = ( leftSize <= rightSize );
    bool turnR = !turnL;

    for ( int newID = 0; newID < leftSize + rightSize; newID++ ) {
        if ( newID % 64000 == 0 ) {
            turnL = !turnL;
            turnR = !turnR;
        } // if

        if ( turnL )
            newOrderL.push_back(newID);
        else
            newOrderR.push_back(newID);
    } // for

    // Update edgeList with new indices
    for ( Edge & edge : edgeList ) {
        if ( edge.src.id < leftSize )
            edge.src.id = newOrderL.at(edge.src.id);
        else
            edge.src.id = newOrderR.at(edge.src.id - leftSize);

        if ( edge.dst.id < leftSize )
            edge.dst.id = newOrderL.at(edge.dst.id);
        else
            edge.dst.id = newOrderR.at(edge.dst.id - leftSize);
    } // for

    return edgeList;
} // mix64k

vector<Edge> myOrder( vector<Edge> edgeList, int leftSize, int rightSize ) {
    if ( leftSize > rightSize )
        edgeList = myOrderLeft( edgeList, leftSize, rightSize );
    else
        edgeList = myOrderRight( edgeList, leftSize, rightSize );

    return edgeList;
} // myOrder

vector<Edge> NCOrder( vector<Edge> edgeList, int leftSize, int rightSize ){
    if ( leftSize <= rightSize ) {
        Node node{ 0, leftSide, 0, 0 };
        vector<Node> arr( leftSize, node );

        // init
        int srcID = 0, dstID = 0;
        for ( int i = 0; i < edgeList.size(); i++ ) {
            srcID = edgeList.at(i).src.id;
            arr.at(srcID).id = srcID;
            arr.at(srcID).degree++;
        } // for
        
        sort(arr.begin(), arr.end(), [](const Node &i, const Node &j) {
            return i.degree > j.degree;
        });

        // arr中，value是原ID，index是新ID

        vector<int> invertedArr = findIndex( arr );
        int originID = 0;
        for ( int i = 0; i < edgeList.size(); i++ ) {
            originID = edgeList.at(i).src.id;
            edgeList.at(i).src.id = invertedArr.at(originID);
        } // for
        
        vector<int> OA, EA;

        vector<Edge> temp = edgeList;
        edgeListForUnDIR( temp );
        convertToCSR( temp, leftSize+rightSize, OA, EA );

        vector<int> rN;
        vector<bool> isChosenList(rightSize, false);

        for ( int i = 0; i < leftSize; i++ ) {
            int sIdx = OA.at(i);
            int eIdx = OA.at(i+1)-1;
            while ( sIdx <= eIdx ) {
                int nID = EA.at(sIdx) - leftSize;
                if ( !isChosenList.at(nID) ) {
                    rN.push_back( nID );
                    isChosenList.at(nID) = true;
                } // if
                    
                sIdx++;
            } // while
        } // for

        if ( rN.size() != rightSize ) {
            for ( int i = 0; i < isChosenList.size(); i++ ) {
                if ( !isChosenList.at(i) )
                    rN.push_back(i);
            } // for
        } // if

        vector<int> invertedList = findIndexI( rN );

        // Update edgeList with new indices
        for ( Edge & edge : edgeList )
            edge.dst.id = invertedList.at(edge.dst.id - leftSize) + leftSize;

    } // if
    else {
        Node node{ 0, rightSide, 0, 0 };
        vector<Node> arr( rightSize, node );

        // init
        int dstID = 0;
        for ( int i = 0; i < edgeList.size(); i++ ) {
            dstID = edgeList.at(i).dst.id - leftSize;
            arr.at(dstID).id = dstID;
            arr.at(dstID).degree++;
        } // for
        
        sort(arr.begin(), arr.end(), [](const Node &i, const Node &j) {
            return i.degree > j.degree;
        });

        // arr中，value是原ID，index是新ID

        vector<int> invertedArr = findIndex( arr );
        int originID = 0;
        for ( int i = 0; i < edgeList.size(); i++ ) {
            originID = edgeList.at(i).dst.id;
            edgeList.at(i).dst.id = invertedArr.at(originID-leftSize)+leftSize;
        } // for

        vector<int> lN;
        vector<bool> isChosenList(leftSize, false);
        vector<int> OA, EA;

        vector<Edge> temp = edgeList;
        edgeListForUnDIR( temp );
        convertToCSR( temp, leftSize+rightSize, OA, EA );

        for ( int i = leftSize; i < OA.size()-2; i++ ) {
            int sIdx = OA.at(i);
            int eIdx = OA.at(i+1)-1;
            while ( sIdx <= eIdx ) {
                int nID = EA.at(sIdx);
                if ( !isChosenList.at(nID) ) {
                    lN.push_back( nID );
                    isChosenList.at(nID) = true;
                } // if
                    
                sIdx++;
            } // while
        } // for

        if ( lN.size() != leftSize ) {
            for ( int i = 0; i < isChosenList.size(); i++ ) {
                if ( !isChosenList.at(i) )
                    lN.push_back(i);
                    
            } // for
        } // if

        vector<int> invertedList = findIndexI( lN );

        // Update edgeList with new indices
        for ( Edge & edge : edgeList )
            edge.src.id = invertedList.at(edge.src.id);
        
    } // else

    return edgeList;
} // NCOrder