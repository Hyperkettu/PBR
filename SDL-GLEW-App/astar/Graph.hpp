//
//  Graph.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 10/17/18.
//  Copyright © 2018 Olli Kettunen. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <string>
#include <vector>

#include "GridNode.hpp"

class Graph {
public:
    /**
     * A graph memory structure
     * @param {Array} gridIn 2D array of input weights
     * @param {Object} [options]
     * @param {bool} [options.diagonal] Specifies whether diagonal moves are allowed
     */
    Graph(int** gridIn, int w, int h, bool diagonal); 
    
    void init();
    void cleanDirty();
    
    void markDirty(GridNode* node) {
        mDirtyNodes.push_back(node);
    }
    
    static void cleanNode(GridNode* node) {
        node->mF = 0;
        node->mG = 0;
        node->mH = 0;
        node->mbVisited = false;
        node->mbClosed = false;
        node->mpParent = nullptr;
    }
    
    std::vector<GridNode*> neighbors(GridNode& node);
    std::string toString();
    
    bool mbDiagonal;
    GridNode*** mGrid;
    int width, height;
    std::vector<GridNode*> mDirtyNodes;
    std::vector<GridNode*> mNodes;
};

#endif /* Graph_hpp */
