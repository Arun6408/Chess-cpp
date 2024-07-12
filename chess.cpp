#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>
#include <tuple>
#include <cctype>

using namespace std;

tuple<bool, bool, bool> canWhiteCastle = {true, true, true}; // Queen'sRook king king'sRook
tuple<bool, bool, bool> canBlackCastle = {true, true, true}; // Queen'sRook king king'sRook
pair<int, int> lastDoubleStep = {-1, -1}; // for pawn enpassant to track weather the lastStep is pawn double movement or not

void displayBoard(vector<vector<char>> board)
{
    cout << "Board:\n\n";
    for (int i = 0; i < 8; ++i)
    {
        cout << 8 - i << "  ";
        for (int j = 0; j < 8; ++j)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "   ";
    for (char k = 'a'; k <= 'h'; ++k)
        cout << k << " ";
    cout << endl;
}
void prepareBoard(vector<vector<char>> &board)
{
    // Blacks will have small letters
    board[0][0] = board[0][7] = 'r'; // Rook
    board[0][1] = board[0][6] = 'n'; // Knight
    board[0][2] = board[0][5] = 'b'; // Bishop
    board[0][3] = 'q';               // Queen
    board[0][4] = 'k';               // King
    for (int i = 0; i < 8; ++i)
    {
        board[1][i] = 'p';
    }

    // Whites will have Capitals
    board[7][0] = board[7][7] = 'R'; // Rook
    board[7][1] = board[7][6] = 'N'; // Knight
    board[7][2] = board[7][5] = 'B'; // Bishop
    board[7][3] = 'Q';               // Queen
    board[7][4] = 'K';               // King
    for (int i = 0; i < 8; ++i)
    {
        board[6][i] = 'P';
    }
}

bool isOpponent(bool isWhite, char piece)
{
    return isWhite ? (piece > 'a' && piece < 'z') : (piece > 'A' && piece < 'Z');
}
bool isFreindly(bool isWhite, char piece) // return true if white is playing and peice is white or black is playing piece is black
{
    return isWhite ? (piece > 'A' && piece < 'Z') : (piece > 'a' && piece < 'z');
}

void getPositions(vector<vector<char>> board, vector<pair<int, int>> &possiblePositions, int Istart, int Jstart, bool isWhite, vector<pair<int, int>> directions)
{

    for (const auto &direction : directions)
    {
        int dx = direction.first;
        int dy = direction.second;
        int x = Istart + dx;
        int y = Jstart + dy;
        while (x >= 0 && x < 8 && y >= 0 && y < 8)
        {
            if (board[x][y] == '*')
            {
                possiblePositions.push_back({x, y});
            }
            else if (isOpponent(isWhite, board[x][y]))
            {
                possiblePositions.push_back({x, y});
                break;
            }
            else
                break;
            x += dx;
            y += dy;
        }
    }
}

void pawnKillings(vector<pair<int, int>> &possiblePositions, const vector<vector<char>> board, int Istart, int Jstart, bool isWhite)
{
    int direction = isWhite ? -1 : 1;
    vector<pair<int, int>> pawnKillingDirections = {{direction, -1}, {direction, 1}};
    for (auto dir : pawnKillingDirections)
    {
        int x = Istart + dir.first;
        int y = Jstart + dir.second;
        if (x >= 0 && x < 8 && y >= 0 && y < 8 && isOpponent(isWhite, board[x][y]))
        {
            possiblePositions.push_back({x, y});
        }
    }
}
void getPawnMoves(vector<pair<int, int>> &possiblePositions, const vector<vector<char>> &board, int Istart, int Jstart, bool isWhite)
{
    int direction = isWhite ? -1 : 1;
    int startRow = isWhite ? 6 : 1;

    if (board[Istart + direction][Jstart] == '*')
    {
        possiblePositions.push_back({Istart + direction, Jstart});

        if (Istart == startRow && board[Istart + 2 * direction][Jstart] == '*')
        {
            possiblePositions.push_back({Istart + 2 * direction, Jstart});
        }
    }

    // Capturing moves
    pawnKillings(possiblePositions, board, Istart, Jstart, isWhite);

    // EnPassant kill
    if (Istart == (isWhite ? 3 : 4)) // Checking correct row for en passant
    {
        if (lastDoubleStep.first == Istart && abs(lastDoubleStep.second - Jstart) == 1)//checking correct pawn position (side by side)
        {
            possiblePositions.push_back({Istart + direction, lastDoubleStep.second});
        }
    }
}

void getKingKnightPositions(vector<vector<char>> board, vector<pair<int, int>> &possiblePositions, int Istart, int Jstart, vector<pair<int, int>> directions, bool isWhite)
{
    char piece = board[Istart][Jstart];
    for (const auto &direction : directions)
    {
        int x = Istart + direction.first;
        int y = Jstart + direction.second;
        if (x >= 0 && x < 8 && y >= 0 && y < 8 && (board[x][y] == '*' || isOpponent(isWhite, board[x][y])))
        {
            possiblePositions.push_back({x, y});
        }
    }
    bool whiteQueenSideCastle = get<1>(canWhiteCastle) && get<0>(canWhiteCastle);
    bool whiteKingSideCastle = get<1>(canWhiteCastle) && get<2>(canWhiteCastle);
    bool blackQueenSideCastle = get<1>(canBlackCastle) && get<0>(canBlackCastle);
    bool blackKingSideCastle = get<1>(canBlackCastle) && get<2>(canBlackCastle);

    if (piece == 'K' && isWhite && Istart == 7 && Jstart == 4)
    {
        if (whiteKingSideCastle && board[Istart][Jstart + 1] == '*' && board[Istart][Jstart + 2] == '*')
        {
            possiblePositions.push_back({Istart, Jstart + 2});
        }
        if (whiteQueenSideCastle && board[Istart][Jstart - 1] == '*' && board[Istart][Jstart - 2] == '*' && board[Istart][Jstart - 3] == '*')
        {
            possiblePositions.push_back({Istart, Jstart - 2});
        }
    }
    else if (piece == 'k' && !isWhite && Istart == 0 && Jstart == 4)
    {
        if (blackKingSideCastle && board[Istart][Jstart + 1] == '*' && board[Istart][Jstart + 2] == '*')
        {
            possiblePositions.push_back({Istart, Jstart + 2});
        }
        if (blackQueenSideCastle && board[Istart][Jstart - 1] == '*' && board[Istart][Jstart - 2] == '*' && board[Istart][Jstart - 3] == '*')
        {
            possiblePositions.push_back({Istart, Jstart - 2});
        }
    }
}

vector<pair<int, int>> getPossiblePositions(vector<vector<char>> board, int Istart, int Jstart, char piece)
{
    vector<pair<int, int>> possiblePositions;
    vector<pair<int, int>> rookDirections = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    vector<pair<int, int>> bishopDirections = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
    vector<pair<int, int>> kingDirections = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    vector<pair<int, int>> knightMoves = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    bool isWhite = (piece > 'A' && piece < 'Z');

    if (tolower(piece) == 'p')
    {
        getPawnMoves(possiblePositions, board, Istart, Jstart, isWhite);
    }
    else if (tolower(piece) == 'r')
    {
        getPositions(board, possiblePositions, Istart, Jstart, isWhite, rookDirections);
    }
    else if (tolower(piece) == 'b')
    {
        getPositions(board, possiblePositions, Istart, Jstart, isWhite, bishopDirections);
    }
    else if (tolower(piece) == 'q')
    {
        getPositions(board, possiblePositions, Istart, Jstart, isWhite, rookDirections);
        getPositions(board, possiblePositions, Istart, Jstart, isWhite, bishopDirections);
    }
    else if (tolower(piece) == 'k')
    {
        getKingKnightPositions(board, possiblePositions, Istart, Jstart, kingDirections, isWhite);
    }
    else if (tolower(piece) == 'n')
    {
        getKingKnightPositions(board, possiblePositions, Istart, Jstart, knightMoves, isWhite);
    }
    return possiblePositions;
}

pair<char, char> isKingChecked(const vector<vector<char>> &board, bool isWhite)
{
    pair<int, int> blackKingPos;
    pair<int, int> whiteKingPos;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j] == 'k')
            {
                blackKingPos = {i, j};
            }
            else if (board[i][j] == 'K')
            {
                whiteKingPos = {i, j};
            }
        }
    }

    pair<char, char> isChecked = {'n', 'n'};

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j] != '*')
            {
                vector<pair<int, int>> possibleMoves = getPossiblePositions(board, i, j, board[i][j]);
                for (const auto &move : possibleMoves)
                {
                    if (move == whiteKingPos && isOpponent(true, board[i][j]))
                    {
                        isChecked.first = 'w';
                    }
                    if (move == blackKingPos && isOpponent(false, board[i][j]))
                    {
                        isChecked.second = 'b';
                    }
                }
            }
        }
    }

    return isChecked;
}

