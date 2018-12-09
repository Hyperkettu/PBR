//
//  astar.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 10/17/18.
//  Copyright © 2018 Olli Kettunen. All rights reserved.
//

#ifndef astar_hpp
#define astar_hpp

#include "Graph.hpp"
#include "GridNode.hpp"
#include "BinaryHeap.hpp"

#include <cstdlib>
#include <vector>
#include <cmath>

std::vector<GridNode*> pathTo(GridNode* node);

BinaryHeap* getHeap();

class AStar {
public:
    AStar() {}
    
    /**
     * Perform an A* Search on a graph given a start and end node.
     * @param graph
     * @param start
     * @param end
     * @param [options.closest] Specifies whether to return the
     path to the closest node if the target is unreachable.
     * @param  [options.heuristic] Heuristic function (see
     *          astar.heuristics).
     */
    std::vector<GridNode*> search(Graph& graph, GridNode* start, GridNode* end, bool closest, float (*heuristic)(GridNode& node0, GridNode& node1));
    
    // See list of heuristics: http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
    float manhattan(GridNode& pos0, GridNode& pos1) {
        float d1 = abs(pos1.mX - pos0.mX);
        float d2 = abs(pos1.mY - pos0.mY);
        return d1 + d2;
    }
    
    float diagonal(GridNode& pos0, GridNode& pos1) {
        float D = 1;
        float D2 = std::sqrt(2);
        float d1 = std::abs(pos1.mX - pos0.mX);
        float d2 = std::abs(pos1.mY - pos0.mY);
        return (D * (d1 + d2)) + ((D2 - (2 * D)) * std::min(d1, d2));
    }
};


#endif /* astar_hpp */
