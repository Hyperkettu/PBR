//
//  astar.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 10/17/18.
//  Copyright © 2018 Olli Kettunen. All rights reserved.
//

#include "astar.hpp"

std::vector<GridNode*> pathTo(GridNode* node) {
    GridNode* curr = node;
    std::vector<GridNode*> path;
    while (curr->mpParent) {
        path.insert(path.begin(), curr);
        curr = curr->mpParent;
    }
    return path;
}

BinaryHeap* getHeap() {
    return new BinaryHeap([](GridNode& node) {
        return node.mF;
    });
}


std::vector<GridNode*> AStar::search(Graph& graph, GridNode* start, GridNode* end, bool closest, float (*heuristic)(GridNode& node0, GridNode& node1)) {
    graph.cleanDirty();
    BinaryHeap* openHeap = getHeap();
    GridNode* closestNode = start; // set the start node to be the closest if required
    
    start->mH = heuristic(*start, *end);
    graph.markDirty(start);
    
    openHeap->push(start);
    
    while (openHeap->size() > 0) {
        
        // Grab the lowest f(x) to process next.  Heap keeps this sorted for us.
        auto currentNode = openHeap->pop();
        
        // End case -- result has been found, return the traced path.
        if (currentNode == end) {
            return pathTo(currentNode);
        }
        
        // Normal case -- move currentNode from open to closed, process each of its neighbors.
        currentNode->mbClosed = true;
        
        // Find all neighbors for the current node.
        auto neighbors = graph.neighbors(*currentNode);
        
        for (int i = 0, il = neighbors.size(); i < il; ++i) {
            auto neighbor = neighbors[i];
            
            if (neighbor->mbClosed || neighbor->isWall()) {
                // Not a valid node to process, skip to next neighbor.
                continue;
            }
            
            // The g score is the shortest distance from start to current node.
            // We need to check if the path we have arrived at this neighbor is the shortest one we have seen yet.
            auto gScore = currentNode->mG + neighbor->getCost(*currentNode);
            auto beenVisited = neighbor->mbVisited;
            
            if (!beenVisited || gScore < neighbor->mG) {
                
                // Found an optimal (so far) path to this node.  Take score for node to see how good it is.
                neighbor->mbVisited = true;
                neighbor->mpParent = currentNode;
                neighbor->mH = neighbor->mH || heuristic(*neighbor, *end);
                neighbor->mG = gScore;
                neighbor->mF = neighbor->mG + neighbor->mH;
                graph.markDirty(neighbor);
                if (closest) {
                    // If the neighbour is closer than the current closestNode or if it's equally close but has
                    // a cheaper path than the current closest node then it becomes the closest node
                    if (neighbor->mH < closestNode->mH || (neighbor->mH == closestNode->mH && neighbor->mG < closestNode->mG)) {
                        closestNode = neighbor;
                    }
                }
                
                if (!beenVisited) {
                    // Pushing to heap will put it in proper place based on the 'f' value.
                    openHeap->push(neighbor);
                } else {
                    // Already seen the node, but since it has been rescored we need to reorder it in the heap
                    openHeap->rescoreElement(neighbor);
                }
            }
        }
    }
    
    if (closest) {
        return pathTo(closestNode);
    }
    
    // No result was found - empty array signifies failure to find path.
    return std::vector<GridNode*>();
}
