//
// Created by Oliver on 27.03.21.
//

#ifndef PGE_TETRIS_2_PLAYFIELD_H
#define PGE_TETRIS_2_PLAYFIELD_H

#include "olcPixelGameEngine.h"

class Playfield {
public:
    Playfield() = default;

    explicit Playfield(const olc::vi2d &realPosition);

    void Draw(olc::PixelGameEngine *pge);

    olc::vi2d BlockToReal(olc::vi2d pos);

    void FillBlock(const olc::vi2d &position, olc::Pixel color);

    bool IsOccupied(olc::vi2d pos);

    void ClearLine(int32_t y);

    void CheckForFullLines();

    int32_t getScore();

    void increaseScore(int32_t amount);

    void incrementLinesCleared();

    int8_t getLevel();

    void incrementLevel();

    float getTick();

    int32_t getLinesCleared();

private:
    olc::vi2d mRealPos;
    olc::vi2d mSize = {101, 201};
    int8_t mSpace[10][20] = {0};
    olc::Pixel mSpaceColor[10][20] = {olc::BLACK};
    int32_t mScore = 0;
    int8_t mLevel = 0;
    int32_t mLinesCleared = 0;
    float mTick = 1.0f;
};


#endif //PGE_TETRIS_2_PLAYFIELD_H
