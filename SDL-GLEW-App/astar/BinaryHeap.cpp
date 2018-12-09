//
//  BinaryHeap.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 10/17/18.
//  Copyright © 2018 Olli Kettunen. All rights reserved.
//

#include "BinaryHeap.hpp"
#include <algorithm>

BinaryHeap::BinaryHeap(float (*scoreFunction) (GridNode& node)) : mContent(), mScoreFunction(scoreFunction) {
}

GridNode* BinaryHeap::pop() {
    // Store the first element so we can return it later.
    GridNode* result = mContent[0];
    // Get the element at the end of the array.
    GridNode* end = mContent.back();
    mContent.pop_back();
    // If there are any elements left, put the end element at the
    // start, and let it bubble up.
    if (mContent.size() > 0) {
        mContent[0] = end;
        bubbleUp(0);
    }
    return result;
}

void BinaryHeap::remove(GridNode* node) {
    auto iter = std::find(mContent.begin(), mContent.end(), node);
    auto i = std::distance(mContent.begin(), iter);
    // When it is found, the process seen in 'pop' is repeated
    // to fill up the hole.
    GridNode* end = mContent.back();
    mContent.pop_back();
    
    if (i != mContent.size() - 1) {
        mContent[i] = end;

        if (mScoreFunction(*end) < mScoreFunction(*node)) {
            sinkDown(i);
        } else {
            bubbleUp((int)i);
        }
    }
}

void BinaryHeap::sinkDown(int n) {
    // Fetch the element that has to be sunk.
    auto element = mContent[n];
    
    // When at 0, an element can not sink any further.
    while (n > 0) {
        
        // Compute the parent element's index, and fetch it.
        int parentN = ((n + 1) >> 1) - 1;
        auto parent = mContent[parentN];
        // Swap the elements if the parent is greater.
        if (mScoreFunction(*element) < mScoreFunction(*parent)) {
            mContent[parentN] = element;
            mContent[n] = parent;
            // Update 'n' to continue at the new position.
            n = parentN;
        }
        // Found a parent that is less, no need to sink any further.
        else {
            break;
        }
    }
}

void BinaryHeap::bubbleUp(int n) {
    // Look up the target element and its score.
    auto length = mContent.size();
    auto element = mContent[n];
    auto elemScore = mScoreFunction(*element);
    
    while (true) {
        // Compute the indices of the child elements.
        auto child2N = (n + 1) << 1;
        auto child1N = child2N - 1;
        // This is used to store the new position of the element, if any.
        int swap = -1;
        float child1Score;
        // If the first child exists (is inside the array)...
        if (child1N < length) {
            // Look it up and compute its score.
            auto child1 = mContent[child1N];
            child1Score = mScoreFunction(*child1);
            
            // If the score is less than our element's, we need to swap.
            if (child1Score < elemScore) {
                swap = child1N;
            }
        }
        
        // Do the same checks for the other child.
        if (child2N < length) {
            auto child2 = mContent[child2N];
            auto child2Score = mScoreFunction(*child2);
            if (child2Score < (swap == -1 ? elemScore : child1Score)) {
                swap = child2N;
            }
        }
        
        // If the element needs to be moved, swap it, and continue.
        if (swap != -1) {
            mContent[n] = mContent[swap];
            mContent[swap] = element;
            n = swap;
        }
        // Otherwise, we are done.
        else {
            break;
        }
    }
}
