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

// GameObject* loadGameObject(std::string path, std::shared_ptr<Program> shader)
// {
//   cy::TriMesh objTriMesh;
//   if(!objTriMesh.LoadFromFileObj(path.c_str(), true, &std::cout)) std::cerr << "Failed to load Obj from file. Path: '"<< path << "'.\n";

//   std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(objTriMesh);

//   std::vector<uint8_t> materialIndices;
//   materialIndices.reserve(mesh->getVerticesAmount());

//   for(uint32_t i = 0; i < objTriMesh.NF(); i++)
//   {
//     int materialIdx = objTriMesh.GetMaterialIndex(i);

//     if (materialIdx < 0) materialIdx = 0;

//     materialIndices.insert(materialIndices.end(), 3, (uint8_t)materialIdx);
//   }

//   std::vector<std::shared_ptr<Material>> materials;

//   for(uint8_t i = 0; i < objTriMesh.NM(); i++)
//   { 
//     cy::TriMesh::Mtl material = objTriMesh.M(i);

//     cy::Vec3f diffuseColor = cy::Vec3f(material.Kd);
//     cy::Vec3f specularColor = cy::Vec3f(material.Ks);
//     // cy::Vec3f ka = cy::Vec3f(material.Ka);
      
//     float shininess = material.Ns;
    
//     materials.push_back(std::make_shared<Material>(diffuseColor, specularColor, shininess));

//     if(material.map_Kd != nullptr)
//       materials[i]->addDiffuseTexture(material.map_Kd.data);

//     if(material.map_Ks != nullptr)
//       materials[i]->addSpecularTexture(material.map_Ks.data);

//     if(material.map_Ke != nullptr)
//       materials[i]->addEmissiveTexture(material.map_Ke.data);
//   }
  
//   /*
//     Str   name;		//!< Material name
// 		float Ka[3];	//!< Ambient color
// 		float Kd[3];	//!< Diffuse color
// 		float Ks[3];	//!< Specular color
// 		float Tf[3];	//!< Transmission color
// 		float Ns;		//!< Specular exponent
// 		float Ni;		//!< Index of refraction
// 		int   illum;	//!< Illumination model
// 		Str   map_Ka;	//!< Ambient color texture map
// 		Str   map_Kd;	//!< Diffuse color texture map
// 		Str   map_Ks;	//!< Specular color texture map
// 		Str   map_Ns;	//!< Specular exponent texture map
// 		Str   map_d;	//!< Alpha texture map
// 		Str   map_bump;	//!< Bump texture map
// 		Str   map_disp;	//!< Displacement texture map*/

//   GameObject* object = new GameObject(mesh, shader, materials);
//   object->setMaterialIndices(materialIndices);
//   return object;
// }