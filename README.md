# TOFFANIM ENGINE

#HOW TO BUILD
(Tested only on windows for now)
- mkdir build && cd build/
- cmake ..  [-G Visual Studio 12 2013 Win64]
- then execute build/ToffanimEngine.sln with visual studio

/!\ IMPORTANT /!\ If the build fails, it is surely because you don't have "git" in your PATH variable,
so you may have to update the submodules by hand with :
git submodule update --init -- lib/glm
git submodule update --init -- lib/glfw
git submodule update --init -- lib/assimp
git submodule update --init -- lib/imgui
git submodule update --init -- lib/stb

TODO :
- add opti deferred (compressed normals + AABB light volume rasterizing )
- improve shadows (soft shadow (PCSS) and CSM )
- clean texture/fbo creation
- clean light creation/display
- add SSAA and SSReflection
- PostFX : lens flare
- clean particle code to do clean particle generation and rendering
- terrain tesselation

DONE :
- sun added
- add shadow for directional/point/spot light
- PostFX : HDR, Bloom, Blur, Light shaft


BUGFIX :

- when we use imgui, cursor only work inside window (deactivated for now)

