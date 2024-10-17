#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>

class JSONHandler {
public:
    JSONHandler(const char* filename);
    bool readScores();  // Reads scores from the JSON file
    bool compareAndStore(const String& name, int score);  // Compares and stores the new score
    bool shouldSaveScore(int score);  // Checks if the new score qualifies to be saved
    void printScores();  // For debugging, prints the current scores

private:
    const char* filename;
    static const int maxEntries = 4;

    struct ScoreEntry {
        String name;
        int score;
    };

    ScoreEntry scores[maxEntries];
    size_t size;

    void saveScores();  // Saves the top scores to the file
    void shiftEntries();  // Shifts the entries in the scores array when a new score is added
};

#endif
