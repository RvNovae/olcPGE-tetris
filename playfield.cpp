//
// Created by Oliver on 27.03.21.
//

#include "playfield.h"

Playfield::Playfield(const olc::vi2d &realPosition) : mRealPos(realPosition) {

}

void Playfield::Draw(olc::PixelGameEngine *pge) {
    pge->DrawRect(((mRealPos - olc::vi2d(1, 0)) - (mSize / 2)), mSize);
    for (int x = 0; x < 10; x++)
        for (int y = 0; y < 20; y++)
            if (mSpace[x][y] == 1)
                pge->FillRect(BlockToReal(olc::vi2d(x, y)), olc::vi2d(10, 10), mSpaceColor[x][y]);
}

olc::vi2d Playfield::BlockToReal(olc::vi2d pos) {
    return (mRealPos - (mSize / 2)) + (pos * 10);
}

void Playfield::FillBlock(const olc::vi2d &position, olc::Pixel color) {
    mSpace[position.x][position.y] = 1;
    mSpaceColor[position.x][position.y] = color;
}

bool Playfield::IsOccupied(olc::vi2d pos) {
    if (mSpace[pos.x][pos.y] == 1)
        return true;
    return false;
}

void Playfield::ClearLine(int32_t y) {
    for (int x = 0; x < 10; x++) {
        mSpace[x][y] = 0;
        mSpaceColor[x][y] = olc::BLACK;
    }

    for (int row = y; row > 0; row--)
        for (int x = 0; x < 10; x++) {
            mSpace[x][row] = mSpace[x][row - 1];
            mSpaceColor[x][row] = mSpaceColor[x][row - 1];

            mSpace[x][row - 1] = 0;
            mSpaceColor[x][row - 1] = olc::BLACK;
        }

    for (int x = 0; x < 10; x++) {
        mSpace[x][0] = 0;
        mSpaceColor[x][0] = olc::BLACK;
    }
    incrementLinesCleared();
}

void Playfield::CheckForFullLines() {
    int8_t linesCleared = 0;
    for (int y = 0; y < 20; y++) {
        int count = 0;
        for (int x = 0; x < 10; x++) {
            if (mSpace[x][y] == 1)
                count++;
        }
        if (count == 10) {
            ClearLine(y);
            linesCleared++;
        }
    }

    switch (linesCleared) {
        case 1:
            increaseScore(40 * (mLevel + 1));
            break;
        case 2:
            increaseScore(100 * (mLevel + 1));
            break;
        case 3:
            increaseScore(300 * (mLevel + 1));
            break;
        case 4:
            increaseScore(1200 * (mLevel + 1));
            break;
        default:
            break;
    }
}

void Playfield::increaseScore(int32_t amount) {
    mScore += amount;
}

int32_t Playfield::getScore() {
    return mScore;
}

void Playfield::incrementLinesCleared() {
    mLinesCleared++;
    if (mLinesCleared % 10 == 0)
        incrementLevel();
}

int8_t Playfield::getLevel() {
    return mLevel;
}

void Playfield::incrementLevel() {
    mLevel++;
    mTick /= 2;
}

float Playfield::getTick() {
    return mTick;
}

int32_t Playfield::getLinesCleared() {
    return mLinesCleared;
}

