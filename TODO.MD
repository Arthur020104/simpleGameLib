# Todo
- 1. Add lights.
- 2. Add input handling for the window
- 3. Add an execution queue with time, will run in a separate thread.
- 4. Add texture.
- 5. Create scene bvh that just intersect the gameObject that is in bound, will be useful for big scenes with a lot of objects, and new forms of intersection
Make sure to create an base abstract class for bvh. Functions like insideBoundingVolume, alterBoudingMax and alterBoudingMin should be an static function of this abstract class; methods like getNewPoint will be an virtual function that will be implemented on the parent class, but can be overridden, intersect will be only implemented in the child class.
This "intersectBoundingVolume" also must be a static function of the parent class.
```cpp
bool MeshBvhNode::intersect(Ray& ray, GameObject* gameObject)
{
  bool hit = intersectBoundingVolume(ray, this->boundingVolume);
  if(!hit) return false;
  
  if(!this->isLeaf)
  {
    if(children[0] != nullptr && children[0]->intersect(ray, gameObject))
      return true;

    if(children[1] != nullptr && children[1]->intersect(ray, gameObject))
      return true;
    
    return false;
  }
  //# old implementation
  // for(uint32_t i = 0; i < items.size(); i += 3)
  // {
  //   if(intersectTriangle(ray, gameObject, items[i], items[i + 1], items[i + 2]))
  //     return true;
      
  // }
  // # Maybe change to this
  return this->intersectLeaf(ray, gameObject);
}
```
- 6. Add materials - look up creating a class that stores for an map for each class data varibles(maybe leave this precompiled thing for latter)
- 7. Create a simple game like a bomberman clone to test
- think if component will inherit transform or if transform will be a component
- Clean up the code, remove commented code and improve code structure

# Done
- CREATE SCENE CLASS
- TEST SCENE
- Create variable for tracking the number of objects using the program, after all finish using delete, exactly same code that exists for mesh
- Create a global variable for the window, so that it can be accessed from anywhere in the program.
- create bvh for optimizing the ray intersection
- move the drawing of the mesh to the mesh class
- change mesh class to use smart pointers
- review/refactor how scene class is working afther the creation of the component class
- The update of the model matrix and camera matrices needs to be update, they are being updated every time the get is called