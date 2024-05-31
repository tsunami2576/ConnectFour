#include "Board.h"
#include "Judge.h"
#include <random>

Board::Board(int **_board, const int *_top, int _M, int _N, int _lastX, int _lastY, int _noX, int _noY, bool _last_fall) : M(_M), N(_N), lastX(_lastX), lastY(_lastY), noX(_noX), noY(_noY), last_fall(_last_fall)
{
    board = new int *[M];
    top = new int[N];
    for (int i = 0; i < M; i++)
    {
        board[i] = new int[N];
        for (int j = 0; j < N; j++)
            board[i][j] = _board[i][j];
    }

    for (int i = 0; i < N; i++)
        top[i] = _top[i];
}

Board::~Board()
{
    for (int i = 0; i < M; i++)
    {
        delete[] board[i];
    }
    delete[] board;
    delete[] top;
}

Board::Board(const Board &other) : M(other.M), N(other.N), lastX(other.lastX), lastY(other.lastY), noX(other.noX), noY(other.noY), last_fall(other.last_fall), status(other.status), legal_action(other.legal_action)
{
    board = new int *[M];
    top = new int[N];
    for (int i = 0; i < M; i++)
    {
        board[i] = new int[N];
        for (int j = 0; j < N; j++)
            board[i][j] = other.board[i][j];
    }

    for (int i = 0; i < N; i++)
        top[i] = other.top[i];
}

bool Board::terminated()
{
    if (0 == status)
        return false;
    else
        return true;
}

void Board::legalAction()
{
    for (int i = 0; i < N; i++)
    {
        if (top[i] != 0)
            legal_action.emplace_back(i);
    }
}

void Board::actionApply(int y)
{
    top[y]--;

    board[top[y]][y] = (last_fall ? 2 : 1);
    lastX = top[y];
    lastY = y;
    last_fall = !last_fall;

    if (0 == last_fall && userWin(lastX, lastY, M, N, board))
        status = 1;
    else if (1 == last_fall && machineWin(lastX, lastY, M, N, board))
        status = 2;
    else if (isTie(N, top))
        status = 3;
    else
        status = 0;

    if (top[y] != 0 && y == noY && (top[y] - 1) == noX)
        top[y]--;
}
