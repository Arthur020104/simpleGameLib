# Todo
- 1. Add an instancing for gameObjects, I think the best aprouch is to continue adding every single gameObject to the scene, and in the draw phase for loop through all gameObjects we can ignore objects that are instanced(add a flag to gameObject class, shouldDraw). for the actual instancing we can create a new class to take care of it. I dont want to add as a component bc it does not have the core functionality of a component, is more like a group.
  - 1.1 When redering make sure we load for shader only once each material. Use a set to make sure unique and to get index faster for the material uniform buffer.
  - 1.2 Execute instancing in loops, to make sure even if we hit the max TEXTURES or max MATERIALS we can still draw the rest of the objects.
  - 1.3 Scene class will have a vector of instacing groups.
  - 1.4 Instacing group must make sure that all gameObjects have the same mesh and shader.
  - 1.5 Define the vbo data once on creation and just change as needed.
    - 1.5.1 When the amounts of gameObjects is the same we just change the vbo data that is relative to position(model matrix), we assume that once is instaced material data does not change, but we will create a method to force the reload of the material data(maybe just reload all the vbo data, using the same method as the below).
    - 1.5.2 When changes we need to reload all the vbo data.
Create, ui. We can create a child of the game object for instancing, and for the ui component we can create its own scene and when components inside change, we can
update the ui, by rendering the ui scene to a texture and then drawing it on the screen.
- 2. Add input handling for the window
- 3. Create a simple game like a bomberman clone to test

# Text Redering
- For text rendering there is a limit of char that i need to render depending on text size. For doing instancing rendering i will need total text/maxTExturesSupported andd rendering n textures. After all done get this n textures and do a final redering, for extreme cases this may need to need an extra call so it needs to be recursive. 

Preload all font chars into textures.

Like:
child of the base texture class

std::string myStringRedering; 
glm::mat4 projectionMat;//On the creation based on uint16_t width, uint16_t height defined on constructor 

struct renderChar
{
    glm::vec2 pos;
    Texture* char;
    bool needsToDestroy;
};

void changeString(std::string str)
{
    myStringRedering = str;
    std::vector<renderChar> fontTexture;
    for(char c: str){
        renderChar r;
        r.char = TEXT.getCharTexture(c);
        //calculate offsets here, will be a lot more complex than this, remember to take into account \n
        r.pos = offset;   
    }
}

Texture* generateFinalTexture(std::vector<renderChar> textures)
{
   uint_16 totalInstancesLoops = textures.size()/MAX_TEXTURE_SUPPORTED;//ROUND UP TO NEXT INTEGER
   if(totalInstancesLoops > 1)
   {
        std::vector<renderChar> newTextures;//all renderChar will have an pos = 0, 0, and needs to destoy = true
        FOR(totalInstancesLoops)
        {
            //prepare gpu data,
            //render
            newTextures.push(generatedTexture)
        }
        
        Texture* finalTexture = generateFinalTexture(newTextures);

        for(texture: newTextures)
            if(texture.needsToDestroy) delete texture.char;
        
    }
    else
    {
        //only diference for the above is that the binds will occours directly on this texture.
        //prepare gpu data,
        //render
    }
   
}

## TODO LATER, NOT RANKED

- Create scene bvh that just intersect the gameObject that is in bound, will be useful for big scenes with a lot of objects, and new forms of intersection
Make sure to create an base abstract class for bvh. Functions like insideBoundingVolume, alterBoudingMax and alterBoudingMin should be an static function of this abstract class; methods like getNewPoint will be an virtual function that will be implemented on the parent class, but can be overridden, intersect will be only implemented in the child class.
- think about if component will inherit transform or if transform will be a component
- Clean up the code, remove commented code and improve code structure

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

# Done
- CREATE SCENE CLASS
- TEST SCENE
- Create variable for tracking the number of objects using the program, after all finish using delete, exactly same code that exists for mesh
- Create a global variable for the window, so that it can be accessed from anywhere in the program.
- create bvh for optimizing the ray intersection
- move the drawing of the mesh to the mesh class
- change mesh class to use smart pointers
- review/refactor how scene class is working afther the creation of the component class
- The update of the model matrix and camera matrices needs to be refactored, they are being updated every time the get is called
- Adding lights
- Add texture.
Materials: every game object should be able to have N materials, and we should be able to set by index. Create an VBO for the materials indices on the object, and a uniform buffer for the materials data.
- Add materials, make sure to load materials in the .obj file - look up creating a class that stores for an map for each class data varibles(maybe leave this precompiled thing for latter)
- Add an execution queue with time, will run in a separate thread.
- Make materials load from the .obj file