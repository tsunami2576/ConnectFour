#include "Node.h"

Node::Node(Node *_parent, int _M, int _N, int **_board, const int *_top, int _lastX, int _lastY, int _noX, int _noY, bool _last_fall) : wins(0), visits(0), parent(_parent), board(_board, _top, _M, _N, _lastX, _lastY, _noX, _noY, _last_fall)
{
}

Node::~Node()
{
    for (Node *child : children)
        delete child;
}

void Node::initExpandSet()
{
    for (auto action : board.legal_action)
        not_expanded.emplace_back(action);
}
