#include <Arduino.h>

// callback function for progress
typedef void(*OTA_progress_cb)(float);

// main entry
bool Do_OTA(String url, OTA_progress_cb cb = NULL);

