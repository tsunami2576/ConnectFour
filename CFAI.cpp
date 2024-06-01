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

double CFAI::uctValue(int totle_visits, int visits, int wins, bool role, double expPara)
{
    std::cerr << double(wins) / visits << ' ' << expPara * std::sqrt(std::log(double(totle_visits)) / visits) << '\n';
    return (double(wins) / visits) * (role ? -1 : 1) + expPara * std::sqrt(std::log(double(totle_visits)) / visits);
}

Node *CFAI::selectChild(Node *node)
{
    double uct = 0.0, uct_curr;
    Node *selected;
    for (auto child : node->children)
    {
        uct_curr = uctValue(node->visits, child->visits, child->wins, child->board.last_fall);
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
        int rd = random() % node->not_expanded.size();
        auto action = node->not_expanded.begin() + rd;
        Node *childNode = new Node(node, node->board.M, node->board.N, node->board.board, node->board.top, node->board.lastX,
                                   node->board.lastY, node->board.noX, node->board.noY, node->board.last_fall);
        childNode->board.legalAction();
        childNode->board.actionApply(*action, rd);
        childNode->initExpandSet();
        node->children.emplace_back(childNode);
        node->not_expanded.erase(action);
        return childNode;
    }
    Node *childNode = selectChild(node);
    return expand(childNode);
}

int CFAI::simulate(Board board)
{
    while (!board.terminated())
    {
        int rd = random() % board.legal_action.size();
        auto action = board.legal_action.begin() + rd;
        // auto action = board.legal_action.begin() + random() % board.legal_action.size();
        board.actionApply(*action, rd);
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
        else if (int(node->board.last_fall) + 1 != winner && winner != 3)
        {
            node->wins--;
        }
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
        if (expanded->parent == root && expanded->board.status == 1)
        {
            int chosenY = expanded->board.lastY;
            delete root;
            return chosenY;
        }
        else if (expanded->parent != root && expanded->parent == root && expanded->board.status == 2)
        {
            int chosenY = expanded->board.lastY;
            delete root;
            return chosenY;
        }
        int winner = simulate(expanded->board);
        backpropagate(expanded, winner);
    }
    double winrate = -2.0, winrate_curr;
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
