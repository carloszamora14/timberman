#include "JSONHandler.h"

JSONHandler::JSONHandler(const char *filename)
    : filename(filename), size(0) {}

bool JSONHandler::readScores() {
  File file = SD.open(filename, FILE_READ);

  // If the file does not exist or failed to open
  if (!file) {
    Serial.println("File does not exist, creating a new one...");

    // Create a new file in write mode
    file = SD.open(filename, FILE_WRITE);
    if (!file) {
      Serial.println("Failed to create the file");
      return false;
    }

    // Initialize an empty JSON document
    StaticJsonDocument<512> doc;
    JsonArray scoresArray = doc.to<JsonArray>();

    // Write the empty JSON array to the file
    if (serializeJson(doc, file) == 0) {
      Serial.println("Failed to write empty JSON to the file");
      file.close();
      return false;
    }

    file.close();
    Serial.println("File created successfully");
    return true; // Successfully created and initialized the file
  }

  // Continue reading if the file exists
  StaticJsonDocument<512> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Failed to parse JSON");
    return false;
  }

  // Parse the scores array from the JSON file
  JsonArray scoresArray = doc.as<JsonArray>();
  size = 0;

  // Load scores from JSON
  for (JsonObject obj : scoresArray) {
    if (size < maxEntries) {
      // Use strcpy to copy the name from the JSON object to the char array
      strncpy(scores[size].name, obj["name"].as<const char *>(), sizeof(scores[size].name) - 1);
      scores[size].name[sizeof(scores[size].name) - 1] = '\0'; // Null-terminate
      scores[size].score = obj["score"];
      size++;
    }
  }

  return true;
}

// Checks if the new score qualifies to be saved
bool JSONHandler::shouldSaveScore(int newScore) {
  if (size < maxEntries) {
    return true; // Always save if we have less than maxEntries
  }

  for (size_t i = 0; i < size; i++) {
    if (newScore > scores[i].score) {
      return true; // Save if the new score is greater than an existing one
    }
  }

  return false;
}

void JSONHandler::sortScores() {
  // Bubble sort to sort scores in descending order
  for (size_t i = 0; i < size - 1; i++) {
    for (size_t j = 0; j < size - i - 1; j++) {
      if (scores[j].score < scores[j + 1].score) {
        // Swap the scores if they are out of order
        ScoreEntry temp = scores[j];
        scores[j] = scores[j + 1];
        scores[j + 1] = temp;
      }
    }
  }
}

bool JSONHandler::compareAndStore(const char *name, int newScore) {
  if (!shouldSaveScore(newScore)) {
    Serial.println("Score is not high enough to be saved");
    return false;
  }

  // Find the position for the new score or add it to the end
  if (size < maxEntries) {
    // Add the new score to the next available slot
    strncpy(scores[size].name, name, sizeof(scores[size].name) - 1);
    scores[size].name[sizeof(scores[size].name) - 1] = '\0'; // Null-terminate
    scores[size].score = newScore;
    size++;
  } else {
    // Replace the lowest score if the new one is higher
    for (size_t i = 0; i < size; i++) {
      if (newScore > scores[i].score) {
        // Shift entries down to make room for the new score
        for (size_t j = size - 1; j > i; j--) {
          scores[j] = scores[j - 1];
        }
        // Insert the new score at the correct position
        strncpy(scores[i].name, name, sizeof(scores[i].name) - 1);
        scores[i].name[sizeof(scores[i].name) - 1] = '\0'; // Null-terminate
        scores[i].score = newScore;
        break;
      }
    }
  }

  sortScores();
  saveScores();
  return true;
}

// Saves the scores back to the JSON file
void JSONHandler::saveScores() {
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  StaticJsonDocument<512> doc;
  JsonArray scoresArray = doc.to<JsonArray>();

  // Populate the JSON array with scores
  for (size_t i = 0; i < size; i++) {
    JsonObject obj = scoresArray.createNestedObject();
    obj["name"] = scores[i].name; // Store the name from char array
    obj["score"] = scores[i].score;
  }

  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write JSON");
  }

  file.close();
}

void JSONHandler::printScores() {
  Serial.println("Current scores:");
  for (size_t i = 0; i < size; i++) {
    Serial.print("Name: ");
    Serial.print(scores[i].name);
    Serial.print(", Score: ");
    Serial.println(scores[i].score);
  }
}