bool checkValidMove(vector<vector<char>> board, string m, char player)
{
    if (m.length() != 5 || m[2] != '-' || (m[0] < 'a' || m[0] > 'h') || (m[3] < 'a' || m[3] > 'h') || (m[1] < '1' || m[1] > '8') || (m[4] < '1' || m[4] > '8'))
    {
        cout << "invalid Syntax" << endl;
        return false;
    }

    int Istart = '8' - m[1];
    int Jstart = m[0] - 'a';
    int Iend = '8' - m[4];
    int Jend = m[3] - 'a';

    if ((player == 'w' && (board[Istart][Jstart] < 'A' || board[Istart][Jstart] > 'Z')) ||
        (player == 'b' && (board[Istart][Jstart] < 'a' || board[Istart][Jstart] > 'z')))
    {
        cout << "please pick correct piece" << endl;
        return false;
    }

    char piece = board[Istart][Jstart];
    vector<pair<int, int>> possiblePositions = getPossiblePositions(board, Istart, Jstart, piece);
    for (const auto &move : possiblePositions)
    {
        if (Iend == move.first && Jend == move.second)
        {
            if (tolower(piece) == 'k' && abs(Jend - Jstart) == 2) // Castling
            {
                if (isKingChecked(board, player == 'w').first == 'w' || isKingChecked(board, player == 'b').second == 'b')
                {
                    cout << "Cannot castle while in check" << endl;
                    return false;
                }

                if (Jend - Jstart == 2) // King Side castling
                {
                    if (isKingChecked(board, player == 'w').first == 'w' || isKingChecked(board, player == 'b').second == 'b')
                    {
                        cout << "Cannot castle through or into check" << endl;
                        return false;
                    }
                }
                else if (Jend - Jstart == -2) // Queen Side castling
                {
                    if (isKingChecked(board, player == 'w').first == 'w' || isKingChecked(board, player == 'b').second == 'b')
                    {
                        cout << "Cannot castle through or into check" << endl;
                        return false;
                    }
                }
            }
            return true;
        }
    }
    cout << "Invalid Move" << endl;
    return false;
}

