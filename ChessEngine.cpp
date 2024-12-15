#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Klasa reprezentująca stan szachownicy
class ChessBoard {
private:
    std::map<char, int> pieceValues; // Wartości figur
    std::vector<std::string> legalMoves; // Lista ruchów dozwolonych
    std::string boardState; // Reprezentacja szachownicy (prosta)

public:
    ChessBoard() {
        // Ustawienie wartości figur
        pieceValues = {
            {'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 1000},
            {'p', -1}, {'n', -3}, {'b', -3}, {'r', -5}, {'q', -9}, {'k', -1000}
        };

        // Proste, przykładowe legalne ruchy
        legalMoves = {"e5", "Nf6", "Nc6", "d5", "e6"};

        // Prosty stan startowy (niepełna implementacja)
        boardState = "rnbqkbnrpppppppp................................PPPPPPPPRNBQKBNR";
    }

    // Funkcja przyjmująca ruch gracza
    void makeMove(const std::string& move) {
        std::cout << "Wykonano ruch: " << move << std::endl;
        // W tej wersji uproszczonej nie zmieniamy rzeczywistego stanu szachownicy
    }

    // Funkcja zwracająca najlepszy ruch (prosta heurystyka)
    std::string getBestMove() {
        std::string bestMove = "";
        int bestValue = -10000;

        // Oceniamy każdy ruch jako losowy ruch z listy legalnych
        for (const auto& move : legalMoves) {
            int value = evaluateMove(move);
            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
        }
        return bestMove;
    }

    // Funkcja oceny ruchu (prosta heurystyka)
    int evaluateMove(const std::string& move) {
        // Przykład: ocena bazująca na obecności figury w ruchu
        int score = 0;
        for (char c : move) {
            if (pieceValues.find(c) != pieceValues.end()) {
                score += pieceValues[c];
            }
        }
        return score + (rand() % 3); // Dodanie losowości
    }
};

int main() {
    ChessBoard chessEngine;
    std::string playerMove;

    std::cout << "Prosty silnik szachowy (C++)" << std::endl;
    std::cout << "Wpisz ruch w notacji algebraicznej (np. Nf3, e4): ";

    while (std::cin >> playerMove) {
        // Wykonanie ruchu gracza
        chessEngine.makeMove(playerMove);

        // Obliczenie i wyświetlenie najlepszego ruchu silnika
        std::string bestMove = chessEngine.getBestMove();
        std::cout << "Najlepszy ruch silnika: " << bestMove << std::endl;

        std::cout << "\nWpisz kolejny ruch: ";
    }

    return 0;
}
