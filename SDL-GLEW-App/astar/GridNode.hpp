//
//  GridNode.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 10/17/18.
//  Copyright © 2018 Olli Kettunen. All rights reserved.
//

#ifndef GridNode_hpp
#define GridNode_hpp

#include <string>
#include <sstream>

class GridNode {
public:
    
    GridNode(){}
    
    GridNode(int x, int y, int weight): mX(x), mY(y), mWeight(weight) {}
    
    GridNode& operator= (const GridNode& other) {
        
        if(this == &other){
            return *this;
        }
        mWeight = other.mWeight;
        mX = other.mX;
        mY = other.mY;
        mF = other.mF;
        mG = other.mG;
        mH = other.mH;
        mbVisited = other.mbVisited;
        mbClosed = other.mbClosed;
        mpParent = other.mpParent;
        
        return *this;
    }
    
    std::string toString() {
        std::ostringstream oss;
        oss << '[' << mX << ' ' << mY << ']';
        return oss.str();
    }
    
    float getCost(GridNode& fromNeighbor) {
        // Take diagonal weight into consideration.
         if(fromNeighbor.mX != mX && fromNeighbor.mY != mX) {
            return mWeight * 1.41421;
        }
        return mWeight;
    }
    
    bool isWall() {
        return mWeight == 0;
    }
    
    int mWeight;
    int mX;
    int mY;
    
    float mF;
    float mG;
    float mH;
    
    bool mbVisited;
    bool mbClosed;
    GridNode* mpParent;
    
};


#endif /* GridNode_hpp */
