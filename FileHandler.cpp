#include "FileHandler.h"

FileHandler::FileHandler(int csPin)
    : chipSelect(csPin) {}

bool FileHandler::init() {
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return false;
  }

  Serial.println("Card initialized.");
  return true;
}

bool FileHandler::writeJson(const char *filename, const JsonDocument &doc) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    serializeJson(doc, dataFile);
    dataFile.close();

    return true;
  } else {
    Serial.println("Error opening file for writing.");
    return false;
  }
}

bool FileHandler::readJson(const char *filename, JsonDocument &doc) {
  File dataFile = SD.open(filename);
  if (dataFile) {
    String jsonData = "";

    while (dataFile.available()) {
      jsonData += char(dataFile.read());
    }
    dataFile.close();

    DeserializationError error = deserializeJson(doc, jsonData);
    if (error) {
      Serial.print("Deserialization failed: ");
      Serial.println(error.f_str());
      return false;
    }

    return true;
  } else {
    Serial.println("Error opening file for reading.");
    return false;
  }
}

// void FileHandler::updateScores(const char *name, int score) {

//     StaticJsonDocument<256> doc;
//     JsonArray array = doc.to<JsonArray>();
//     String jsonData = "";

//     bool reorderNeeded = false;
//     int index = -1;
//     for (jsonObject obj : array) {
//       if (score > obj["score"]) {
//         reorderNeeded = true;
//         break;
//       }
//     }

//     char *previousName;
//     int previousScore;
//     for (int i = 1; i <= 10; i++) {
//       JsonObject obj = array.createNestedObject();

//       if (score > obj["score"]) {
//         reorderNeeded = true;
//         break;
//       }
//     }

//     sdHandler.writeJson("scores.json", doc);
// }
