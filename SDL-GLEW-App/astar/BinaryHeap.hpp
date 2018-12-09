//
//  BinaryHeap.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 10/17/18.
//  Copyright © 2018 Olli Kettunen. All rights reserved.
//

#ifndef BinaryHeap_hpp
#define BinaryHeap_hpp

#include <vector>

#include "GridNode.hpp"


class BinaryHeap {
public:

    std::vector<GridNode*> mContent;
    float (*mScoreFunction)(GridNode& node);

    BinaryHeap(){}
    BinaryHeap(float (*scoreFunction) (GridNode& node));
    void push(GridNode* element) {
        // Add the new element to the end of the array.
        mContent.push_back(element);
        // Allow it to sink down.
        sinkDown(mContent.size() - 1);
    }
    
    GridNode* pop();
    void remove(GridNode* node);
    
    size_t size() {
        return mContent.size();
    }
    
    void rescoreElement(GridNode* node) {
        auto iter = std::find(mContent.begin(), mContent.end(), node);
        auto i = std::distance(mContent.begin(), iter);
        sinkDown((int)i);
    }
    
    void sinkDown(int n);
    void bubbleUp(int n);
    

};

#endif /* BinaryHeap_hpp */
