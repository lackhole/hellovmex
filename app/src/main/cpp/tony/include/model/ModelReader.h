//
// Created by elon on 2020-01-16.
//

#ifndef VMEX_NEW_VMEX_SRC_MAIN_CPP_COMMON_DETECTOR_MODELREADER_H_
#define VMEX_NEW_VMEX_SRC_MAIN_CPP_COMMON_DETECTOR_MODELREADER_H_


class ModelReader {
 public:
  
  typedef char* buffer_type;
  
  struct ModelData{
      buffer_type byte;
      unsigned int size;
  };
  
  static ModelData ReadFlippedGaze();
  static ModelData ReadGenerator();
  static ModelData ReadRegressor();
  
};

#endif //VMEX_NEW_VMEX_SRC_MAIN_CPP_COMMON_DETECTOR_MODELREADER_H_
