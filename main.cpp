#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include "tetrimino.h"
#include "playfield.h"

class Game : public olc::PixelGameEngine {
public:
    Game() {
        sAppName = "Tetris";
    }

public:
    bool OnUserCreate() override {
        // Called once at the start, so create things here
        srand(time(NULL));
        playfield = Playfield(olc::vi2d(ScreenWidth() / 2, ScreenHeight() / 2));
        mTet = Tetrimino::random();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        timeSinceLastTick += fElapsedTime;

        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++)
                Draw(x, y, olc::Pixel(olc::BLACK));

        if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed)
            mTet.MoveLeft(&playfield);
        if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed)
            mTet.MoveRight(&playfield);
        if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed) {
            mTet.MoveDown(&playfield);
            timeSinceLastTick = 0;
        }
        if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed)
            mTet.Flip(&playfield);
        if (GetKey(olc::Key::SPACE).bPressed)
            mTet.HardDrop(&playfield);

        playfield.Draw(this);
        mTet.Draw(this, &playfield);

        if (mTet.isInFinalPosition) {
            playfield.CheckForFullLines();
            mTet = Tetrimino::random();
        }

        if (timeSinceLastTick >= 1) {
            mTet.MoveDown(&playfield);
            timeSinceLastTick = 0;
        }

        return true;
    }

private:
    Tetrimino mTet;
    float timeSinceLastTick = 0.0f;
    Playfield playfield;
};


int main() {
    Game game;
    if (game.Construct(256, 240, 4, 4))
        game.Start();

    return 0;
}
