# Todo
- 1. review/refactor how scene class is working afther the creation of the component class
- 2. create types of meshes, now just triangle and line, but leave an switch on the draw function to maybe add more types of meshes in the future
- 3. Add lights.
- 4. Add input handling for the window
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