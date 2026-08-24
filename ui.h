#pragma once
#include <memory>
#include <string>
#include <vector>
#include <cy/cyMatrix.h>
#include <mesh.h>
#include <component.h>

class Mesh;
class Texture;
class Program;

class UIItem: public Transform
{
  public:
    UIItem(std::string texturePath);
    UIItem(Texture* texture);
    ~UIItem();

    Texture* texture;
};

class UI: public Component
{
  public:
    UI(std::vector<UIItem*> uiItems);
    UI();
    ~UI();

    virtual void start() override {};
    virtual void beforeUpdate();
    virtual void afterUpdate() override {};

    virtual void fakeDraw();
    virtual void addUIItem(UIItem* uiItem) { this->uiItems.push_back(uiItem);};
    virtual void draw();

  private:
    std::vector<UIItem*> uiItems;
    std::shared_ptr<Program> uiItemShader, uiShader;
    std::shared_ptr<Mesh> mesh;
    Mesh* instanceMesh;

    uint16_t depthBufferID = 0, framebufferId = 0, originalFBO = 0, VBO = 0;

    uint16_t lastW, lastH;
    bool isFirstFrame = true;

    void loadVBOData();
    void init();

    Texture* combinedTextures;
};

struct uiItemData
{
  cy::Vec4f modelMatrixRow0;
  cy::Vec4f modelMatrixRow1;
  cy::Vec4f modelMatrixRow2;
  cy::Vec4f modelMatrixRow3;
  GLuint textureIdx;
};