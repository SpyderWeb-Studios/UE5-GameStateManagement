This is an open source plugin for basic game state management. 

You can create new states, each one requiring a new Class of Object and they will be responsible for their own init/cleanup on each transition.

You can query the GameStateManagerComponent for each class, and it will either return an Object or create a new Object and then return it. 

The roadmap for this plugin is:
- Author objects rather than simply creating new classes, therefore being able to create a state once and then create instances to use individually

Other Roadmap points will be taken from community feedback

