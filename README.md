# RIO Engine
A framework for easier game development on Wii U and Windows (32-bit strictly, for now) PC, as well as homebrew development on Wii U.  
  
RIO guarantees equivalent behavior between PC and Wii U for all defined behavior (well, as far as my tests went :P), making it much easier to develop for Wii U on PC, removing the need for frequent and cumbersome testing on real HW (although testing is still recommended from time to time).  
  
RIO is based heavily on Nintendo's own libraries for Wii U such as my [sead](https://github.com/aboood40091/sead) decompilation project. (Some components are even direct copies, as described later below, with added Windows support). 
Therefore, it can be used as an accurate source on how to use certain features on the Wii U, as well as Nintendo's answer to cross-platform support that includes their platforms.  

Not all components in RIO are thread-safe and **there is currently no multi-threading support**, with hopes of actually adding it in the future.  

Examples can be found [here](https://github.com/aboood40091/RIO-Tests).  
  
Docs can be found... nowhere yet as I am still working on it. In the meantime, I tried to fill this README with as much information I could and left some useful comments in the headers.  

## Requirements & Dependencies
* GCC compiler with C++17 support and GNU extensions.  
* A build target must be specified by defining one of the two macros: `RIO_DEBUG` and `RIO_RELEASE`.
* RIO has been tested with `Og` in the debug build and `O3` in the release build.  
* `notes_build.txt` specifies the build options I used for testing.  

Dependencies:
##### Windows
* OpenGL (Minimum: v4.0 Core Profile, see `notes_windows.txt` for more)  
* GLEW  
* GLFW3  
##### Wii U
* devkitPPC (Tested with latest version as of August 7th, 2022)  
* [wut](https://github.com/devkitPro/wut)  

Optional:
* SDL2 Mixer (for the audio module)  

## Initialization
RIO can be initialized and used as follows:  
```cpp
int main()
{
    // Initialize RIO with root task
    if (!rio::Initialize<RootTask>())
        return -1;

    // Main loop
    rio::EnterMainLoop();

    // Exit RIO
    rio::Exit();

    return 0;
}
```  
Where `RootTask` is the root task class (see `task` module below).   
See `rio.h` for more.  

Singletons initialized by calling `rio::Initialize()` in order (destroyed in the opposite order by `rio::Exit()`):  
* `FileDeviceMgr`  
* `Window`  
* `TaskMgr`  
* `ControllerMgr`  
* `PrimitiveRenderer`  
* `Renderer`  
* `ModelCacher`  
* `AudioMgr`  

Main loop starts with `TaskMgr` executing, followed by `Renderer` rendering all layers, and, finally, swapping buffers of `Window`. (May change in the future with `Window` events being first to be processed.)  

(See below for explanation of all aforementioned classes.)

## Modules
This section describes the modules (folders) in the RIO project and their components.  
Each module may contain sub-folders for platform-specific implementations of some components. These platforms are:  
* `cafe` for Wii U.  
* `win` for Windows.  

### misc
Structures that do not need each their own separate folder have been collected here.  

#### `rio_Types.h`
Header that defines:
* Macro `RIO_IS_WIN` on Windows and `RIO_IS_CAFE` on Wii U to 1 (0 otherwise). This is to be used to distinguish between host platforms at compile-time.  
* Fixed-size types: `BOOL` as `int`, `s8`, `u8`, `s16`, `u16`, `s32`, `u32`, `s64`, `u64`, `f32` and `f64`.
* `RIO_ASSERT` and `RIO_LOG` preprocessor functions (that only have an effect if build target is `RIO_DEBUG`).  

#### gl/`rio_GL.h`
Header that takes care of including GLEW and GLFW3 headers for GL targets. Additionally, header defines `RIO_GL_CHECK_ERROR` and `RIO_GL_CALL` preprocessor functions that help with error checking (and only have an effect if build target is `RIO_DEBUG`).  

#### win/`rio_Windows.h`
Header that takes care of including `windows.h` properly.  

#### `BitFlag<T>`
Class with fixed size (of `sizeof(T)`) that treats each bit as a flag. Copied directly from sead.  
See header for more.  

#### `MemUtil`
Self-explanatory class for memory-related operations. See header for more.  

### audio
This module is a simple wrapper over SDL2 Mixer and is completely optional.  
It is enabled by defining the macro `RIO_AUDIO_USE_SDL_MIXER`.  
It allows for creating music instances and sound effect instances.  

The main three differences between a music instance and a sound effect instance are:  
* Number of instances that can be played at the same time.  
* Music is streamed, whereas sound effects are stored in their entirety in memory.  
* Panning is not possible with music instances, whereas this module provides an interface for simulating 3D audio for sound effects using panning.  
Supported file formats are `WAV`, `MP3`, `OGG`.  

#### `AudioBgm`
The music instance. Many can be created, but only 1 can be played at a time. Playing another instance will take over.  

#### `AudioSfx`
The sound effect instance (called "chunk" by SDL2 Mixer). Many can be created, but only 16 (more specifically, double the SDL Mixer limit) can be played at a time. Since only 16 sound effects can be played at a time, there are 16 "slots" (SDL2 Mixer calls them "channels") for storing the currently playing sound effect instances. Therefore, there are 2 types of `play` functions provided:  
* 1. The slot is picked automatically by simply using the next slot from the last one used (last slot loops back to the first).  
* 2. Picking the slot is up to the user. (But may be replaced by the mechanism above. A locking mechanism for preventing replacement of playing sound effects may be added in the future.)  

#### `AudioSfxHandle`
An class for storing a playing `AudioSfx` instance, providing the instance and which slot it's currently playing in.  

#### `AudioMgr`
The class responsible for loading and caching audio files, as well as managing the listener for the 3D audio interface and controlling *global* volumes (Master, Music, Sfx).  
Audio files are loaded using the functions `load{Bgm/Sfx}()`, which expect the audio file(s)' path, _including_ the extension, ***relative to the `sounds` folder on the default file device***. (File devices are explained below.)  

### container
This is a module for fast and lightweight containers that has been carried over from sead. Not all containers have been copied over, in favor of STL containers, although that may change in the future (either with more sead containers or completely custom ones).  
* `LinkList`: Basic, circular, doubly-linked list base class for other container types.  
* `TList<T>`: Circular, doubly-linked list of objects of type `T`.  

### controller
This module for provides a virtual interface for checking controller input, independent of the platform and the controller device connected.  
This module has also been copied from sead, with controllers for Windows specifically made for RIO.  
  
There are 3 main components in the controller system:  

#### Controller Device (`ControlDevice`)
This base class (and its inheritors) represents the component that directly interacts with, checks the status of and fetches information from a physical controller device (e.g. buttons currently held).  At runtime, an instance exists for every supported controller device.  
  
Supported devices are:  
##### Wii U
* `CafeVPadDevice`: Wii U Gamepad.  
* `CafeWPadDevice`: 4 Wii Remotes (with optional Nunchuk and Classic Controller extensions) or Wii U Pro Controllers.  
##### Windows
* `KeyboardMouseDevice`: Keyboard and mouse.  
* All devices supported by the [SDL Game Controller Database](https://github.com/gabomdq/SDL_GameControllerDB). (Download `gamecontrollerdb.txt` and place it **on the root of the default file device**. File devices are explained below.)  

#### Controller Mapper Interface (`Controller`)
This base class (and its inheritors) represents a virtual controller which provides a unified, fixed list of key bindings. Its job is to process the information fetched from a `ControlDevice` and to map it to the unified key bindings. It also provides pointer/touch functionality.  
This removes the need for developers to write extra code for handling different types of controllers, although if the application is required to do that (e.g. for different icons), that is still possible by checking the ID provided of a control device or controller at runtime.
  
The buttons of a controller are shown below:  
  
![](https://i.imgur.com/kE2Yhje.png)

At runtime, for each `ControlDevice` instance, there should be at least one `Controller` instance.  

Provided controller types are:  
##### Wii U
* `CafeDRCController`: Wii U Gamepad.  
* `CafeRemoteController`: A single Wii Remote or Wii U Pro Controller.  
##### Windows
* `WinController`: Keyboard and mouse.  
* `WinGamepadController`: A single instance of any device supported by the SDL Game Controller Database.  (Maximum is defined by GLFW, but as of now, is 16)  

#### Controller Manager (`ControllerMgr`)
This is a task which keeps track of all present instances of controller devices and controllers for retrieval when needed, as well as automatically updating them.  

This manager can be used to retrieve any control device or controller instance by their type and index, but for convenience, it can also be used to retrieve:  
* The main controller for the platform. (Wii U Gamepad or Keyboard and Mouse on Windows)  
* The main "gamepad" for the platform. (Same as above, for now)  
* The main controller with a pointer or touch functionality. (Same as main controller for Wii U and Windows)  
* A generic "gamepad" (controller with multiple instances connected) by its index. (`CafeRemoteController` on Wii U and `WinGamepadController` on Windows)  

### filedevice
This module represents the system to handle reading (TODO: and writing) files using virtual paths and independent of the host platform. Based heavily on sead with some custom file device implementations.  

#### `FileDevice`
This is a class that maps a certain, platform-dependent directory to a virtual directory, such that files can be accessed by using the virtual path instead of the complete, platform-dependent path. The device will perform all the platform-dependent operations needed to manage files (reading, streaming, seeking, etc.).  
Each device will have a drive name, similar to drive letters on Windows.  

This allows for a unified logical directory for game content so that developers don’t have to worry about handling platform differences when reading game files.  

Provided file devices (with their drive names) are:  
##### Windows & Wii U
* `ContentFileDevice` (drive name `content`): This file device maps to:  
	* The path **“./fs/content”** (relative to the executable) on _Windows_.  
	* The appropriate game **content** folder on _Wii U_.  
* `NativeFileDevice` (drive name `native`): File device that allows for handling files using the platform's native pathes for those files (i.e. does no mapping).  
##### Wii U
* `CafeSDFileDevice` (drive name `sd`): This file device maps to a certain path on the SD card.  
	This path is specified as a string by the macro `RIO_CAFE_SD_BASE_PATH`. By default, its value is `"rio"`, meaning that this device will deal with files in this folder and its subdirectories.  
	Developers can define their own value for the macro.
##### Planned
* `CafeSaveFileDevice`: A file device for natively handling save files on Wii U.  

#### `FileDeviceMgr`
This is a class that keeps track of all created file devices. The main feature of this class is that, instead of retrieving a file device and using it directly, if given the drive name and virtual path, it will automatically find the correct file device through the drive name and perform any operation requested on the given virtual path. The general format would be:  
- `{drive name}://{path relative to drive's mapped directory}`

For example, if trying to perform operations on the file “test.txt” that is stored in the mapped directory of `ContentFileDevice` (such that its path would end up being e.g. on Windows `./fs/content/text.txt`), that can be done by passing the path `content://test.txt` to the methods of `FileDeviceMgr`. If drive name is not specified (e.g. you just pass `test.txt`), the manager's default file device is used, which is described below.  

There are three kinds of file devices that `FileDeviceMgr` always provides:
* An instance of `NativeFileDevice`.  
* `MainFileDevice`: This is considered the platform's main device. On Windows, it's always `ContentFileDevice`. On Wii U, it is `CafeSDFileDevice` (as convenience for homebrew), but defining the macro `RIO_CAFE_MAIN_FILE_DEVICE_AS_CONTENT` makes `ContentFileDevice` the main device. The main device can always be acquired by calling `FileDeviceMgr::getMainFileDevice()`.  
* Default file device: This is the device type mentioned earlier. By default, it is the manager's main device, but a custom device can be made the default by using `FileDeviceMgr::setDefaultFileDevice()`.  

### gpu
Module for a general-purpose render API, providing components and wrappers that deal directly with the GPU and its data.  

#### `Drawer`
Class for issuing native draw commands. See header for more.  
Additionally, it provides useful alignments for Wii U:  
* `cVtxAlignment`: Recommended alignment for vertex attribute buffer data.  
* `cIdxAlignment`: Recommended alignment for index buffer data.  
* `cUniformBlockAlignment`: **Required** alignment for uniform block data.  

OpenGL EBOs are not used on Windows, which may be one of the biggest bottlenecks in RIO on Windows. Rather, index buffers must be passed to the draw call instead for easier compatibility with Wii U. (TODO: Explore more solutions?)  

#### `RenderState`
Class for setting the render state of the GPU (blending, depth and stencil tests, culling, polygon mode and offset). Copied from sead.  

While the Wii U natively supports performing alpha testing and separate polygon modes for front- and back-facing faces, they were removed from core modern OpenGL as they are expected to be performed by the user using shaders if needed and therefore are not supported, but may be added as Wii U-only features in the future.  

#### `Shader`
A class for loading and handling shaders. (Only Vertex and Fragment for now, with plans for Geometry and maybe Compute)  
Expected format is GLSL source on Windows and GSH (GFD Shader) on Wii U (no alignment requirement).  
GSH can be generated from assembly using [latte assembler](https://github.com/GaryOderNichts/wiiu-shaders), or GLSL source using the Cafe SDK's `gshCompile` tool. (Example of usage: `gshCompile -no_limit_array_syms -nospark -O -v {Vertex GLSL source file} -p {Fragment GLSL source file} -o {GSH output file}`)  

Shaders are loaded using the function `load()`, which expects the shader file(s)' path, without the extension, ***relative to the `shaders` folder on the default file device***.  
On Windows, it will search for that path appended with the extensions `.vert` for the vertex shader source and `.frag` for the fragment shader source. On Wii U, the appended extension is `.gsh`.  

Note that on Wii U, you cannot mix uniform variables and blocks in the same shader. You also cannot use uniform variables with Geometry and Compute shaders and must use uniform blocks.  
The Wii U also has a global shader mode that must be set accordingly. This is automatically done when binding a `Shader` instance, but if needed to be done manually, the (static) function `Shader::setShaderMode()` can be used.  

(Consequently, the concept of shader modes does not exist at all on Windows and therefore are not respected.)  

#### `Texture2D`
A class for loading texture files and runtime native textures and creating handles for them, with mipmaps support.  
Expected format is RTX (custom format) on Windows and GTX (GFD Texture) on Wii U (no alignment requirement).  
Many tools are available for generating GTX files, such as [this one](https://github.com/aboood40091/GTX-Extractor-Rewrite) (builds of older version [here](https://github.com/aboood40091/GTX-Extractor/releases)).  
A script is available in the repository for converting GTX to RTX, with hopes for a separate tool in the future.  
See header for supported texture formats.  
Support for other dimensions and depths, as well as multiple textures per file (though not a priority) is planned.  

Texture files are expected to be ***relative to the `textures` folder on the default file device***.  
Appended extension is `.rtx` on Windows and `.gtx` on Wii U.  

#### `NativeSurface2D`
Structure used to store the native 2D surface data. (`GX2Surface` on Wii U, see header for structure on Windows)  

#### `NativeTexture2D`
Structure used to store the native 2D texture data. (`GX2Texture` on Wii U, see header for structure on Windows)  

##### `NativeTexture2DHandle`
Pointer to `GX2Texture` on Wii U, OpenGL object ID (`unsigned int`) on Windows.  

#### `TextureSampler2D`
Class representing the parameters for a shader 2D sampler and handles binding it with a linked `Texture2D` instance. Based heavily on agl.  
See header for more.  

#### `UniformBlock`
Class for binding uniform blocks.  
Only for uniform blocks with std140 layout.  
Explicit binding on Windows with OpenGL is currently NOT supported.  

Note that on Wii U, the data is passed directly to the GPU, therefore it must not be freed as long as the uniform block buffer is being used. Furthermore, keep in mind that on Wii U the data setter functions **byteswap** the passed data as the Wii U's GPU expects uniform data to be in **little endian**, whereas the CPU is **big endian**.  
Also, on Wii U, uniform blocks can have different indices between the Vertex and Fragment shaders. They can only guaranteed to be the same through explicit binding in the shader sources.  
As for Windows, which currently uses OpenGL, it is guaranteed that the uniform block index will be the same for the Vertex and Fragment shaders (explicit binding in the shader sources, however, will be ignored and the uniform block index must be used).  

#### `VertexBuffer` 
A class for storing buffers of vertex attribute data. There is a maximum of 16 slots for concurrent vertex buffers per drawcall, but multiple attributes can be stored in a single vertex buffer.  

Note that on Wii U, the data is passed directly to the GPU, therefore it must not be freed as long as the vertex buffer is being used.  

#### `VertexStream`
Class representing the layout of a vertex attribute  (location in shader, offset in vertex buffer, data format).  
See header for supported data formats.  

#### `VertexArray`
Class for keeping track of vertex streams and their assigned vertex buffers, as well as binding them and configuring the drawer internally to use them.  

Note that on Wii U, this class generates a fetch shader, therefore the vertex array must only be bound after the appropriate global shader mode has been set as setting the shader mode resets bound shaders. (Reminder: binding a `Shader` instance resets the global shader mode if it's not the same as that instance's shader mode.)  

### gfx
Module for graphics-related classes that do not affect the GPU state directly (or use the classes from the gpu module to do so).  

#### `Graphics`
Class for setting global graphics states (currently: viewport and scissor).  

#### `Window`
This is a class for simplification of windowing and screen handling operations, such as operations on and retrieval of the framebuffer, as well as automatic synchronization (for a constant framerate).  
On Windows, it is a wrapper around GLFW, and, on Wii U, it is a wrapper around GX2.  
See header for more.  

Main features:  
* Variable window dimensions upon creation (although *currently*, the window is not be resize-able).  
* Availability of Color, Depth and Stencil buffers that can be cleared to custom values at any time.  
* Variable swap intervals, currently for setting how many screen refreshes to wait between each swap of front and back buffers. (Do note that on Windows, the screen refresh rate is used and, for equal framerates as on Wii U, should be 60Hz. This behavior may change in the future.)  

On Windows, the coordinate-system is changed to be compliant with GX2 and origin is set to upper left.  
However, this seems to affect scissors on Intel GPUs as they are not reversed accordingly. In case you are facing this issue, try defining the macro `RIO_WIN_GL_SCISSOR_INVERTED`.  

Currently, there is no way to trigger an exit from the code itself, but it will be added eventually.  
Moreover, do note that the main loop does not return **on Wii U** as the Window termination code calls `exit()` directly, as Cafe OS lets you not to worry about freeing resources.  
Therefore, if you have code you are expecting to run at the end of the application, do not rely on that. (This behavior may change in the future.)  

#### `LookAtCamera`
Self-explanatory class for a look-at camera. See header for more.  
(Look-around camera may be added later for debugging purposes.)

#### `OrthoCamera`
Camera class for easier usage with `OrthoProjection`. See header for more.  

#### `OrthoProjection`
Self-explanatory class for orthographic projection. See header for more.  

#### `PerspectiveProjection`
Self-explanatory class for perspective projection. See header for more.  

#### `Color4f`
Basic class for floating-point RGBA colors.  

#### `PrimitiveRenderer`
Class for simple and easy rendering of primitives (single function call per primitive). Copied directly from sead and updated to use RIO gpu wrappers.  

Before rendering, the camera and projection should be set and model matrix reset if needed.  
Draw calls should be enclosed between a `begin()` call and an `end()` call. If there are multiple consecutive draw calls, `begin()` and `end()` do not need to be called for each one, but right before the first one and after the last one.  

Provided primitive types are:
* Quad (with or without texture). (TODO: configurable texture sampler)  
* Box (quad with only the edges drawn).  
* Cube.  
* Wire Cube (cube with only the edges drawn).  
* Lines.  
* Sphere.  
* Disk.  
* Circle.  
* Cylinder.  
* X-Y-Z Axes.  

### gfx/lyr
Submodule of gfx, provided for layered rendering.  

#### `IDrawable`
Base interface for a drawable object, that is, an object with "draw methods".  

#### `DrawMethod`
Class for holding a pointer to a draw function, alongside a pointer to its owner object, for use with the `Renderer`.  (TODO: Handle priority better and variable priority)  

#### `Layer`
Class for separating the rendering process into several phases, i.e., “layers”. See header for more.  
Layers can intersect or be made to strictly appear on top of each other (regardless of objects' Z ordering), by setting their clear settings.  
`DrawMethod`s can be added to layers to be called when the layer is drawn.

#### `Renderer`
Class for holding and rendering layers. See header for more.  

### gfx/mdl
Submodule of gfx, provided with a *simple* custom model format for easier rendering of models exported from common 3D modelling applications.  
Some scripts are provided in the repository for generating the custom format from OBJ, with hopes of a proper model editor tool in the future. (TODO: tutorial)  

#### `Mesh`
Class represents a runtime polygon mesh instance, a collection of vertices, edges and triangular faces to define the shape of an object. A material can be assigned to it to define its shader parameters.  

#### `Material`
Class representing a runtime material instance, which can be assigned to multiple meshes.  
A material is a collection of parameters passed to the shader when rendering a mesh. These parameters are:  
* Shader filename.  
* List of texture filenames and their equivalent shader sampler names.  
* Sampler parameters for each texture sampler.  
* List of model-specific uniform variables and their default values.  
* List of model-specific uniform blocks and their default values.  
* Render state to apply before rendering mesh (maps to `RenderState` class).  

#### `Model`
Class representing a runtime model instance, which is just a collection of meshes and their materials. When a transformation is applied to it, the same transformation is applied accordingly to all meshes contained within it.  

### gfx/mdl/res
Submodule of gfx/mdl which contains the structures serialized in the custom model resource format.  
See headers for specifications.  

#### `ModelCacher`
Model resource cache manager class. See header for more.  

Model resource files are expected to be ***relative to the `models` folder on the default file device***.  
Appended extension is `_LE.rmdl` on Windows and `_BE.rmdl` on Wii U.  

### math
Module for math-related structures and utilities.  

#### `Math<T>`
Class for general math-related functions with provided platform-specific implementations.

#### `rio_MathTypes.h`
This header provides basic structures for vectors and matrices of different dimensions.

#### `Vector{n}<T>`
Classes storing `n` components of `T`, with useful mathematical operations (e.g. addition, scaling, calculating length, normalization, etc.) and provided platform-specific implementations.  

TODO: Note that, on Wii U, vectors use paired-singles. However, their usage may not be extremely efficient (especially when compile-time optimizations are expected) and should be investigated further.  

#### `Quat<T>`
Class storing 4 components of `T`, representing a complex number (quaternion) commonly used for rotation in 3D space, with useful mathematical operations (e.g. multiplication, normalization, etc.) and provided platform-specific implementations.  

TODO: Note that, on Wii U, quaternions use paired-singles. However, their usage may not be extremely efficient (especially when compile-time optimizations are expected) and should be investigated further.  

#### `Matrix{n}{m}<T>`:
Classes for storing `n` (rows) x `m` (columns) matrices of `T`, with basic matrix operations (e.g. addition, multiplication, transformations i.e. scaling, rotation and translation).  

### task
Oh boy. This is the module I had the least time available to implement and the end result was a sloppy mess with lots of unfinished parts. (Very barebones) copy from sead.  

TODO: Explain this thing better and actual planned features...  

#### `ITask`
Base class for a task that will be executed on every frame.  

The task’s lifespan begins with `prepare()` being called; it loads and prepares what is needed for the task to be executed. `enter()` is called on the next frame in case more initialization is needed. (There is an intended use for this, but currently not with my sloppy implementation, so this is completely useless...)  
`calc()` handles task execution on each frame until the task is terminated by the user.  
`exit()` is automatically called before the task is terminated to destroy the task’s resources.  

#### `TaskMgr`
This class oversees task operations such as task creation, execution, deletion requests and destruction. It handles changing the task’s state and actions to be taken based on that state (also currently implemented in a sloppy way). It also keeps a list of all current tasks.  

To create a task, call `createTask<T>()` (`T`: task class type).  
For immediate termination of a task, call `destroyTask()`.  
Use `requestDestroyTask()` when a task is no longer needed, but immediate termination is not required (such tasks are terminated at the end of the frame).  

(TODO: Task sleeping, takeover, multi-threading, etc...)

#### Root Task
The root task is the first task that will be executed when the application starts.  
It can be used to initialize the game's resources then switch to other tasks that represent scenes in the game (TODO: fader tasks). Otherwise, (especially with the way things are currently implemented... :/), everything in the can be done inside of it instead.  
