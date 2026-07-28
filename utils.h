#pragma once
#include <string>
#include <mesh.h>

std::string readFile(const char* path);

Mesh* loadMesh(const char* path);

Mesh* loadMesh(std::string path);

Mesh* loadObj(char* path);