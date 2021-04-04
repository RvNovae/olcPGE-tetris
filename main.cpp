#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include "tetrimino.h"
#include "userAction.h"


enum class Scene {
    START, GAME, END
};

class Game : public olc::PixelGameEngine {
public:
    Game() {
        sAppName = "Tetris";
    }

public:
    bool OnUserCreate() override {
        // Called once at the start, so create things here
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // called once per frame
        timeSinceLastTick += fElapsedTime;

        Clear(olc::BLACK);

        switch (scene) {
            case Scene::GAME: {
                if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed)
                    mTet.Flip(&playfield);
                if (GetKey(olc::Key::SPACE).bPressed)
                    mTet.HardDrop(&playfield);
                if (GetKey(olc::R).bReleased)
                    StartGame();

                for (auto const&[key, val] : buttonAction) {
                    if (GetKey(key).bHeld || GetKey(key).bPressed || GetKey(key).bReleased)
                        PrepareUserAction(GetKey(key), val, fElapsedTime);
                }

                if (timeSinceLastTick >= playfield.getTick()) {
                    mTet.MoveDown(&playfield, false);
                    timeSinceLastTick -= playfield.getTick();
                }

                playfield.Draw(this);
                mTet.Draw(this, &playfield);
                mTet.DrawGhost(this, &playfield);

                std::string sScore = "Score: " + std::to_string(playfield.getScore());
                std::string sLevel = "Level: " + std::to_string(playfield.getLevel());
                std::string sLC = "LC:    " + std::to_string(playfield.getLinesCleared());
                DrawString(olc::vi2d(10, 20), sScore);
                DrawString(olc::vi2d(10, 35), sLevel);
                DrawString(olc::vi2d(10, 50), sLC);
                DrawString({10, 70}, "Next:");
                mTetNext.Draw(this, &playfield, {10, 80});

                if (mTet.isInFinalPosition) {
                    for (int i = 0; i < 10; i++)
                        if (playfield.IsOccupied({i, 1}))
                            GameOver();

                    playfield.CheckForFullLines();
                    mTet = mTetNext;
                    mTetNext = Tetrimino::random();
                }
                break;
            }
            case Scene::START: {
                DrawString({(ScreenWidth() / 2) - 20, (ScreenHeight() / 2) - 7}, "START");
                if (GetKey(olc::ENTER).bPressed) StartGame();
                break;
            }

            case Scene::END: {
                DrawString({(ScreenWidth() / 2) - 20, (ScreenHeight() / 2) - 7}, "GAMEOVER");
                DrawString({(ScreenWidth() / 2) - 20, (ScreenHeight() / 2) + 13}, "SCORE: " + std::to_string(playfield.getScore()));
                DrawString({25, ScreenHeight() / 2 + 30}, "Press Enter to play again.");

                if (GetKey(olc::ENTER).bPressed) StartGame();

                break;
            }
        }

        return true;
    }

    void StartGame() {
        scene = Scene::GAME;
        srand(time(NULL));
        playfield = Playfield(olc::vi2d((ScreenWidth() / 2) + 25, ScreenHeight() / 2));
        mTet = Tetrimino::random();
        mTetNext = Tetrimino::random();
    }

    void GameOver() {
        scene = Scene::END;
    }

    void PrepareUserAction(olc::HWButton button, UserAction action, float fElapsedTime) {
        if (button.bReleased)
            buttonHeldTime[action] = 0.0f;

        if (button.bPressed)
            PerformUserAction(action);

        if (button.bHeld) {
            if (buttonHeldTime[action] >= (mMoveSpeed - (float(playfield.getLevel()) * mMoveSpeed / 4))) {
                PerformUserAction(action);
                buttonHeldTime[action] = 0.0f;
            } else
                buttonHeldTime[action] += fElapsedTime;
        }
    }

    void PerformUserAction(UserAction action) {
        switch (action) {
            case UserAction::LEFT :
                mTet.MoveLeft(&playfield);
                break;
            case UserAction::RIGHT :
                mTet.MoveRight(&playfield);
                break;
            case UserAction::DOWN:
                mTet.MoveDown(&playfield);
        }
    }

private:
    Tetrimino mTet;
    Tetrimino mTetNext;
    float timeSinceLastTick = 0.0f;
    float mMoveSpeed = 0.15f;
    std::map<UserAction, float> buttonHeldTime = {{UserAction::LEFT,  0.0f},
                                                  {UserAction::RIGHT, 0.0f},
                                                  {UserAction::DOWN,  0.0f}};
    std::map<olc::Key, UserAction> buttonAction = {{olc::Key::A,     UserAction::LEFT},
                                                   {olc::Key::LEFT,  UserAction::LEFT},
                                                   {olc::Key::D,     UserAction::RIGHT},
                                                   {olc::Key::RIGHT, UserAction::RIGHT},
                                                   {olc::Key::S,     UserAction::DOWN},
                                                   {olc::Key::DOWN,  UserAction::DOWN}};
    Playfield playfield;
    Scene scene = Scene::START;
};

int main() {
    Game game;
    if (game.Construct(256, 240, 4, 4))
        game.Start();

    return 0;
}
