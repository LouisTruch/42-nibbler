#pragma once

#include <fstream>
#include <string>

class Score
{
  public:
    Score(int, int);
    ~Score();
    void readScoreFile();
    void updateScoreFile();
    void setCurrentScore(size_t);
    int getCurrentScore() const;
    int getHighScore() const;

  private:
    std::ifstream _inFile;
    std::ofstream _outFile;
    int _highScoreWidth;
    int _highScoreHeight;
    int _highScore;
    int _currentScore;

    size_t _highScoreLinePos;
    size_t _highScoreEndLinePos;
    std::string _textHighScoreFile;

    inline static const std::string HIGH_SCORE_FILE = {"highscore.txt"};
    inline static const std::string HIGH_SCORE_PLACEHOLDER = {"w:$width h:$height"};
};