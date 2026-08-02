#pragma once
#include <string>
#include <mesh.h>

std::string readFile(const char* path);

std::shared_ptr<Mesh> loadMesh(const char* path);

std::shared_ptr<Mesh> loadMesh(std::string path);

std::shared_ptr<Mesh> loadMesh(char* path);