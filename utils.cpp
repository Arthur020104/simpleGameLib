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

std::shared_ptr<Mesh> loadMesh(std::string path)
{
  return loadMesh(path.c_str());
}

std::shared_ptr<Mesh> loadMesh(const char* path)
{
  cy::TriMesh objTriMesh;
  if(!objTriMesh.LoadFromFileObj(path, true, &std::cout)) std::cerr << "Failed to load Obj from file. Path: '"<< path << "'.\n";

  if(!objTriMesh.HasNormals()) objTriMesh.ComputeNormals();

  if(!objTriMesh.IsBoundBoxReady()) objTriMesh.ComputeBoundingBox();

  std::vector<Vertex> meshData;

  for(uint32_t i = 0; i < objTriMesh.NF(); i++)
  {
    const cy::TriMesh::TriFace faceVerticesPos = objTriMesh.F(i);
    const cy::TriMesh::TriFace faceVerticesNormals = objTriMesh.FN(i);

    cy::TriMesh::TriFace faceVerticesUv;

    if(objTriMesh.HasTextureVertices()) faceVerticesUv = objTriMesh.FT(i);

    for(uint8_t j = 0; j < 3; j++)
    {
      Vertex vertex;

      vertex.pos = objTriMesh.V(faceVerticesPos.v[j]);

      vertex.normal = objTriMesh.VN(faceVerticesNormals.v[j]);

      vertex.uv = objTriMesh.HasTextureVertices() ? 
                  objTriMesh.VT(faceVerticesUv.v[j]).XY() : 
                  cy::Vec2f(0.0f, 0.0f);

      meshData.push_back(vertex);
    }
  }
  cy::Vec3f bbox[2] = {objTriMesh.boundMin, objTriMesh.boundMax};
  return std::make_shared<Mesh>(meshData, bbox);
}

std::shared_ptr<Mesh> loadMesh(char* path)
{
  return loadMesh((const char*)path);
}