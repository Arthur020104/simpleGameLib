# Todo
- 1. The update of the model matrix and camera matrices needs to be update, they are being updated every time the get is called
- 2. Add lights.
- 3. Add input handling for the window
- 4. Add texture.
- 5. Add materials - look up creating a class that stores for an map for each class data varibles(maybe leave this precompiled thing for latter)
- 6. Create a simple game like a bomberman clone to test
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