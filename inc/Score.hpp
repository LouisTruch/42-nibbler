#pragma once

#include <fstream>
#include <string_view>

class Score
{
  public:
    Score();
    ~Score();
    void readScoreFile(int, int);

  private:
    std::fstream file;
    inline static const std::string HIGH_SCORE_FILE = {"highscore.txt"};
};