#include <fstream>
#include <string>
#include <sstream>
#include <utils.h>
#include <mesh.h>
#include <cy/cyTriMesh.h>

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
