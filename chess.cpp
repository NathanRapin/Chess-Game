#include "chess.hpp"
#include <iostream>

PieceColor getOpponent(PieceColor player)
{
    return (player == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}

ChessBoard::ChessBoard() : chessBoard(BOARD_SIZE, std::vector<ChessPiece>(BOARD_SIZE, {PieceType::EMPTY, PieceColor::NONE})), currentPlayer(PieceColor::WHITE), whiteKingPosition({0, 4}), blackKingPosition({7, 4}) {}

void ChessBoard::initializeBoard()
{
    // Initialize pawns
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        chessBoard[1][i] = {PieceType::PAWN, PieceColor::WHITE};
        chessBoard[6][i] = {PieceType::PAWN, PieceColor::BLACK};
    }

    // Initialize other pieces
    chessBoard[0][0] = {PieceType::ROOK, PieceColor::WHITE};
    chessBoard[0][7] = {PieceType::ROOK, PieceColor::WHITE};
    chessBoard[7][0] = {PieceType::ROOK, PieceColor::BLACK};
    chessBoard[7][7] = {PieceType::ROOK, PieceColor::BLACK};

    chessBoard[0][1] = {PieceType::KNIGHT, PieceColor::WHITE};
    chessBoard[0][6] = {PieceType::KNIGHT, PieceColor::WHITE};
    chessBoard[7][1] = {PieceType::KNIGHT, PieceColor::BLACK};
    chessBoard[7][6] = {PieceType::KNIGHT, PieceColor::BLACK};

    chessBoard[0][2] = {PieceType::BISHOP, PieceColor::WHITE};
    chessBoard[0][5] = {PieceType::BISHOP, PieceColor::WHITE};
    chessBoard[7][2] = {PieceType::BISHOP, PieceColor::BLACK};
    chessBoard[7][5] = {PieceType::BISHOP, PieceColor::BLACK};

    chessBoard[0][3] = {PieceType::QUEEN, PieceColor::WHITE};
    chessBoard[7][3] = {PieceType::QUEEN, PieceColor::BLACK};

    chessBoard[0][4] = {PieceType::KING, PieceColor::WHITE};
    chessBoard[7][4] = {PieceType::KING, PieceColor::BLACK};
}

void ChessBoard::displayBoard() 
{
    for (int i = (currentPlayer == PieceColor::BLACK) ? 0 : BOARD_SIZE - 1;
         (currentPlayer == PieceColor::BLACK) ? (i < BOARD_SIZE) : (i >= 0);
         (currentPlayer == PieceColor::BLACK) ? ++i : --i)
    {
        // Print rank name
        std::cout << (i + 1) << " ";
        for (int j = (currentPlayer == PieceColor::WHITE) ? 0 : BOARD_SIZE - 1;
             (currentPlayer == PieceColor::WHITE) ? (j < BOARD_SIZE) : (j >= 0);
             (currentPlayer == PieceColor::WHITE) ? ++j : --j) 
        {
            char pieceChar = ' ';
            switch (chessBoard[i][j].type)
            {
                case PieceType::EMPTY: pieceChar = ' '; break;
                case PieceType::PAWN: pieceChar = (chessBoard[i][j].color == PieceColor::WHITE) ? 'P' : 'p'; break;
                case PieceType::ROOK: pieceChar = (chessBoard[i][j].color == PieceColor::WHITE) ? 'R' : 'r'; break;
                case PieceType::KNIGHT: pieceChar = (chessBoard[i][j].color == PieceColor::WHITE) ? 'N' : 'n'; break;
                case PieceType::BISHOP: pieceChar = (chessBoard[i][j].color == PieceColor::WHITE) ? 'B' : 'b'; break;
                case PieceType::QUEEN: pieceChar = (chessBoard[i][j].color == PieceColor::WHITE) ? 'Q' : 'q'; break;
                case PieceType::KING: pieceChar = (chessBoard[i][j].color == PieceColor::WHITE) ? 'K' : 'k'; break;
            }
            std::wcout << pieceChar << " ";
        }
        std::cout << "\n";
    }

    // Print file name
    if (currentPlayer == PieceColor::BLACK) {std::cout << "  h g f e d c b a\n";}
    else {std::cout << "  a b c d e f g h\n";}
}

