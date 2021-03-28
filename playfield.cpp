//
// Created by Oliver on 27.03.21.
//

#include "playfield.h"

Playfield::Playfield(const olc::vi2d &realPosition) : mRealPos(realPosition) {

}

void Playfield::Draw(olc::PixelGameEngine *pge) {
    pge->DrawRect(mRealPos - mSize / 2, mSize);
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
