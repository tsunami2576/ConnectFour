#include <vector>

class Board
{
public:
    int **board;
    int *top;
    int M;
    int N;
    int lastX;
    int lastY;
    int noX;
    int noY;
    bool last_fall; // 0 for player, 1 for machine.
    int status;
    std::vector<int> legal_action;

    Board(int **_board, const int *_top, int _M, int _N, int _lastX, int _lastY, int _noX, int _noY, bool _last_fall);
    ~Board();
    bool terminated();
    void legalAction();
    void actionApply(int y);
};
