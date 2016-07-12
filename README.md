# TOFFANIM ENGINE

#HOW TO BUILD
(Tested only on windows for now)
- mkdir build && cd build/
- cmake ..  [-G Visual Studio 12 2013 Win64]
- then execute build/ToffanimEngine.sln with visual studio

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

