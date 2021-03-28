//
// Created by Oliver on 27.03.21.
//

#ifndef PGE_TETRIS_2_TETRIMINO_H
#define PGE_TETRIS_2_TETRIMINO_H

#include <array>

#include "olcPixelGameEngine.h"
#include "tetriminoType.h"
#include "playfield.h"

class Tetrimino {
public:
    Tetrimino() = default;

    explicit Tetrimino(TetriminoType type);

public:
    static Tetrimino random();

    void Draw(olc::PixelGameEngine *pge, Playfield *playfield);

    void MoveLeft(Playfield *playfield);

    void MoveRight(Playfield *playfield);

    void MoveDown(Playfield *playfield);

    void Flip(Playfield *playfield);

    bool canMoveToPosition(Playfield *playfield, const olc::vi2d& newPosition);

    void HardDrop(Playfield *playfield);

public:
    bool isInFinalPosition = false;
private:
    const static int8_t typeCount = 7;
    TetriminoType mType;
    olc::Pixel mColor;
    std::array<std::array<int8_t, 5>, 5> mShape{};
    olc::vi2d mBlockPosition = {5, 5};
};


#endif //PGE_TETRIS_2_TETRIMINO_H
