#pragma once
#include <string>
#include <mesh.h>

std::string readFile(const char* path);

GameObject* loadGameObject(std::string path, std::shared_ptr<Program> shader = DEFAULT_SHADER);