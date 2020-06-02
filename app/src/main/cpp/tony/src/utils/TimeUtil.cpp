//
//  TimeUtil.cpp
//  VisualCampFaceTracker
//
//  Created by ik_visualcamp on 2018. 7. 17..
//  Copyright © 2018년 ik_visualcamp. All rights reserved.
//

#include "utils/TimeUtil.hpp"

long long int getCurrentMillis() {
  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return millis;
}
long long int getCurrentMicro() {
  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto micro = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
  return micro;
}

long long int getCurrentNanos() {
  std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
  return nanos;
}