#include "camera.h"
#include <esp_camera.h>
#include "edge-impulse-sdk/dsp/image/image.hpp"
#include "edge-impulse-sdk/classifier/ei_classifier_types.h"
#include "camera_config.h" 

// Private variables
static bool is_initialized = false;
static uint8_t* snapshot_buf;

static camera_config_t camera_config = {
    .pin_pwdn = -1,
    .pin_reset = -1,
    .pin_xclk = 21,
    .pin_sscb_sda = 26,
    .pin_sscb_scl = 27,
    .pin_d7 = 35,
    .pin_d6 = 34,
    .pin_d5 = 39,
    .pin_d4 = 36,
    .pin_d3 = 19,
    .pin_d2 = 18,
    .pin_d1 = 5,
    .pin_d0 = 4,
    .pin_vsync = 25,
    .pin_href = 23,
    .pin_pclk = 22,
    .xclk_freq_hz = 20000000,
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 12,
    .fb_count = 1,
    .fb_location = CAMERA_FB_IN_PSRAM,
};

bool initializeCamera() {
    if (is_initialized) return true;

    // Power up the camera if PWDN pin is defined
    if (camera_config.pin_pwdn != -1) {
        pinMode(camera_config.pin_pwdn, OUTPUT);
        digitalWrite(camera_config.pin_pwdn, LOW);
        delay(10);
    }

    // Reset the camera if RESET pin is defined
    if (camera_config.pin_reset != -1) {
        pinMode(camera_config.pin_reset, OUTPUT);
        digitalWrite(camera_config.pin_reset, LOW);
        delay(10);
        digitalWrite(camera_config.pin_reset, HIGH);
        delay(10);
    }

    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        // Additional error handling based on error code
        if (err == 0x103) {
            Serial.println("Failed to set frame size. Check camera configuration.");
        }
        return false;
    }

    is_initialized = true;
    return true;
}

void deinitializeCamera() {
    esp_camera_deinit();
    is_initialized = false;
}

bool captureImage(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return false;
    }

    // Convert captured image to RGB888 format (you might need additional conversion logic here)
    bool converted = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, snapshot_buf);
    esp_camera_fb_return(fb);

    if (!converted) {
        Serial.println("Conversion failed");
        return false;
    }

    // Resize if necessary
    if (img_width != EI_CAMERA_RAW_FRAME_BUFFER_COLS || img_height != EI_CAMERA_RAW_FRAME_BUFFER_ROWS) {
        // Perform resizing or cropping (implement this as needed)
    }

    return true;
}

int getCameraData(size_t offset, size_t length, float *out_ptr) {
    size_t pixel_ix = offset * 3;
    size_t pixels_left = length;
    size_t out_ptr_ix = 0;

    while (pixels_left > 0) {
        out_ptr[out_ptr_ix] = (snapshot_buf[pixel_ix + 2] << 16) + (snapshot_buf[pixel_ix + 1] << 8) + snapshot_buf[pixel_ix];
        out_ptr_ix++;
        pixel_ix += 3;
        pixels_left--;
    }
    return 0;
}
