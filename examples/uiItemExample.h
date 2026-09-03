#include <../ui.h>
#include <../window.h>

class ExampleUIItem: public UIItem
{
  public:
    using UIItem::UIItem;

    void afterUpdate() override
    {
      //this->setPosition(this->getPosition() + glm::vec3(0.01f * WINDOW.deltaTime, 0.0f, 0.0f));
    }
};