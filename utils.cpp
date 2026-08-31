#include <utils.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <thread>
#include <algorithm>

std::string readFile(const char* path)
{
  std::ifstream file(path);
  std::string lineStr;
  std::string fileContents;

  while (std::getline(file, lineStr))
  {
    fileContents += lineStr;
    fileContents.push_back('\n');
  }

  return fileContents;
}

b3Vec3 b3QuatToEuler( b3Quat q )
{
  float x = q.v.x;
  float y = q.v.y;
  float z = q.v.z;
  float w = q.s;

  b3Vec3 euler;

  float sinrCosp = 2.0f * (w * x + y * z);
  float cosrCosp = 1.0f - 2.0f * (x * x + y * y);
  euler.x = b3Atan2( sinrCosp, cosrCosp );

  float sinp = 2.0f * (w * y - z * x);
  sinp = b3ClampFloat( sinp, -1.0f, 1.0f );
  euler.y = asinf( sinp );

  float sinyCosp = 2.0f * (w * z + x * y);
  float cosyCosp = 1.0f - 2.0f * (y * y + z * z);
  euler.z = b3Atan2( sinyCosp, cosyCosp );

  return euler;
}

b3Quat b3EulerToQuat( b3Vec3 euler )
{
	b3CosSin csX = b3ComputeCosSin( 0.5f * euler.x );
	b3CosSin csY = b3ComputeCosSin( 0.5f * euler.y );
	b3CosSin csZ = b3ComputeCosSin( 0.5f * euler.z );

	float cx = csX.cosine, sx = csX.sine;
	float cy = csY.cosine, sy = csY.sine;
	float cz = csZ.cosine, sz = csZ.sine;

	b3Quat q;
	q.v.x = sx * cy * cz + cx * sy * sz;
	q.v.y = cx * sy * cz - sx * cy * sz;
	q.v.z = cx * cy * sz + sx * sy * cz;
	q.s   = cx * cy * cz - sx * sy * sz;

	return q;
}

uint8_t getTotalThreads()
{
  uint8_t totalThreads = std::thread::hardware_concurrency();
  return std::max<uint8_t>(1, totalThreads);
}