void pawnPromotion(vector<vector<char>> &board, int Iend, int Jend, char player)
{
    if ((player == 'w' && Iend == 0 && board[Iend][Jend] == 'P') ||
        (player == 'b' && Iend == 7 && board[Iend][Jend] == 'p'))
    {
        char promotion;
        bool validInput = false;
        while (!validInput)
        {
            cout << "Promote pawn to (Q, R, B, N): ";
            cin >> promotion;
            promotion = toupper(promotion);
            if (promotion == 'Q' || promotion == 'R' || promotion == 'B' || promotion == 'N')
            {
                board[Iend][Jend] = (player == 'w') ? toupper(promotion) : tolower(promotion);
                validInput = true;
            }
            else
            {
                cout << "Invalid input. Please enter Q, R, B, or N." << endl;
            }
        }
    }
}

void movepiece(vector<vector<char>> &board, string m, char player, stack<tuple<int, int, int, int, char>> &moveHistory)
{
    int Istart = '8' - m[1];
    int Jstart = m[0] - 'a';
    int Iend = '8' - m[4];
    int Jend = m[3] - 'a';
    char capturedPiece = board[Iend][Jend];

    moveHistory.push(make_tuple(Istart, Jstart, Iend, Jend, capturedPiece));

    // Enpassant kill
    if (tolower(board[Istart][Jstart]) == 'p' && Jstart != Jend && board[Iend][Jend] == '*')//when pawn moves cross
    {
        capturedPiece = board[Istart][Jend]; // side of pawn
        board[Istart][Jend] = '*';
        //need to change top value as the capturedPiece is not recorded but on going back after enPassant does a mistake :( 
        moveHistory.top() = make_tuple(Istart, Jstart, Iend, Jend, capturedPiece); 

    }

    //normal moves but in castling it only updates the king
    board[Iend][Jend] = board[Istart][Jstart];
    board[Istart][Jstart] = '*';

    if (tolower(board[Iend][Jend]) == 'k' && abs(Jend - Jstart) == 2) // so for rook movement
    {
        if (Jend == 6)
        { // King-side
            board[Iend][5] = board[Iend][7];
            board[Iend][7] = '*';
        }
        else if (Jend == 2)
        { // Queen-side
            board[Iend][3] = board[Iend][0];
            board[Iend][0] = '*';
        }
        if (player == 'w')
        {
            canWhiteCastle = {false, false, false};
        }
        else
        {
            canBlackCastle = {false, false, false};
        }
    }

    if (tolower(board[Iend][Jend]) == 'k')//if king moves cancel castling
    {
        if (player == 'w')
        {
            get<1>(canWhiteCastle) = false;
        }
        else
        {
            get<1>(canBlackCastle) = false;
        }
    }
    else if (tolower(board[Iend][Jend]) == 'r')//if rook moves cancel castling to that side
    {
        if (player == 'w')
        {
            if (Istart == 7 && Jstart == 0)
            {
                get<0>(canWhiteCastle) = false;
            }
            else if (Istart == 7 && Jstart == 7)
            {
                get<2>(canWhiteCastle) = false;
            }
        }
        else
        {
            if (Istart == 0 && Jstart == 0)
            {
                get<0>(canBlackCastle) = false;
            }
            else if (Istart == 0 && Jstart == 7)
            {
                get<2>(canBlackCastle) = false;
            }
        }
    }

    if (tolower(board[Iend][Jend]) == 'p' && abs(Iend - Istart) == 2)
    {
        lastDoubleStep = {Iend, Jend};
    }
    else
    {
        lastDoubleStep = {-1, -1};
    }

    // Check for pawn promotion
    pawnPromotion(board, Iend, Jend, player);
}