void ChessBoard::validateMoveFormat(const std::string& move)
{
    // Use of isalpha and isdigit functions to verify the format is the same as "e2 e4"
    if (move.length() != 5 ||
        !isalpha(move[0]) || !isdigit(move[1]) || move[2] != ' ' ||
        !isalpha(move[3]) || !isdigit(move[4]))
    {
        throw InvalidFormatException();
    }
}

void ChessBoard::convertAlgebraicToCoords(const std::string& move, std::pair<int, int>& source, std::pair<int, int>& dest)
{
    // Convert 'a' into 0 and 'h' into 7
    int sourceFile = move[0] - 'a';
    int destFile = move[3] - 'a';
    // Convert '1' into 0 and '8' into 7
    int sourceRank = move[1] - '1';
    int destRank = move[4] - '1';

    source = {sourceRank, sourceFile};
    dest = {destRank, destFile};
}

void ChessBoard::validatePawnMove(const std::pair<int, int>& source, const std::pair<int, int>& dest) {
    int sourceRank = source.first;
    int destRank = dest.first;
    int sourceFile = source.second;
    int destFile = dest.second;

    ChessPiece sourcePiece = chessBoard[sourceRank][sourceFile];
    ChessPiece destPiece = chessBoard[destRank][destFile];

    // Ensure pawns are moving in the correct direction based on their color
    int moveDirection = (sourcePiece.color == PieceColor::WHITE) ? 1 : -1;
    int initialRank = (sourcePiece.color == PieceColor::WHITE) ? 1 : 6;

    // Pawns can move one square forward
    if (destRank == sourceRank + moveDirection && destFile == sourceFile && destPiece.type == PieceType::EMPTY)
    {
        return;
    }

    // Pawns can move two squares forward from their starting rank
    if (sourceRank == initialRank && destRank == sourceRank + 2 * moveDirection &&
        destFile == sourceFile && destPiece.type == PieceType::EMPTY &&
        chessBoard[sourceRank + moveDirection][sourceFile].type == PieceType::EMPTY)
    {
        return;
    }

    // Pawns can capture diagonally
    if (destRank == sourceRank + moveDirection && (destFile == sourceFile + 1 || destFile == sourceFile - 1) &&
        destPiece.color == getOpponent(currentPlayer))
    {
        return;
    }

    throw InvalidMoveException();
}

void ChessBoard::validateRookMove(const std::pair<int, int>& source, const std::pair<int, int>& dest, bool isQueen) {
    int sourceRank = source.first;
    int destRank = dest.first;
    int sourceFile = source.second;
    int destFile = dest.second;

    // Check if the move is either horizontal or vertical and not staying in the same position
    if (sourceRank == destRank || sourceFile == destFile) {
        // Check if there are no pieces in the way along the rank
        if (sourceRank == destRank)
        {
            int step = (destFile > sourceFile) ? 1 : -1;
            for (int file = sourceFile + step; file != destFile; file += step)
            {
                if (chessBoard[sourceRank][file].type != PieceType::EMPTY)
                {
                    throw InvalidMoveException();
                }
            }
        }

        // Check if there are no pieces in the way along the file
        if (sourceFile == destFile)
        {
            int step = (destRank > sourceRank) ? 1 : -1;
            for (int rank = sourceRank + step; rank != destRank; rank += step)
            {
                if (chessBoard[rank][sourceFile].type != PieceType::EMPTY)
                {
                    throw InvalidMoveException();
                }
            }
        }

        // Check if the destination square is empty or occupied by an opponent's piece
        // In particular, returns false when the piece is not moving
        if (chessBoard[destRank][destFile].color != currentPlayer)
        {
            return;
        }
        else
        {
            throw InvalidMoveException();
        }
    }

    throw InvalidMoveException();
}

void ChessBoard::validateKnightMove(const std::pair<int, int>& source, const std::pair<int, int>& dest)
{
    int sourceRank = source.first;
    int destRank = dest.first;
    int sourceFile = source.second;
    int destFile = dest.second;

    // Knights move in an "L" shape: two squares in one direction and one square perpendicular to that
    int rankDiff = abs(destRank - sourceRank);
    int fileDiff = abs(destFile - sourceFile);

    if ((rankDiff == 2 && fileDiff == 1) || (rankDiff == 1 && fileDiff == 2) && chessBoard[destRank][destFile].color != currentPlayer)
    {
        return;
    }

    throw InvalidMoveException();
}

