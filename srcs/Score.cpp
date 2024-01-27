#include "../inc/Score.hpp"

#include <iostream>
#include <stdexcept>

Score::Score(int width, int height) : _highScoreWidth(width), _highScoreHeight(height), _highScore(0), _currentScore(0)
{
    _outFile.open(HIGH_SCORE_FILE, std::ios::app);
    _outFile.close();
    _inFile.open(HIGH_SCORE_FILE, std::ios::in | std::ios::out);
    if (!_inFile)
    {
        _highScore = 9999;
        return;
    }
    readScoreFile();
}

Score::~Score()
{
    if (_currentScore > _highScore)
        updateScoreFile();
}

void Score::readScoreFile()
{
    try
    {
        std::string highScoreStr = HIGH_SCORE_PLACEHOLDER;
        size_t widthPos = highScoreStr.find("$width");
        if (widthPos == std::string::npos)
            throw std::runtime_error("find() $width");
        highScoreStr.replace(widthPos, 6, std::to_string(_highScoreWidth));
        size_t heightPos = highScoreStr.find("$height");
        if (heightPos == std::string::npos)
            throw std::runtime_error("find() $height");
        highScoreStr.replace(heightPos, 7, std::to_string(_highScoreHeight));

        std::string buffer;
        while (std::getline(_inFile, buffer))
        {
            _textHighScoreFile.append(buffer);
            _textHighScoreFile.append("\n");
        }

        _highScoreLinePos = _textHighScoreFile.find(highScoreStr);
        if (_highScoreLinePos == std::string::npos)
            return;
        std::string highScoreLine = _textHighScoreFile.substr(_highScoreLinePos);
        _highScoreEndLinePos = highScoreLine.find("\n");
        if (_highScoreEndLinePos == std::string::npos)
            throw std::runtime_error("highScoreLine no end of line");
        highScoreLine.erase(_highScoreEndLinePos);
        size_t equalPos = highScoreLine.find("=");
        if (equalPos == std::string::npos)
            throw std::runtime_error("highScoreLine no equal sign");
        std::string equal = highScoreLine.substr(equalPos);
        equal.erase(0, 1);
        _highScore = std::stoi(equal);
        _inFile.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in: Score: readScoreFile(): " << e.what() << std::endl;
    }
}

void Score::updateScoreFile()
{
    try
    {
        _outFile.open(HIGH_SCORE_FILE, std::ios::out);
        if (!_outFile)
            throw std::runtime_error("can't open outfile");

        if (_highScoreLinePos != std::string::npos)
            _textHighScoreFile.erase(_highScoreLinePos, _highScoreEndLinePos + 1);
        std::string highScoreStr = HIGH_SCORE_PLACEHOLDER;
        size_t widthPos = highScoreStr.find("$width");
        if (widthPos == std::string::npos)
            throw std::runtime_error("find $width");
        highScoreStr.replace(widthPos, 6, std::to_string(_highScoreWidth));
        size_t heightPos = highScoreStr.find("$height");
        if (heightPos == std::string::npos)
            throw std::runtime_error("find $height");
        highScoreStr.replace(heightPos, 7, std::to_string(_highScoreHeight));
        highScoreStr += "=";
        highScoreStr += std::to_string(_currentScore);
        _textHighScoreFile += highScoreStr;
        _outFile << _textHighScoreFile;
        _outFile.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in: Score: updateScoreFile(): " << e.what() << std::endl;
    }
}

void Score::setCurrentScore(int score)
{
    _currentScore = score;
}

int Score::getCurrentScore() const
{
    return _currentScore;
}

int Score::getHighScoreFile() const
{
    return _highScore;
}