#include "inference.h"
#include "camera.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "edge-impulse-sdk/dsp/image/image.hpp"
#include "edge-impulse-sdk/classifier/ei_classifier_types.h"
#include "camera_config.h"

void startInference() {
    uint8_t *snapshot_buf = (uint8_t*)malloc(EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS * 3);
    if (snapshot_buf == nullptr) {
        Serial.println("Failed to allocate snapshot buffer.");
        return;
    }

    if (!captureImage(EI_CLASSIFIER_INPUT_WIDTH, EI_CLASSIFIER_INPUT_HEIGHT, snapshot_buf)) {
        free(snapshot_buf);
        return;
    }

    runEdgeImpulseInference(snapshot_buf);
    free(snapshot_buf);
}

void runEdgeImpulseInference(uint8_t *snapshot_buf) {
    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &getCameraData;

    ei_impulse_result_t result = {0};
    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
        Serial.println("Failed to run classifier");
        return;
    }

    // Process the result and print detected objects
    for (size_t i = 0; i < result.bounding_boxes_count; i++) {
        auto bb = result.bounding_boxes[i];
        Serial.printf("Detected object: %s, Confidence: %f\n", bb.label, bb.value);
    }
}
