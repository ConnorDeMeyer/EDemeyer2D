EDemeyer2D is a 2D Engine made in C++ which can be used for simple 2D applications.

It is powered by win32 and Direct2D and includes the following features:
  - Rendering of bitmaps
  - Playing sounds
  - Simple dynamic delegates system
  - Simple 2D collision and overlap detection (work in progress)
  - a Game instance -> Scene -> Object -> Component code hierarchy
  - Automatically cleans up destroyed instances of added components
  - A resource manager that loads bitmaps and only unloads them when they are not in use anymore. Also doesn't load resources more than once
  - An Input system where the user can link keys, buttons and controllers to functions.
  
The engine also cleans up every scene, objects and components that the user makes.

Some examples of components are:
  - Audio component (capable of playing and looping sounds and is powered by XAudio2)
  - Collision components (circle collision, square collision and polygon collision)
  - Time line component (used for timing and waiting)
  - texture component (draws a texture where to component is in space)
  - sprite component (draws an animated sprite)
  - text component (draws any kind of text and is powered by DWrite)
