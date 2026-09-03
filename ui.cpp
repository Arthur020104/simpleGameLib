#include <ui.h>
#include <window.h>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

UIItem::UIItem(std::string texturePath)
{
  this->texture = new Texture(texturePath);
  this->lastTexture = this->texture;
  this->lastModelMatrix = this->getModelMatrix();
}

UIItem::UIItem(Texture* texture)
{
  this->texture = texture;
  this->lastTexture = this->texture;
  this->lastModelMatrix = this->getModelMatrix();
}

UIItem::~UIItem()
{
  delete this->texture;
}
UI::~UI()
{
  for(UIItem* item: this->uiItems)
  {
    delete item;
  }

  delete this->combinedTextures;
  delete this->instanceMesh;
}

UI::UI(std::vector<UIItem*> uiItems, Scene* scene): Component(), uiItems(uiItems)
{
  this->scene = scene;

  for(UIItem* item: this->uiItems)
  {
    item->scene = scene;
  }

  init();
}

UI::UI(): Component()
{
  init();
}

void UI::addUIItem(UIItem* uiItem)
{
  this->uiItems.push_back(uiItem);
  uiItem->scene = this->scene;
}

void UI::removeUIItem(UIItem* uiItem)
{
  this->destroyQueue.push(uiItem);
}

void UI::erase(UIItem* uiItem)
{
  auto itemOnUIItems = std::find(this->uiItems.begin(), this->uiItems.end(), uiItem);
  if(itemOnUIItems == this->uiItems.end()) return;
  
  this->uiItems.erase(itemOnUIItems);
  delete uiItem;
}

void UI::init()
{
  this->uiItemShader = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/uiItem.vs", "/home/arthur/Documents/simpleGame/shaders/uiItem.fs");
  this->uiShader = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/ui.vs", "/home/arthur/Documents/simpleGame/shaders/ui.fs");

  this->mesh = Mesh::getQuadMesh();

  this->instanceMesh = new Mesh(this->mesh->getVertices(), MeshType::TRIANGLE_MESH);

  this->combinedTextures = new Texture(WINDOW.w, WINDOW.h, GL_RGBA, nullptr);

  this->lastW = WINDOW.w;
  this->lastH = WINDOW.h;
  this->lastUIItemCount = this->uiItems.size();
  this->lastModelMatrix = this->getModelMatrix();

  glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, (GLint*)&originalFBO );
  
  GLuint depthBufferID, framebufferId;

  //Create the frame buffer
  glGenFramebuffers(1, &framebufferId);
  glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

  //Create depth buffer
  glGenRenderbuffers(1, &depthBufferID);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, WINDOW.w, WINDOW.h);

  //Config frame buffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->combinedTextures->getId(), 0);

  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);
  
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw std::runtime_error("Framebuffer not complete");
  
  this->depthBufferID = depthBufferID;
  this->framebufferId = framebufferId;

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, originalFBO);
}

void UI::fakeDraw()
{
  if (this->uiItems.empty()) return;

  this->updateLastData();

  GLint currentFBO = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);

  glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferId);
  glViewport(0, 0, WINDOW.w, WINDOW.h);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const uint16_t shaderID = this->uiItemShader->getProgram();
  glUseProgram(shaderID);

  glm::mat4 projection = glm::ortho(-WINDOW.aspect, WINDOW.aspect, -1.0f, 1.0f, -1.0f, 1.0f);
  this->uiItemShader->bindMat4("projection", projection);

  this->loadVBOData(); 
  this->instanceMesh->bindVAO();

  glDrawArraysInstanced(GL_TRIANGLES, 0, this->instanceMesh->getVerticesAmount(), this->uiItems.size()); 
  
  glBindTexture(GL_TEXTURE_2D, this->combinedTextures->getId());
  glGenerateMipmap(GL_TEXTURE_2D); 
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, currentFBO);
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindVertexArray(0);
  glUseProgram(0);
}

