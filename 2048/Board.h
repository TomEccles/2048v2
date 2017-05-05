#pragma once
#include "stdafx.h"
#include <array>

constexpr int side = 4;

enum class Move
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE,
};
const std::array<Move, 4> allMoves = { Move::UP, Move::DOWN, Move::LEFT, Move::RIGHT };


class Board
{
private:
    std::array<int, side*side> board;
    bool consolidateInPlace(int start, int gap);
    bool removeZerosInPlace(int start, int gap);
    bool squashInPlace(int start, int gap);
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    bool canMove(Move move);

public:
    Board();
    ~Board();
    std::array<int, 36> calcArray();
    std::string toString();
    std::string normalisedString();
    std::string moveIndicatorString();
    bool move(Move move);
    bool addRandom();
    void print(int indent = 0);
    bool equals(Board other);
    bool operator <(const Board& rhs) const
    {
        return board < rhs.board;
    }
};
