#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include "Board.h"

class Node
{
public:
    int wins;
    int visits;
    Board board;
    std::vector<Node *> children;
    Node *parent;
    // status: 1 for playerWin, 2 for machineWin, 3 for tie, 0 for unfinished.

    Node(Node *_parent, int _M, int _N, int **_board, const int *_top, int _lastX, int _lastY, int _noX, int _noY, bool _last_fall);
    // _last_fall: 0 for player, 1 for machine.
    ~Node();
};

#endif
