#include <../gameObject.h>

class ExampleObject: public GameObject
{
  public:
    using GameObject::GameObject;

    void start() override;
    void beforeDrawing() override;
    void aftherDrawing() override;
  private:
};