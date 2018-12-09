//
//  Graph.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 10/17/18.
//  Copyright © 2018 Olli Kettunen. All rights reserved.
//

#include <sstream>
#include <iostream>

#include "Graph.hpp"

Graph::Graph(int** gridIn, int w, int h, bool diagonal) {
    mNodes = std::vector<GridNode*>();
    mbDiagonal = diagonal;
    int rows = w; //sizeof (gridIn) / sizeof (gridIn[0]);
    int columns = h; //sizeof (gridIn[0]) / sizeof(gridIn[0][0]);
    width = w;
    height = h;
    
    mGrid = new GridNode**[rows];
    
    for (int x = 0; x < rows; x++) {
        mGrid[x] = new GridNode*[columns];
        
        for (int y = 0; y < columns; y++) {
            GridNode* node = new GridNode(x, y, gridIn[x][y]);
            mGrid[x][y] = node;
            mNodes.push_back(node);
        }
    }
    init();
}

void Graph::init() {
    mDirtyNodes = std::vector<GridNode*>();
    for (int i = 0; i < mNodes.size(); i++) {
        Graph::cleanNode(mNodes[i]);
    }
}

void Graph::cleanDirty() {
    for (int i = 0; i < mDirtyNodes.size(); i++) {
        Graph::cleanNode(mDirtyNodes[i]);
    }
    mDirtyNodes.clear();
}

std::vector<GridNode*> Graph::neighbors(GridNode& node) {
    std::vector<GridNode*> ret;
    int x = node.mX;
    int y = node.mY;
    GridNode*** grid = mGrid;
    
    int rows = width; //sizeof grid / sizeof grid[0];
    int columns = height; //sizeof grid[0] / sizeof(GridNode);
    
    // West
    if (x - 1 >= 0 && x - 1 < rows && y >= 0 && y < columns) {
        ret.push_back(grid[x - 1][y]);
    }
    
    // East
    if (x + 1 >= 0 && x + 1 < rows && y >= 0 && y < columns) {
        ret.push_back(grid[x + 1][y]);
    }
    
    // South
    if (x >= 0 && x < rows && y - 1 >= 0 && y - 1 < columns) {
        ret.push_back(grid[x][y - 1]);
    }
    
    // North
    if (x >= 0 && x < rows && y + 1 >= 0 && y + 1 < columns) {
        ret.push_back(grid[x][y + 1]);
    }
    
    if (mbDiagonal) {
        // Southwest
        if (x - 1 >= 0 && x - 1 < rows && y - 1 >= 0 && y - 1 < columns) {
            ret.push_back(grid[x - 1][y - 1]);
        }
        
        // Southeast
        if (x + 1 >= 0 && x + 1 < rows && y - 1 >= 0 && y - 1 < columns) {
            ret.push_back(grid[x + 1][y - 1]);
        }
        
        // Northwest
        if (x - 1 >= 0 && x - 1 < rows && y + 1 >= 0 && y + 1 < columns) {
            ret.push_back(grid[x - 1][y + 1]);
        }
        
        // Northeast
        if (x + 1 >= 0 && x + 1 < rows && y + 1 >= 0 && y + 1 < columns) {
            ret.push_back(grid[x + 1][y + 1]);
        }
    }
    
    return ret;
}

std::string Graph::toString() {
    std::stringstream ss;
    int rows = width; //sizeof mGrid / sizeof mGrid[0];
    int columns = height; //sizeof mGrid[0] / sizeof(GridNode);
    for (int y = 0; y < columns; y++) {
        for (int x = 0; x < rows; x++) {
            ss << mGrid[x][y]->mWeight << " ";
        }
        ss << '\n';
    }
    
    return ss.str();
}