void goBack(vector<vector<char>> &board, stack<tuple<int, int, int, int, char>> &moveHistory)
{ // undo
    if (!moveHistory.empty())
    {
        int Istart = get<0>(moveHistory.top());
        int Jstart = get<1>(moveHistory.top());
        int Iend = get<2>(moveHistory.top());
        int Jend = get<3>(moveHistory.top());
        char capturedPiece = get<4>(moveHistory.top());

        board[Istart][Jstart] = board[Iend][Jend];
        if (tolower(board[Istart][Jstart]) == 'p' && board[Iend][Jend] == '*' && Jstart != Jend)
        {
            board[Iend][Jend] = '*';
            board[Istart][Jend] = capturedPiece;
        }
        else
        {
            board[Iend][Jend] = capturedPiece;
        }

        moveHistory.pop();
    }
    else
    {
        cout << "No more moves to go back" << endl;
    }
}

vector<tuple<int, int, int, int>> possibleMovesOnCheck(vector<vector<char>> board, bool isWhite, stack<tuple<int, int, int, int, char>> &moveHistory)
{
    vector<tuple<int, int, int, int>> movesOnCheck;
    char kingCheck = isWhite ? 'b' : 'w';

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (isOpponent(isWhite, board[i][j]))
            {
                vector<pair<int, int>> moves = getPossiblePositions(board, i, j, board[i][j]);

                for (auto move : moves)
                {
                    int x = move.first;
                    int y = move.second;

                    char capturedPiece = board[x][y];

                    board[x][y] = board[i][j];
                    board[i][j] = '*';
                    moveHistory.push({i, j, x, y, capturedPiece});

                    pair<char, char> checkedPlayers = isKingChecked(board, isWhite);

                    if ((isWhite && checkedPlayers.second != kingCheck) || (!isWhite && checkedPlayers.first != kingCheck))
                    {
                        movesOnCheck.push_back({i, j, x, y});
                    }
                    goBack(board, moveHistory);
                }
            }
        }
    }

    return movesOnCheck;
}

