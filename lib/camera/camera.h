#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include "esp_camera.h"

// Function declarations
bool initializeCamera();
void deinitializeCamera();
bool captureImage(uint32_t img_width, uint32_t img_height, uint8_t *out_buf);
int getCameraData(size_t offset, size_t length, float *out_ptr);

#endif // CAMERA_H
    