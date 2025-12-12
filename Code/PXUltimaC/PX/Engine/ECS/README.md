# Entity Component System (ECS)

## Info
Entitys, ingame spawnable objects that a player might interact with.<br>
Components, building blocks that can be used to add behaviour to entitys.<br>
This system allows a very modular building system for different kinds of behaviours for any types of entitys.

## Design - Common issues in other engines
### The base Object issue
A big problem caused by Object Oriented Programming (OOP) is the idea of a base GameObject (Unity) or UObject/Actor (Unreal). 
These are often very heavy and hold way more data then needed. This results in the overall resource suffecation of the engine with a moderate amount of objects.
Because of this behaviour, alternative APIs where created like Unity DOTs. For the anoyance of the user, these systems are crude and not intuative, even worse, incompatible with already created code.
Although these systems do their work, the added learning curve and limited advantage for smal programs stops most casual users from engaging with it.

### Component where?
Unity has a neat problem, if you want to get components, you either need to set it up in the IDE, which can randomly clear itself or be stuck on an previous state. 
Or we need to use alot of possibly nested GetComponent<T>() in the worst case. This is very instable and can sure break if the object is simply rearranged. It is a common cause of bugs or even crashes.

### Component data location
The first thought of implementation, again from the mindset of OOP, is to give our entity a linked list of components it owns. 
This sure makes it intuitive and easy to understand, jet this causes a big problem if we now want to loop over all kind of components.
We could create a linked list purely just to loop over each component, this would seem to solve the problem.
If we look closer where exactly the objects are allocated and stored, we will see that these will be all over the place in the memory space.
To solve this, we can just use an object pool. This avoids allocations on create and is more cache friendly if we update all components in a scene.

### Adding new components
A very frustrating element is new the creation and registration of 3rd party components.
Often not intuitive and sometimes very slow.
Complains are often more about unituitive APIs and slow adding of components. 


## Design - A solution attempt
### Structure of types
- Entity 
  - As it purest form. shall be only an ID.<br>We might add a few generic flags for behaviour and info. This object needs to be as smal as possible to allow alot of them to exist.
- Component
  - Position, Rotation, generally plain data.<br>To allow better parralelisation, we need to put them closer together in a flat array as a pool of components.
- Resource
  - Texture, Meshe, Shader, all elements that contain any handle to some API.<br>OS, OpenGL, or anything from an external library.   
- System
  - Function that can loop over all components and compute them.<br>Seperates computation away from the main system and can allow parralelism when components are stored in a compact manner. 

Additionally to mention, we need to register all of these types into a system. As "C" does not have any reflextion system, we need to store meta data manually or could use makro magic.
The plan is to even alow 3rd party mods to make their own structs/classes and register them into the system. From this system now, we can use the GUI or any other system to view and change everything dynamically. This should allow for a vastly dynamic system. 

### A safe reference
As we use "C", a possible problem are pointers to these elements.
If the given array is too smal, we might move the data, this could make the holded adress stale, thats bad.
To solve this, we give the element an ID aswell and combine it with the adress. 
Now if we want to access the element, we need to call a verify function, after that we can diretly use the adress if valid.
To check if a given adress is valid, we have to firstly check if the adress is in a valid range, known from the component pool.
If its not, we need to search out object via the ID in the list. After we can point correctly to our target again.
When we dont have an object after searching, we have detected a deleted object.
With a valid reference, check the ID equal to what we expect or search it new.
If we dont find any valid  reference, the object has been deleted, invalidating the reference.
With this, with locks aiding, it should be impossible to get invalid references, which makes these pointers safe to use.

## Links
- [wikipedia - ECS](https://en.wikipedia.org/wiki/Entity_component_system)
