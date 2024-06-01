#include "CFAI.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <chrono>
#include <iostream>

CFAI::CFAI()
{
    srand(time(0));
}

double CFAI::uctValue(int totle_visits, int visits, int wins, double expPara)
{
    return (double(wins) / visits) + expPara * std::sqrt(std::log(double(totle_visits)) / visits);
}

Node *CFAI::selectChild(Node *node)
{
    double uct = 0.0, uct_curr;
    Node *selected;
    for (auto child : node->children)
    {
        uct_curr = uctValue(node->visits, child->visits, child->wins);
        if (uct_curr > uct)
        {
            uct = uct_curr;
            selected = child;
        }
    }
    return selected;
}

Node *CFAI::expand(Node *node)
{
    if (node->board.terminated())
        return node;
    if (!(node->not_expanded.empty()))
    {
        auto action = node->not_expanded.begin() + (random() % (node->not_expanded.size()));
        Node *childNode = new Node(node, node->board.M, node->board.N, node->board.board, node->board.top, node->board.lastX,
                                   node->board.lastY, node->board.noX, node->board.noY, node->board.last_fall);
        childNode->board.legalAction();
        childNode->board.actionApply(*action);
        childNode->initExpandSet();
        node->children.emplace_back(childNode);
        node->not_expanded.erase(action);
        if (node->parent == nullptr && childNode->board.status == 2)
            return nullptr;
        return childNode;
    }
    Node *childNode = selectChild(node);
    return expand(childNode);
}

int CFAI::simulate(Board board)
{
    while (!board.terminated())
    {
        auto action = board.legal_action.begin();
        // auto action = board.legal_action.begin() + random() % board.legal_action.size();
        board.actionApply(*action);
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
        // else if (int(node->board.last_fall) + 1 != winner && winner != 3)
        // {
        //     node->wins--;
        // }
        node = node->parent;
    }
}

int CFAI::think(int _M, int _N, int **_board, const int *_top, int _lastX, int _lastY, int _noX, int _noY, double time_limit)
{
    auto start = std::chrono::system_clock::now();
    Node *root = new Node(nullptr, _M, _N, _board, _top, _lastX, _lastY, _noX, _noY, 1);
    root->board.legalAction();
    root->initExpandSet();
    while ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)).count() < time_limit)
    {
        Node *expanded = expand(root);
        if (expanded == nullptr)
            continue;
        if (expanded->parent == root && expanded->board.status == 1)
        {
            int chosenY = expanded->board.lastY;
            delete root;
            return chosenY;
        }
        std::cerr << "1";
        int winner = simulate(expanded->board);
        std::cerr << "2";
        backpropagate(expanded, winner);
        std::cerr << "3";
    }
    double winrate = -1.0, winrate_curr;
    Node *chosen;
    for (auto child : root->children)
    {
        winrate_curr = double(child->wins) / child->visits;
        if (winrate_curr > winrate)
        {
            winrate = winrate_curr;
            chosen = child;
        }
    }
    int chosenY = chosen->board.lastY;
    delete root;
    return chosenY;
}
