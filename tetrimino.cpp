//
// Created by Oliver on 27.03.21.
//

#include "tetrimino.h"

Tetrimino::Tetrimino(TetriminoType type) : mType(type) {

    switch (mType) {
        case TetriminoType::I: {
            mShape[0][2] = 1;
            mShape[1][2] = 1;
            mShape[2][2] = 1;
            mShape[3][2] = 1;
            mColor = olc::Pixel(0, 188, 212);

        }
            break;
        case TetriminoType::O: {
            mShape[1][1] = 1;
            mShape[2][1] = 1;
            mShape[1][2] = 1;
            mShape[2][2] = 1;
            mColor = olc::Pixel(255, 235, 59);
        }
            break;
        case TetriminoType::T: {
            mShape[2][1] = 1;
            mShape[1][2] = 1;
            mShape[2][2] = 1;
            mShape[3][2] = 1;
            mColor = olc::Pixel(103, 58, 183);
        }
            break;
        case TetriminoType::S: {
            mShape[2][1] = 1;
            mShape[3][1] = 1;
            mShape[2][2] = 1;
            mShape[1][2] = 1;
            mColor = olc::Pixel(76, 175, 80);
        }
            break;
        case TetriminoType::Z: {
            mShape[1][1] = 1;
            mShape[2][1] = 1;
            mShape[2][2] = 1;
            mShape[3][2] = 1;
            mColor = olc::Pixel(244, 67, 54);
        }
            break;
        case TetriminoType::J: {
            mShape[1][1] = 1;
            mShape[1][2] = 1;
            mShape[2][2] = 1;
            mShape[3][2] = 1;
            mColor = olc::Pixel(33, 150, 243);
        }
            break;
        case TetriminoType::L: {
            mShape[3][1] = 1;
            mShape[1][2] = 1;
            mShape[2][2] = 1;
            mShape[3][2] = 1;
            mColor = olc::Pixel(255, 152, 0);
        }
            break;
    }
}

Tetrimino Tetrimino::random() {
    auto type = static_cast<TetriminoType>(rand() % typeCount);

    return Tetrimino(type);
}

void Tetrimino::Draw(olc::PixelGameEngine *pge, Playfield *playfield) {
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            if (mShape[x][y] == 1)
                pge->FillRect(playfield->BlockToReal(mBlockPosition + olc::vi2d(x - 2, y - 2)),
                              olc::vi2d(10, 10), mColor);

}

void Tetrimino::Draw(olc::PixelGameEngine *pge, Playfield *playfield, const olc::vi2d &pos) {
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            if (mShape[x][y] == 1)
                pge->FillRect(pos + olc::vi2d(x * 10, y * 10), olc::vi2d(10, 10), mColor);
}


void Tetrimino::MoveLeft(Playfield *playfield) {
    auto newPosition = mBlockPosition + olc::vi2d(-1, 0);
    if (canMoveToPosition(playfield, newPosition))
        mBlockPosition = newPosition;
}

void Tetrimino::MoveRight(Playfield *playfield) {
    auto newPosition = mBlockPosition + olc::vi2d(1, 0);
    if (canMoveToPosition(playfield, newPosition))
        mBlockPosition = newPosition;
}

void Tetrimino::MoveDown(Playfield *playfield, bool userTriggered, bool dry) {
    auto newPosition = mBlockPosition + olc::vi2d(0, 1);
    if (canMoveToPosition(playfield, newPosition)) {
        mBlockPosition = newPosition;
        if (userTriggered && !dry)
            playfield->increaseScore(1);
        return;
    }
    isInFinalPosition = true;

    if (dry) return;

    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            if (mShape[x][y] == 1)
                playfield->FillBlock(mBlockPosition + olc::vi2d(x - 2, y - 2), mColor);
}

void Tetrimino::Flip(Playfield *playfield) {
    std::array<std::array<int8_t, 5>, 5> originalShape = mShape;
    // Consider all squares one by one
    for (int x = 0; x < 5 / 2; x++) {
        // Consider elements in group of 4 in current square
        for (int y = x; y < 5 - x - 1; y++) {
            // Store current cell in temp variable
            int8_t temp = mShape[x][y];
            // Move values from right to top
            mShape[x][y] = mShape[y][5 - 1 - x];
            // Move values from bottom to right
            mShape[y][5 - 1 - x] = mShape[5 - 1 - x][5 - 1 - y];
            // Move values from left to bottom
            mShape[5 - 1 - x][5 - 1 - y] = mShape[5 - 1 - y][x];
            // Assign temp to left
            mShape[5 - 1 - y][x] = temp;
        }
    }

    if (!canMoveToPosition(playfield, mBlockPosition))
        mShape = originalShape;
}

bool Tetrimino::canMoveToPosition(Playfield *playfield, const olc::vi2d &newPosition) {
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            if (mShape[x][y] == 1) {
                auto blockPosition = newPosition + olc::vi2d(x - 2, y - 2);
                if (blockPosition.y < 0) return true;
                if (blockPosition.x < 0 || blockPosition.x >= 10 || blockPosition.y >= 20)
                    return false;
                if (playfield->IsOccupied(blockPosition))
                    return false;
            }
    return true;
}

void Tetrimino::HardDrop(Playfield *playfield) {
    while (!isInFinalPosition) {
        MoveDown(playfield);
    }
}

void Tetrimino::DrawGhost(olc::PixelGameEngine *pge, Playfield *playfield) {
    if (isInFinalPosition) return;
    olc::vi2d currentPos = mBlockPosition;

    while (!isInFinalPosition) {
        MoveDown(playfield, false, true);
    }

    olc::vi2d finalPos = mBlockPosition;
    mBlockPosition = currentPos;
    isInFinalPosition = false;

    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
            if (mShape[x][y] == 1)
                pge->DrawRect(playfield->BlockToReal(finalPos + olc::vi2d(x - 2, y - 2)),
                              olc::vi2d(10, 10));
}
