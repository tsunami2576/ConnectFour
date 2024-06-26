#pragma once
#include "Node.h"

class CFAI
{
private:
    static double uctValue(int totle_visits, int visits, int wins, bool role, double expPara = 1);
    Node *selectChild(Node *node);
    Node *expand(Node *node);
    int simulate(Board board);
    void backpropagate(Node *node, int winner);

public:
    int think(int _M, int _N, int **_board, const int *_top, int _lastX, int _lastY, int _noX, int _noY, double time_limit = 2100);
    CFAI();
};
