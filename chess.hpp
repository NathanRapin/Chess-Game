#ifndef CHESS_HPP
#define CHESS_HPP

#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>

enum class PieceType 
{
    EMPTY, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING
};

enum class PieceColor
{
    NONE, WHITE, BLACK
};

PieceColor getOpponent(PieceColor currentPlayer);

struct ChessPiece
{
    PieceType type;
    PieceColor color;
};

class ChessBoard
{
public:
    ChessBoard();
    void initializeBoard();
    void displayBoard();

    void validateMoveFormat(const std::string& move);
    void convertAlgebraicToCoords(const std::string& move, std::pair<int, int>& source, std::pair<int, int>& dest);
    void validatePawnMove(const std::pair<int, int>& source, const std::pair<int, int>& dest);
    void validateRookMove(const std::pair<int, int>& source, const std::pair<int, int>& dest, bool isQueen);
    void validateKnightMove(const std::pair<int, int>& source, const std::pair<int, int>& dest);
    void validateBishopMove(const std::pair<int, int>& source, const std::pair<int, int>& dest, bool isQueen);
    void validateQueenMove(const std::pair<int, int>& source, const std::pair<int, int>& dest);
    void validateKingMove(const std::pair<int, int>& source, const std::pair<int, int>& dest);
    void validateMove(const std::pair<int, int>& source, const std::pair<int, int>& dest);

    std::pair<int, int>& getKingPosition(PieceColor color);
    void updateKingPosition(const std::pair<int, int>& newPosition, PieceColor color);
    bool isCheck();

    void makeMove(const std::string& move);
    bool isCheckmate();
    void switchTurn();

    void play();

private:
    const int BOARD_SIZE = 8;
    std::vector<std::vector<ChessPiece>> chessBoard;
    PieceColor currentPlayer;
    std::pair<int, int> whiteKingPosition;
    std::pair<int, int> blackKingPosition;
};


class ChessException : public std::exception
{
public:
    ChessException(const char* errorMessage) : std::exception()
    {
        message = errorMessage;
    }

    const char* what() const noexcept override
    {
        return message.c_str();
    }

private:
    std::string message;
};

class InvalidMoveException : public ChessException
{
public:
    InvalidMoveException() : ChessException("Invalid move. \n") {}
};

class OutsideBoardException : public ChessException
{
public:
    OutsideBoardException() : ChessException("Position is outside the board. \n") {}
};

class InvalidFormatException : public ChessException
{
public:
    InvalidFormatException() : ChessException("Invalid move. \n") {}
};

#endif // CHESS_HPP