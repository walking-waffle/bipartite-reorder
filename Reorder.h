#ifndef _REORDER_H
#define _REORDER_H
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include "Graph.h"

using namespace std;

vector<Edge> separateDegreeOrder( vector<Edge> edgeList, int leftSize, int rightSize );

vector<Edge> randomOrder( vector<Edge> edgeList, int leftSize, int rightSize );

vector<Edge> hubSort( vector<Edge> edgeList, int leftSize, int rightSize );

vector<Edge> hubCluster( vector<Edge> edgeList, int leftSize, int rightSize );

vector<Edge> myOrderLeft( vector<Edge> edgeList, int leftSize, int rightSize );

vector<Edge> myOrderRight( vector<Edge> edgeList, int leftSize, int rightSize );

vector<Edge> myOrder( vector<Edge> edgeList, int leftSize, int rightSize );

vector<Edge> NCOrder( vector<Edge> edgeList, int leftSize, int rightSize );

#endif
