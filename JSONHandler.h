#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

const int maxEntries = 10;

class JSONHandler {
  public:
    JSONHandler(const char *filename);
    bool readScores();
    bool compareAndStore(const char *name, int score);
    bool shouldSaveScore(int score);
    void printScores();

    struct ScoreEntry {
        char name[5];
        int score;
    };

    ScoreEntry scores[maxEntries];
    size_t size;

  private:
    const char *filename;
    void sortScores();
    void saveScores();
};

#endif
