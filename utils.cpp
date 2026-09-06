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
  file.close();

  return fileContents;
}

uint8_t getTotalThreads()
{
  uint8_t totalThreads = std::thread::hardware_concurrency();
  return std::max<uint8_t>(1, totalThreads);
}