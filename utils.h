#pragma once
#include <string>
#include <Libs/box3d/include/box3d/box3d.h>

std::string readFile(const char* path);

b3Vec3 b3QuatToEuler( b3Quat q );

b3Quat b3EulerToQuat( b3Vec3 euler );

uint8_t getTotalThreads();