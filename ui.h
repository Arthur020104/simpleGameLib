#pragma once
#include <mesh.h>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <component.h>

class Mesh;
class Texture;
class Program;

class UIItem: public Component
{
  public:
    UIItem(std::string texturePath);
    UIItem(Texture* texture);
    ~UIItem();

    virtual void start() {};
    virtual void beforeUpdate() {};
    virtual void afterUpdate() {};
    
    Texture* texture;

    glm::mat4 lastModelMatrix;
    Texture* lastTexture;
};

class UI: public Component
{
  public:
    UI(std::vector<UIItem*> uiItems, Scene* scene);
    UI();
    ~UI();

    virtual void start();
    virtual void beforeUpdate();
    virtual void afterUpdate();

    virtual void fakeDraw();
    virtual void addUIItem(UIItem* uiItem); 
    virtual void removeUIItem(UIItem* uiItem);
    virtual void draw();

  private:
    std::vector<UIItem*> uiItems;

    GLuint depthBufferID = 0, framebufferId = 0, originalFBO = 0, VBO = 0;

    std::shared_ptr<Program> uiItemShader, uiShader;
    std::shared_ptr<Mesh> mesh;
    Mesh* instanceMesh;

    Texture* combinedTextures;

    std::queue<UIItem*> destroyQueue;

    uint16_t lastW, lastH, lastUIItemCount;
    glm::mat4 lastModelMatrix;

    bool isFirstFrame = true;
  
    void loadVBOData();
    void init();
    void erase(UIItem* uiItem);

    bool hasChanged();
};

struct uiItemData
{
  glm::vec4 modelMatrixRow0;
  glm::vec4 modelMatrixRow1;
  glm::vec4 modelMatrixRow2;
  glm::vec4 modelMatrixRow3;
  GLuint textureIdx;
};