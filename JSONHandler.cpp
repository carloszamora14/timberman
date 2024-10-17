#include "JSONHandler.h"

// Constructor
JSONHandler::JSONHandler(const char* filename)
  : filename(filename), size(0) {
  // Initialize size to 0, will be updated after reading from the file
}

bool JSONHandler::readScores() {
  // Try to open the file in read mode
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
    return true;  // Successfully created and initialized the file
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
      scores[size].name = obj["name"].as<String>();
      scores[size].score = obj["score"];
      size++;
    }
  }

  return true;
}

// Checks if the new score qualifies to be saved
bool JSONHandler::shouldSaveScore(int newScore) {
  if (size < maxEntries) {
    return true;  // Always save if we have less than maxEntries
  }

  for (size_t i = 0; i < size; i++) {
    if (newScore > scores[i].score) {
      return true;  // Save if the new score is greater than an existing one
    }
  }

  return false;
}

// Compares the new score with existing ones and stores it if it's higher
bool JSONHandler::compareAndStore(const String& name, int newScore) {
  if (!shouldSaveScore(newScore)) {
    Serial.println("Score is not high enough to be saved");
    return false;
  }

  // Find the position for the new score
  int pos = -1;
  for (size_t i = 0; i < size; i++) {
    if (newScore > scores[i].score) {
      pos = i;
      break;
    }
  }

  // If there's an open slot or the score is higher than one of the current scores
  if (size < maxEntries) {
    pos = size;
    size++;
  }

  // Shift entries if necessary
  if (pos >= 0) {
    for (size_t i = size - 1; i > pos; i--) {
      scores[i] = scores[i - 1];
    }
    scores[pos].name = name;
    scores[pos].score = newScore;
  }

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
    obj["name"] = scores[i].name;
    obj["score"] = scores[i].score;
  }

  // Serialize the JSON document to the file
  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write JSON");
  }

  file.close();
}

// Prints the current scores to the serial monitor (for debugging)
void JSONHandler::printScores() {
  Serial.println("Current scores:");
  for (size_t i = 0; i < size; i++) {
    Serial.print("Name: ");
    Serial.print(scores[i].name);
    Serial.print(", Score: ");
    Serial.println(scores[i].score);
  }
}
