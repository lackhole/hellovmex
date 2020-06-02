//
// Created by elon on 2020-01-16.
//

#include "model/ModelReader.h"

#include "model/ld_generator.h"
#include "model/ld_regressor.h"
#include "model/gaze_flip_f16.h"


ModelReader::ModelData ModelReader::ReadFlippedGaze() {
  return {(buffer_type)gaze_flip_f16_tflite, gaze_flip_f16_tflite_len};
}


ModelReader::ModelData ModelReader::ReadGenerator() {
  return {(buffer_type)conv_generator_tflite, conv_generator_tflite_len};
}





ModelReader::ModelData ModelReader::ReadRegressor() {
  return {(buffer_type)conv_regressor_re_tflite, conv_regressor_re_tflite_len};
}