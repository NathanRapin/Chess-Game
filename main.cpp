#include "chess.hpp"
#include "chess.cpp"

int main() {
    ChessBoard chess;
    chess.initializeBoard();
    chess.play();

    return 0;
}