#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

class FileHandler {
private:
  int chipSelect;

public:
  FileHandler(int csPin);
  bool init();
  bool writeJson(const char* filename, const JsonDocument& doc);
  bool readJson(const char* filename, JsonDocument& doc);
};

#endif
