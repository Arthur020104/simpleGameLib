#include <float.h>
#include <movementController.h>

#define MAX_PLANES 16

float MovementController::minValueToMove = 0.1f;

typedef struct PlaneCollectContext
{
  b3CollisionPlane planes[MAX_PLANES];
  int count;
} PlaneCollectContext;

static bool CollectPlane(b3ShapeId shapeId, const b3PlaneResult* plane, int planeCount, void* context)
{
  PlaneCollectContext* ctx = (PlaneCollectContext*)context;

  if (ctx->count >= MAX_PLANES)
    return false;

  b3CollisionPlane* out = &ctx->planes[ctx->count++];
  out->plane = plane->plane;

  out->pushLimit = FLT_MAX;
  out->clipVelocity = true;
  out->push = 0.0f;

  return true;
}

MovementController::MovementController(glm::vec3 position): GameObject("../obj/square.obj", Program::getDefaultShader(), GameObjectType::KINEMATIC)
{
  this->setPosition(position);
  this->setScale(glm::vec3(1.0f, 2.0f, 1.0f));

  std::shared_ptr<Material> material = std::make_shared<Material>(
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    1.0f
  );

  this->useOnly(material);
}

void MovementController::setScale(glm::vec3 scale)
{
  GameObject::setScale(scale);

  float radialScale = (this->getScale().x + this->getScale().z) * 0.5f;
  this->mover.radius = 0.5f * radialScale;

  this->unscaledHalfHeight = (this->getMesh()->boundingVolume[1].y - this->getMesh()->boundingVolume[0].y) * 0.5f;
  float scaledHalfHeight = this->unscaledHalfHeight * this->getScale().y;

  this->mover.center1 = (b3Vec3){ 0.0f, -scaledHalfHeight, 0.0f };
  this->mover.center2 = (b3Vec3){ 0.0f, scaledHalfHeight, 0.0f };
}

void MovementController::start()
{
  this->createPhysicalBody(PhysicalShapeType::CUBE, 1.0f, 0.3f);
}

void MovementController::updateMoverPosition()
{
  const float timeStep = (float)WINDOW.deltaTime;

  b3Vec3 vel = (b3Vec3){ this->velocity.x, this->velocity.y, this->velocity.z };
  b3Vec3 translation = b3MulSV(timeStep, vel);

  b3QueryFilter filter = b3DefaultQueryFilter();
  b3Pos origin = (b3Pos){ this->getPosition().x, this->getPosition().y, this->getPosition().z };

  float fraction = b3World_CastMover(this->scene->getWorldId(), origin, &this->mover, translation, filter, NULL, NULL);
  b3Vec3 safeDelta = b3MulSV(fraction, translation);

  glm::vec3 newPos = glm::vec3(origin.x + safeDelta.x, origin.y + safeDelta.y, origin.z + safeDelta.z);
  this->setPosition(newPos);
  origin = (b3Pos){ newPos.x, newPos.y, newPos.z };

  PlaneCollectContext planeCtx;
  planeCtx.count = 0;
  b3World_CollideMover(this->scene->getWorldId(), origin, &this->mover, filter, CollectPlane, &planeCtx);

  b3PlaneSolverResult result = b3SolvePlanes(b3Vec3_zero, planeCtx.planes, planeCtx.count);

  glm::vec3 correctedPos = newPos + glm::vec3(result.delta.x, result.delta.y, result.delta.z);
  this->setPosition(correctedPos);

  vel = b3ClipVector(vel, planeCtx.planes, planeCtx.count);
  this->velocity = glm::vec3(vel.x, vel.y, vel.z);

  float maximumVelocity = this->maxSpeed > 0.0f ? this->maxSpeed : FLT_MAX;

  this->velocity = glm::clamp(this->velocity, -maximumVelocity, maximumVelocity);

  this->isGrounded = false;
  b3Pos rayOrigin = (b3Pos){ this->getPosition().x, this->getPosition().y, this->getPosition().z };
  glm::vec3 downVec = this->getUpVector() * -this->groundedRayDistance;
  b3Vec3 direction = (b3Vec3){ downVec.x, downVec.y, downVec.z };

  b3RayResult rayResult = b3World_CastRayClosest(this->scene->getWorldId(), rayOrigin, direction, b3DefaultQueryFilter());

  if (rayResult.hit)
    this->isGrounded = true;
}

void MovementController::beforeUpdate()
{
  this->currentAcceleration = this->acceleration;
  this->currentDrag = this->drag;

  if (!this->isGrounded)
  {
    this->currentAcceleration = this->inAirAcceleration;
    this->currentDrag = this->inAirDrag;
    this->velocity += glm::vec3(0.0f, -20.81f, 0.0f) * (float)WINDOW.deltaTime;
  }

  glm::vec3 xyVelocity = glm::vec3(this->velocity.x, 0.0f, this->velocity.z);

  this->velocity -= this->currentDrag * (float)WINDOW.deltaTime * xyVelocity;

  glm::vec3 moveDirection = glm::vec3(0.0f, 0.0f, 0.0f);
  
  if (glfwGetKey(WINDOW.window, GLFW_KEY_W) == GLFW_PRESS)
    moveDirection += this->getForwardVector();
  if (glfwGetKey(WINDOW.window, GLFW_KEY_S) == GLFW_PRESS)
    moveDirection -= this->getForwardVector();
  if (glfwGetKey(WINDOW.window, GLFW_KEY_A) == GLFW_PRESS)
    moveDirection -= this->getRightVector();
  if (glfwGetKey(WINDOW.window, GLFW_KEY_D) == GLFW_PRESS)
    moveDirection += this->getRightVector();
  if (glfwGetKey(WINDOW.window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    moveDirection += this->getRightVector();

  if (glfwGetKey(WINDOW.window, GLFW_KEY_SPACE) == GLFW_PRESS && this->isGrounded)
  {
    this->velocity += glm::vec3(0.0f, this->jumpForce, 0.0f);
    this->isGrounded = false;
  }

  if (moveDirection != glm::vec3(0.0f, 0.0f, 0.0f))
    moveDirection = glm::normalize(moveDirection);

  float yVel = this->velocity.y;

  this->velocity += this->currentAcceleration * (float)WINDOW.deltaTime * moveDirection;

  this->updateMoverPosition();

  this->setRotation(glm::vec3(0.0f, this->scene->getActiveCamera()->getRotation().y, 0.0f));
}