void UI::draw()
{
  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  this->mesh->bindVAO();

  const uint16_t shaderID = this->uiShader->getProgram();
  glUseProgram(shaderID);

  this->combinedTextures->bind(this->uiShader.get(), "textureUnit", 0);
  this->uiShader->bindMat4("modelMatrix", this->getModelMatrix());

  this->mesh->renderMesh();

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glUseProgram(0);
}

bool UI::hasChanged()
{
  if(this->isFirstFrame)
    return true;

  if(this->lastW != WINDOW.w || this->lastH != WINDOW.h) 
    return true;

  if(this->lastModelMatrix != this->getModelMatrix())
    return true;

  if(this->lastUIItemCount != this->uiItems.size()) 
    return true;

  for(UIItem* item: this->uiItems)
  {
    if(item->lastModelMatrix != item->getModelMatrix()) 
      return true;
    if(item->lastTexture != item->texture)
      return true;
  }

  return false;
}

void UI::updateLastData()
{
  this->lastW = WINDOW.w; this->lastH = WINDOW.h;
  this->lastModelMatrix = this->getModelMatrix();
  this->lastUIItemCount = this->uiItems.size();

  for(UIItem* item: this->uiItems)
  {
    item->lastModelMatrix = item->getModelMatrix();
    item->lastTexture = item->texture;
  }
}

void UI::beforeUpdate()
{
  for(UIItem* item: this->uiItems)
  {
    item->beforeUpdate();
  }

  if(WINDOW.w != this->lastW || WINDOW.h != this->lastH)
  {
    this->combinedTextures->setResolution(WINDOW.w, WINDOW.h);

    glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferId);

    glBindRenderbuffer(GL_RENDERBUFFER, this->depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, WINDOW.w, WINDOW.h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->combinedTextures->getId(), 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      std::cout << "UI: framebuffer incomplete after resize!" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, this->originalFBO);
  }

  if(this->hasChanged())
  {
    this->fakeDraw();
  }
}

void UI::start()
{
  for(UIItem* item: this->uiItems)
  {
    item->start();
  }
}

void UI::afterUpdate()
{
  for(UIItem* item: this->uiItems)
  {
    item->afterUpdate();
  }

  while(!this->destroyQueue.empty())
  {
    UIItem* item = this->destroyQueue.front();
    this->destroyQueue.pop();
    erase(item);
  }
}

void UI::loadVBOData()
{
  this->instanceMesh->bindVAO();

  if (this->isFirstFrame)
  {
    glGenBuffers(1, (GLuint*)&VBO);
    this->isFirstFrame = false;  
  }
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  uint16_t textureCounter = 0;
  std::vector<uiItemData> gpuData;
  gpuData.reserve(this->uiItems.size());

  for (UIItem* item : this->uiItems)
  {
    uiItemData data;
    data.modelMatrixRow0 = glm::row(item->getModelMatrix(), 0);
    data.modelMatrixRow1 = glm::row(item->getModelMatrix(), 1);
    data.modelMatrixRow2 = glm::row(item->getModelMatrix(), 2);
    data.modelMatrixRow3 = glm::row(item->getModelMatrix(), 3);

    data.textureIdx = textureCounter;

    gpuData.push_back(data);

    item->texture->bind(this->uiItemShader.get(), textureCounter);
  
    textureCounter++;
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(uiItemData) * gpuData.size(), gpuData.data(), GL_STATIC_DRAW);

  std::size_t vec4Size = sizeof(glm::vec4);
  
  for(uint8_t i = 0; i < 4; i++)
  {
    glEnableVertexAttribArray(4 + i);
    glVertexAttribPointer(
      4 + i,
      4,
      GL_FLOAT,
      GL_FALSE,
      sizeof(uiItemData),
      (void*)(i * vec4Size)
    );
    glVertexAttribDivisor(4 + i, 1);
  }

  glEnableVertexAttribArray(8);
  glVertexAttribIPointer(
    8,
    1,
    GL_UNSIGNED_INT,
    sizeof(uiItemData),
    (void*)offsetof(uiItemData, textureIdx)
  );
  glVertexAttribDivisor(8, 1);
}