void ChessBoard::validateBishopMove(const std::pair<int, int>& source, const std::pair<int, int>& dest, bool isQueen)
{
    int sourceRank = source.first;
    int destRank = dest.first;
    int sourceFile = source.second;
    int destFile = dest.second;

    // Check if the move is diagonal
    if (abs(destRank - sourceRank) == abs(destFile - sourceFile))
    {
        // Determine the direction of the diagonal movement
        int rankStep = (destRank > sourceRank) ? 1 : -1;
        int fileStep = (destFile > sourceFile) ? 1 : -1;

        // Check if there are no pieces in the way along the diagonal
        for (int rank = sourceRank + rankStep, file = sourceFile + fileStep; rank != destRank; rank += rankStep, file += fileStep)
        {
            if (chessBoard[rank][file].type != PieceType::EMPTY)
            {
                throw InvalidMoveException();
            }
        }

        // Check if the destination square is empty or occupied by an opponent's piece
        if (chessBoard[destRank][destFile].color != currentPlayer)
        {
            return;
        }
        else
        {
            throw InvalidMoveException();
        }
    }

    throw InvalidMoveException();
}

void ChessBoard::validateQueenMove(const std::pair<int, int>& source, const std::pair<int, int>& dest)
{
    // Queen moves like a Bishop or a Rook
    try
    {
        validateRookMove(source, dest, true);
    }
    catch (const ChessException& e)
    {
        validateBishopMove(source, dest, true);
    }
}

void ChessBoard::validateKingMove(const std::pair<int, int>& source, const std::pair<int, int>& dest)
{
    int sourceRank = source.first;
    int destRank = dest.first;
    int sourceFile = source.second;
    int destFile = dest.second;

    // Check if the move is within one square in any direction
    // Check if the destination square is empty or occupied by an opponent's piece
    if (abs(destRank - sourceRank) <= 1 && abs(destFile - sourceFile) <= 1 && chessBoard[destRank][destFile].color != currentPlayer)
    {
            return;   
    }

    throw InvalidMoveException();
}

void ChessBoard::validateMove(const std::pair<int, int>& source, const std::pair<int, int>& dest)
{
    // Check if the source coordinates are within the board boundaries
    if (source.first < 0 || source.first >= BOARD_SIZE || source.second < 0 || source.second >= BOARD_SIZE)
    {
        throw OutsideBoardException();
    }

    // Check if the destination coordinates are within the board boundaries
    if (dest.first < 0 || dest.first >= BOARD_SIZE || dest.second < 0 || dest.second >= BOARD_SIZE)
    {
        throw OutsideBoardException();
    }

    ChessPiece sourcePiece = chessBoard[source.first][source.second];
    ChessPiece destPiece = chessBoard[dest.first][dest.second];

    // Check if the source piece belongs to the current player
    if (sourcePiece.color != currentPlayer)
    {
        throw InvalidMoveException();
    }

    // Check if the destination square is empty or occupied by an opponent's piece
    if (destPiece.color == currentPlayer)
    {
        throw InvalidMoveException();
    }

    // Specific move validation based on the piece type
    switch (sourcePiece.type)
    {
        case PieceType::PAWN:
            validatePawnMove(source, dest);
            return;
        case PieceType::ROOK:
            validateRookMove(source, dest, false);
            return;
        case PieceType::KNIGHT:
            validateKnightMove(source, dest);
            return;
        case PieceType::BISHOP:
            validateBishopMove(source, dest, false);
            return;
        case PieceType::QUEEN:
            validateQueenMove(source, dest);
            return;
        case PieceType::KING:
            validateKingMove(source, dest);
            return;
        default:
            throw InvalidMoveException();
    }
}

std::pair<int, int>& ChessBoard::getKingPosition(PieceColor color)
{
    if (color == PieceColor::WHITE)
    {
        return whiteKingPosition;
    }
    else
    {
        return blackKingPosition;
    }
}

