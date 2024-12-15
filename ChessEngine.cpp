#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <limits>
#include <cctype>

// Rozmiar szachownicy
const int BOARD_SIZE = 8;

// Klasa reprezentująca szachownicę
class ChessBoard {
private:
    std::vector<std::vector<char>> board;  // Szachownica 8x8
    std::map<char, int> pieceValues;       // Wartości figur
    const int MAX_DEPTH = 2;               // Głębokość dla algorytmu minimax

public:
    ChessBoard() {
        // Inicjalizacja wartości figur
        pieceValues = {
            {'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 1000},
            {'p', -1}, {'n', -3}, {'b', -3}, {'r', -5}, {'q', -9}, {'k', -1000}
        };

        // Ustawienie początkowego stanu szachownicy
        initializeBoard();
    }

    // Inicjalizacja początkowego układu szachownicy
    void initializeBoard() {
        board = {
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {'.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.'},
            {'.', '.', '.', '.', '.', '.', '.', '.'},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        };
    }

    // Wyświetlanie szachownicy
    void displayBoard() {
        std::cout << "  a b c d e f g h\n";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            std::cout << 8 - i << " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                std::cout << board[i][j] << " ";
            }
            std::cout << 8 - i << "\n";
        }
        std::cout << "  a b c d e f g h\n";
    }

    // Sprawdzenie, czy współrzędne są w granicach planszy
    bool isInBounds(int row, int col) {
        return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
    }

    // Sprawdzenie, czy pole jest puste
    bool isSquareEmpty(int row, int col) {
        return isInBounds(row, col) && board[row][col] == '.';
    }

    // Generowanie ruchów dla wszystkich figur
    std::vector<std::string> generateMoves(bool isWhite) {
        std::vector<std::string> moves;

        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                char piece = board[i][j];

                // Sprawdzanie, czy figura należy do gracza
                if ((isWhite && std::isupper(piece)) || (!isWhite && std::islower(piece))) {
                    generatePieceMoves(i, j, moves);
                }
            }
        }

        return moves;
    }

    // Generowanie ruchów dla pojedynczej figury
    void generatePieceMoves(int row, int col, std::vector<std::string>& moves) {
        char piece = board[row][col];
        switch (std::tolower(piece)) {
            case 'p':
                generatePawnMoves(row, col, moves, std::isupper(piece));
                break;
            case 'n':
                generateKnightMoves(row, col, moves);
                break;
            case 'b':
                generateSlidingMoves(row, col, moves, {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}});
                break;
            case 'r':
                generateSlidingMoves(row, col, moves, {{-1, 0}, {1, 0}, {0, -1}, {0, 1}});
                break;
            case 'q':
                generateSlidingMoves(row, col, moves, {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}});
                break;
            case 'k':
                generateKingMoves(row, col, moves);
                break;
        }
    }

    // Generowanie ruchów dla pionów
    void generatePawnMoves(int row, int col, std::vector<std::string>& moves, bool isWhite) {
        int direction = isWhite ? -1 : 1;

        // Ruch do przodu
        if (isInBounds(row + direction, col) && isSquareEmpty(row + direction, col)) {
            moves.push_back(moveToString(row, col, row + direction, col));
        }

        // Bicie
        for (int dc : {-1, 1}) {
            if (isInBounds(row + direction, col + dc) && !isSquareEmpty(row + direction, col + dc)) {
                moves.push_back(moveToString(row, col, row + direction, col + dc));
            }
        }
    }

    // Generowanie ruchów dla skoczka
    void generateKnightMoves(int row, int col, std::vector<std::string>& moves) {
        const std::vector<std::pair<int, int>> knightOffsets = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2}, {1, 2}, {2, -1}, {2, 1}
        };

        for (const auto& [dr, dc] : knightOffsets) {
            int newRow = row + dr, newCol = col + dc;
            if (isInBounds(newRow, newCol)) {
                moves.push_back(moveToString(row, col, newRow, newCol));
            }
        }
    }

    // Generowanie ruchów dla figur ślizgowych (wieża, goniec, hetman)
    void generateSlidingMoves(int row, int col, std::vector<std::string>& moves, const std::vector<std::pair<int, int>>& directions) {
        for (const auto& [dr, dc] : directions) {
            int newRow = row + dr, newCol = col + dc;
            while (isInBounds(newRow, newCol) && isSquareEmpty(newRow, newCol)) {
                moves.push_back(moveToString(row, col, newRow, newCol));
                newRow += dr;
                newCol += dc;
            }
        }
    }

    // Generowanie ruchów króla
    void generateKingMoves(int row, int col, std::vector<std::string>& moves) {
        const std::vector<std::pair<int, int>> kingOffsets = {
            {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
            {0, 1}, {1, -1}, {1, 0}, {1, 1}
        };

        for (const auto& [dr, dc] : kingOffsets) {
            int newRow = row + dr, newCol = col + dc;
            if (isInBounds(newRow, newCol)) {
                moves.push_back(moveToString(row, col, newRow, newCol));
            }
        }
    }

    // Konwersja ruchu na notację (np. e2e4)
    std::string moveToString(int startRow, int startCol, int endRow, int endCol) {
        return std::string() + char('a' + startCol) + char('8' - startRow) +
               char('a' + endCol) + char('8' - endRow);
    }

    // Symulacja ruchu (wykonanie)
    void makeMove(const std::string& move) {
        int startCol = move[0] - 'a';
        int startRow = '8' - move[1];
        int endCol = move[2] - 'a';
        int endRow = '8' - move[3];
        board[endRow][endCol] = board[startRow][startCol];
        board[startRow][startCol] = '.';
    }

    // Cofnięcie ruchu
    void undoMove(const std::string& move, char capturedPiece) {
        int startCol = move[0] - 'a';
        int startRow = '8' - move[1];
        int endCol = move[2] - 'a';
        int endRow = '8' - move[3];
        board[startRow][startCol] = board[endRow][endCol];
        board[endRow][endCol] = capturedPiece;
    }

    // Ocena pozycji (suma wartości figur)
    int evaluatePosition() {
        int score = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                char piece = board[i][j];
                if (pieceValues.find(piece) != pieceValues.end()) {
                    score += pieceValues[piece];
                }
            }
        }
        return score;
    }

    // Algorytm Minimax
    int minimax(int depth, bool isMaximizingPlayer) {
        if (depth == 0) return evaluatePosition();

        std::vector<std::string> moves = generateMoves(isMaximizingPlayer);
        if (moves.empty()) return evaluatePosition(); // Gra zakończona

        int bestScore = isMaximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            char capturedPiece = board[move[3] - '8'][move[2] - 'a'];
            makeMove(move);

            int score = minimax(depth - 1, !isMaximizingPlayer);
            if (isMaximizingPlayer) {
                bestScore = std::max(bestScore, score);
            } else {
                bestScore = std::min(bestScore, score);
            }

            undoMove(move, capturedPiece);
        }
        return bestScore;
    }

    // Wybór najlepszego ruchu
    std::string getBestMove(bool isWhite) {
        std::vector<std::string> moves = generateMoves(isWhite);
        std::string bestMove;
        int bestScore = isWhite ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

        for (const auto& move : moves) {
            char capturedPiece = board[move[3] - '8'][move[2] - 'a'];
            makeMove(move);

            int score = minimax(MAX_DEPTH - 1, !isWhite);
            if (isWhite ? (score > bestScore) : (score < bestScore)) {
                bestScore = score;
                bestMove = move;
            }

            undoMove(move, capturedPiece);
        }

        return bestMove;
    }
};

int main() {
    ChessBoard chessEngine;
    std::string playerMove;

    std::cout << "Grasz białymi. Komputer gra czarnymi.\n\n";
    chessEngine.displayBoard();

    while (true) {
        // Ruch gracza
        std::cout << "Twój ruch: ";
        std::cin >> playerMove;

        // Dodaj sprawdzanie poprawności i wykonanie ruchu
        chessEngine.makeMove(playerMove);
        chessEngine.displayBoard();

        // Ruch komputera
        std::cout << "Czekaj... komputer analizuje ruch.\n";
        std::string computerMove = chessEngine.getBestMove(false);
        std::cout << "Komputer wykonuje ruch: " << computerMove << "\n";
        chessEngine.makeMove(computerMove);
        chessEngine.displayBoard();
    }

    return 0;
}
