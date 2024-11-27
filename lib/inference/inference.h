#ifndef INFERENCE_H
#define INFERENCE_H

#include "edge-impulse-sdk/classifier/ei_classifier_types.h"

// Function declarations
void startInference();
void runEdgeImpulseInference(uint8_t *snapshot_buf);

#endif // INFERENCE_H
