#include <ray.h>
#include <algorithm>

bool intersectBoundingVolume(Ray& r, cy::Vec3f* boundingVolume)
{
  float t1x = (boundingVolume[0].x - r.origin.x) / r.direction.x;
  float t2x = (boundingVolume[1].x - r.origin.x) / r.direction.x;

  if(t1x > t2x)
    std::swap(t1x, t2x);

  float t1y = (boundingVolume[0].y - r.origin.y) / r.direction.y;
  float t2y = (boundingVolume[1].y - r.origin.y) / r.direction.y;

  if(t1y > t2y)
    std::swap(t1y, t2y);
  
  float t1z = (boundingVolume[0].z - r.origin.z) / r.direction.z;
  float t2z = (boundingVolume[1].z - r.origin.z) / r.direction.z;

  if(t1z > t2z)
    std::swap(t1z, t2z);
  
  float maxOfMin = std::max(t1x, t1y);
  maxOfMin = std::max(maxOfMin, t1z);
  float minOfMax = std::min(t2x, t2y);
  minOfMax = std::min(minOfMax, t2z);


  if(maxOfMin > minOfMax || minOfMax < ERROR_MARGIN)
    return false;
  
  return true;
}

bool intersectTriangle(Ray& ray, GameObject* gameObject, Vertex &v0, Vertex &v1, Vertex &v2)
{
  //using parallelogram area bc the ratio will be the same
  cy::Vec3f planeNormal = (v1.pos - v0.pos) ^ (v2.pos - v0.pos);

  float area = planeNormal.Length();

  if(abs(area) <= ERROR_MARGIN) return false;

  float overArea = 1.0f / planeNormal.Length();

  planeNormal.Normalize();

  cy::Vec3f centerPoint = (v0.pos + v1.pos + v2.pos) *  0.33333333333f;

  float v = planeNormal % ray.direction;
  if(abs(v) <= ERROR_MARGIN) return false;

  const float t = (planeNormal % (centerPoint - ray.origin)) / v;

  if(t < TMIN) return false;

  cy::Vec3f pointOnPlane = ray.origin + ray.direction * t;

  float total = 0;

  float alpha = (((v1.pos - pointOnPlane) ^ (v2.pos - pointOnPlane)) % planeNormal) * overArea;
  if (alpha < 0.0f || alpha > 1.0f) return false;

  float beta = (((v2.pos - pointOnPlane) ^ (v0.pos - pointOnPlane)) % planeNormal) * overArea;
  if (beta < 0.0f || beta > 1.0f) return false;

  float gamma = 1.0f - alpha - beta;
  if (gamma < 0.0f || gamma > 1.0f) return false;
  
  total = alpha + beta + gamma;

  if(abs(total - 1.0) > ERROR_MARGIN) return false;

  Hit hit;
  hit.point = pointOnPlane;
  hit.hitObject = gameObject;

  ray.hits.push_back(hit);

  return true;
}