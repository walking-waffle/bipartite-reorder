#ifndef _REORDER_H
#define _REORDER_H
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include "Graph.h"

using namespace std;

void jointDegreeOrder( vector<Edge> & edgeList, int leftSize, int rightSize );

void separateDegreeOrder( vector<Edge> & edgeList, int leftSize, int rightSize );

void randomOrder( vector<Edge> & edgeList, int numOfNodes );

void hubSort( vector<Edge> & edgeList, int leftSize, int rightSize );

void hubCluster( vector<Edge> & edgeList, int leftSize, int rightSize );

void myOrderLeft( vector<Edge> & edgeList, int leftSize, int rightSize );

void myOrderRight( vector<Edge> & edgeList, int leftSize, int rightSize );

void myOrderBalance( vector<Edge> & edgeList, int leftSize, int rightSize );

#endif
