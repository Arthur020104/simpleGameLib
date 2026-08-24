#include <ui.h>
#include <window.h>

UIItem::UIItem(std::string texturePath)
{
  this->texture = new Texture(texturePath);
}

UIItem::UIItem(Texture* texture)
{
  this->texture = texture;
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

UI::UI(std::vector<UIItem*> uiItems): Component(), uiItems(uiItems)
{
  init();
}

UI::UI(): Component()
{
  init();
}

void UI::init()
{
  this->lastW = WINDOW.w;
  this->lastH = WINDOW.h;

  this->uiItemShader = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/ui copy.vs", "/home/arthur/Documents/simpleGame/shaders/ui copy.fs");
  this->uiShader = std::make_shared<Program>("/home/arthur/Documents/simpleGame/shaders/ui.vs", "/home/arthur/Documents/simpleGame/shaders/ui.fs");

  this->mesh = Mesh::getQuadMesh();

  this->instanceMesh = new Mesh(this->mesh->getVertices(), MeshType::TRIANGLE_MESH);

  this->combinedTextures = new Texture(WINDOW.w, WINDOW.h, GL_RGBA, nullptr);

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

  GLint currentFBO = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);

  glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferId);
  glViewport(0, 0, WINDOW.w, WINDOW.h);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const uint16_t shaderID = this->uiItemShader->getProgram();
  glUseProgram(shaderID);

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

void UI::beforeUpdate()
{
  if(true)//if ui items have changed, window size has changed or this tranform has changed.
  {
    this->fakeDraw();
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
    data.modelMatrixRow0 = item->getModelMatrix().GetRow(0);
    data.modelMatrixRow1 = item->getModelMatrix().GetRow(1);
    data.modelMatrixRow2 = item->getModelMatrix().GetRow(2);
    data.modelMatrixRow3 = item->getModelMatrix().GetRow(3);

    data.textureIdx = textureCounter;

    gpuData.push_back(data);

    item->texture->bind(this->uiItemShader.get(), textureCounter);
  
    textureCounter++;
  }

  glBufferData(GL_ARRAY_BUFFER, sizeof(uiItemData) * gpuData.size(), gpuData.data(), GL_STATIC_DRAW);

  std::size_t vec4Size = sizeof(cy::Vec4f);
  
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