//
// Created by Oliver on 27.03.21.
//

#ifndef PGE_TETRIS_2_PLAYFIELD_H
#define PGE_TETRIS_2_PLAYFIELD_H

#include "olcPixelGameEngine.h"

class Playfield {
public:
    Playfield() = default;
    explicit Playfield(const olc::vi2d& realPosition);
    void Draw(olc::PixelGameEngine* pge);
    olc::vi2d BlockToReal(olc::vi2d pos);
    void FillBlock(const olc::vi2d& position, olc::Pixel color);
    bool IsOccupied(olc::vi2d pos);
private:
    olc::vi2d mRealPos;
    olc::vi2d mSize = {100, 200};
    int8_t mSpace[10][20] = {0};
    olc::Pixel mSpaceColor[10][20] = {olc::BLACK};
};


#endif //PGE_TETRIS_2_PLAYFIELD_H
