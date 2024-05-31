#include "CFAI.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <chrono>
#include <iostream>

CFAI::CFAI()
{
    std::cerr << "AI constructed.\n";
    srand(time(0));
}

double CFAI::uctValue(int totle_visits, int visits, int wins, double expPara)
{
    if (visits == 0)
        return std::numeric_limits<double>::max();
    return (double(wins) / visits) + expPara * sqrt(log(totle_visits) / visits);
}

Node *CFAI::selectChild(Node *node)
{
    int visits = node->visits;
    return *(std::max_element(node->children.begin(), node->children.end(), [&visits](Node *a, Node *b)
                              { return uctValue(visits, a->visits, a->wins) < uctValue(visits, b->visits, b->wins); }));
}

Node *CFAI::expand(Node *node)
{
    if (node->board.terminated())
        return node;
    for (int y : node->board.legal_action)
    {
        Node *childNode = new Node(node, node->board.M, node->board.N, node->board.board, node->board.top, node->board.lastX,
                                   node->board.lastY, node->board.noX, node->board.noY, node->board.last_fall);
        childNode->board.actionApply(y);
        childNode->board.legalAction();
        node->children.emplace_back(childNode);
        if (node->children.size() >= node->board.legal_action.size())
            break;
    }
    return node->children.back();
}

int CFAI::simulate(Board board)
{
    while (!board.terminated())
    {
        auto action = board.legal_action.begin() + random() % board.legal_action.size();
        board.actionApply(*action);
        if (0 == board.top[*action] || (*action == board.noY && 1 == board.top[*action] && 0 == board.noX))
            board.legal_action.erase(action);
    }
    return board.status;
}

void CFAI::backpropagate(Node *node, int winner)
{
    while (node != nullptr)
    {
        node->visits++;
        if (int(node->board.last_fall) + 1 == winner)
        {
            node->wins++;
        }
        /* Plan B:
        node->visits+=2;
        if (int(node->board.last_fall) + 1 == winner)
        {
            node->wins+=2;
        }
        else if(3==winner)
            node->wins++;
        */
        node = node->parent;
    }
}

int CFAI::think(int _M, int _N, int **_board, const int *_top, int _lastX, int _lastY, int _noX, int _noY, double time_limit)
{
    std::cerr << "Think start.\n";
    auto start = std::chrono::system_clock::now();
    std::cerr << "Clock start.\n";
    Node *root = new Node(nullptr, _M, _N, _board, _top, _lastX, _lastY, _noX, _noY, 1);
    std::cerr << "Root 实例化.\n";
    expand(root);
    std::cerr << "Root expanded.\n";
    std::cerr << "Simulation start.\n";
    while ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)).count() < time_limit)
    {
        Node *selected = selectChild(root);
        std::cerr << "Child selected.\n";
        Node *expanded = expand(selected);
        std::cerr << "Child expanded.\n";
        int winner = simulate(expanded->board);
        std::cerr << "Simulation finished.\n";
        backpropagate(expanded, winner);
        std::cerr << "Backpropagate finished.\n";
    }
    Node *mostVisited = *(std::max_element(root->children.begin(), root->children.end(), [](Node *a, Node *b)
                                           { return a->visits < b->visits; }));
    int bestAction = mostVisited->board.lastY;
    delete root;
    return bestAction;
}
