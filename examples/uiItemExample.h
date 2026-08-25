#include <../ui.h>
#include <../window.h>

class ExampleUIItem: public UIItem
{
  public:
    using UIItem::UIItem;

    void afterUpdate() override
    {
      this->setPosition(this->getPosition() + cy::Vec3f(0.01f, 0.0f, 0.0f) * WINDOW.deltaTime);
    }
};