#include "../inc/Score.hpp"

#include <iostream>
Score::Score()
{
    // Create file if does not exist
    file.open(HIGH_SCORE_FILE.c_str(), std::ios_base::out);
    if (file.fail() || file.bad() || !file.good() || !file.is_open())
    {
        std::cout << "failed to open/create file\n";
        return;
    }
    file.close();

    // Open the file in both input and read mode
    file.open(HIGH_SCORE_FILE.c_str(), std::ios_base::out | std::ios_base::in);
    if (file.fail() || file.bad() || !file.good() || !file.is_open())
    {
        std::cout << "failed to open file\n";
        return;
    }
    std::cout << "success opening   \n";
}

Score::~Score()
{
}

void Score::readScoreFile(int width, int height)
{
    std::string line;
    std::string textFromFile;
    while (std::getline(file, line))
    {
        textFromFile.append(line);
        textFromFile.append("\n");
    }
    std::cout << textFromFile;
}