void ChessBoard::updateKingPosition(const std::pair<int, int>& newPosition, PieceColor color)
{
    if (color == PieceColor::WHITE)
    {
        whiteKingPosition = newPosition;
    }
    else
    {
        blackKingPosition = newPosition;
    }
}

bool ChessBoard::isCheck()
{
    switchTurn();
    for (int i = 0 ; i < BOARD_SIZE ; ++i)
    {
        for (int j = 0 ; j < BOARD_SIZE ; ++j)
        {
            try
            {
                validateMove({i, j}, getKingPosition(getOpponent(currentPlayer))); //TO CHECK
                switchTurn();
                return true;
            }

            catch (const ChessException& e) {}
        }
    }
    switchTurn();
    return false;
}

void ChessBoard::makeMove(const std::string& move)
{
    // Validate move format
    validateMoveFormat(move);

    // Convert algebraic notation to coordinates
    std::pair<int, int> source, dest;
    convertAlgebraicToCoords(move, source, dest);

    // Validate move based on the piece
    validateMove(source, dest);

    ChessPiece sourcePiece = chessBoard[source.first][source.second];
    ChessPiece destPiece = chessBoard[dest.first][dest.second];

    chessBoard[source.first][source.second] = {PieceType::EMPTY, PieceColor::NONE};
    chessBoard[dest.first][dest.second] = sourcePiece;

    if (sourcePiece.type == PieceType::KING)
    {
        updateKingPosition(dest, sourcePiece.color);
    }

    if (isCheck())
    {
        chessBoard[source.first][source.second] = sourcePiece;
        chessBoard[dest.first][dest.second] = destPiece;

        if (sourcePiece.type == PieceType::KING)
        {
            updateKingPosition(source, sourcePiece.color);
        }

        throw InvalidMoveException();
    }

    switchTurn();

    return;
}

bool ChessBoard::isCheckmate()
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            std::pair<int, int> source = {i, j};
            ChessPiece sourcePiece = chessBoard[i][j];

            if (sourcePiece.color == currentPlayer)
            {
                for (int k = 0; k < BOARD_SIZE; ++k)
                {
                    for (int l = 0; l < BOARD_SIZE; ++l)
                    {
                        std::pair<int, int> dest = {k, l};
                        try
                        {
                            validateMove(source, dest);
                            // Simulate the move
                            ChessPiece destPiece = chessBoard[k][l];
                            chessBoard[i][j] = {PieceType::EMPTY, PieceColor::NONE};
                            chessBoard[k][l] = sourcePiece;

                            if (sourcePiece.type == PieceType::KING)
                            {
                                updateKingPosition(dest, currentPlayer);
                            }

                            // Check if the player is still in check after the move
                            if (!isCheck())
                            {
                                // Undo the move
                                chessBoard[i][j] = sourcePiece;
                                chessBoard[k][l] = destPiece;

                                if (sourcePiece.type == PieceType::KING)
                                {
                                    updateKingPosition(source, currentPlayer);
                                }

                                return false; // Player is not in checkmate
                            }

                            // Undo the move
                            chessBoard[i][j] = sourcePiece;
                            chessBoard[k][l] = destPiece;

                            if (sourcePiece.type == PieceType::KING)
                            {
                                updateKingPosition(source, currentPlayer);
                            }
                        }

                        catch (const ChessException& e)
                        {
                            continue;
                        }
                    }
                }
            }
        }
    }
    return true; // Player is in checkmate
}

void ChessBoard::switchTurn()
{
    currentPlayer = getOpponent(currentPlayer);
}

void ChessBoard::play()
{
    while (!isCheckmate())
    {
        displayBoard();
        std::string userMove;
        std::cout << "Enter your move (e.g., 'e2 e4'): ";
        std::getline(std::cin, userMove);
        try
        {
            makeMove(userMove);
        }
        catch (const ChessException& e)
        {
            std::cout << e.what();
        }
    }
    displayBoard();
    std::cout << "Checkmate !\n";
    if (currentPlayer == PieceColor::WHITE)
    {
        std::cout << "Black wins. \n";
    }
    else
    {
        std::cout << "White wins. \n";
    }
}