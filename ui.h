#pragma once
#include <memory>
#include <string>
#include <vector>
#include <cy/cyMatrix.h>
#include <mesh.h>
#include <component.h>
#include <queue>

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

    cy::Matrix4f lastModelMatrix;
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
    cy::Matrix4f lastModelMatrix;

    bool isFirstFrame = true;
  
    void loadVBOData();
    void init();
    void erase(UIItem* uiItem);

    bool hasChanged();
};

struct uiItemData
{
  cy::Vec4f modelMatrixRow0;
  cy::Vec4f modelMatrixRow1;
  cy::Vec4f modelMatrixRow2;
  cy::Vec4f modelMatrixRow3;
  GLuint textureIdx;
};