void printRules()
{
    cout << "Welcome to Chess!\n";
    cout << "Rules of the game:\n";
    cout << "1. The game is played on an 8x8 board.\n";
    cout << "2. The pieces are King (K), Queen (Q), Rook (R), Bishop (B), Knight (N), and Pawn (P).\n";
    cout << "3. White moves first, followed by Black.\n";
    cout << "4. To move a piece, enter the move in the format e2-e4 (move from e2 to e4).\n";
    cout << "5. If a pawn reaches the opposite end of the board, it must be promoted to Q, R, B, or N.\n";
    cout << "6. If a King is in check, it must be moved out of check. If no valid moves are available, it is checkmate.\n";
    cout << "7. You cannot make a move that would put or leave your own King in check.\n";
    cout << "8. The game ends when a King is checkmated or a stalemate occurs.\n";
    cout << "9. Type 'exit' to quit the game at any time.\n";
    cout << "10. Have fun!\n\n";
}

int main()
{
    vector<vector<char>> board(8, vector<char>(8, '*'));
    prepareBoard(board);
    printRules();
    displayBoard(board);
    string move;
    char player = 'w';
    stack<tuple<int, int, int, int, char>> moveHistory; // to store moves history and moving back

    while (true)
    {
        string playerStr = player == 'w' ? "White" : "Black";
        bool isWhite = player == 'w';
        cout << playerStr << " Enter your move (e2-e4): ";
        cin >> move;

        if (move == "exit")
        {
            cout << "Game exited by user." << endl;
            break;
        }

        if (checkValidMove(board, move, player))
        {
            movepiece(board, move, player, moveHistory);
            pair<char, char> checkedPlayers = isKingChecked(board, isWhite);
            if ((isWhite && checkedPlayers.first == 'w') || (!isWhite && checkedPlayers.second == 'b'))
            { // when player tries to move a pinned piece
                string checkedplayer = isWhite ? "White" : "Black";
                cout << checkedplayer << "'s King will be checked \n can't move that" << endl;
                goBack(board, moveHistory);
            }
            else
            {
                displayBoard(board);
                if ((isWhite && checkedPlayers.second == 'b') || (!isWhite && checkedPlayers.first == 'w'))
                {
                    string checkedplayer = isWhite ? "Black" : "White";
                    cout << checkedplayer << "'s King is Checked" << endl;
                    vector<tuple<int, int, int, int>> moves = possibleMovesOnCheck(board, isWhite, moveHistory);
                    if (moves.size() == 0)
                    {
                        cout << "Game Over \n"
                             << playerStr << " Wins!!!" << endl;
                        break;
                    }
                }
                player = (isWhite) ? 'b' : 'w';
            }
        }
        else
        {
            cout << "Invalid move. Please try again." << endl;
        }
    }
    return 0;
}

