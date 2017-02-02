# TOFFANIM ENGINE

Objectives : Simple demonstration of modern real-time realistic rendering techniques. I don't really care about FPS or loading heavily complex scenes (as long as I can demonstrate at 15FPS+).
This is not a good coding examples data base or whatnot. If the idea of using this software crossed your mind at some point (I sincerely hope that you don't), DON'T USE THIS SOFTWARE FOR PRODUCTIONS

#HOW TO BUILD
(Tested only on windows for now)
- mkdir build && cd build/
- cmake ..  [-G Visual Studio 14 2015 Win64]
- then execute build/ToffanimEngine.sln with visual studio

/!\ IMPORTANT /!\ If the build fails, it is surely because you don't have "git" in your PATH variable,
so you may have to update the submodules by hand with :
git submodule update --init -- lib/glm
git submodule update --init -- lib/glfw
git submodule update --init -- lib/assimp
git submodule update --init -- lib/imgui
git submodule update --init -- lib/stb

TODO :
- Add Models loading and rendering
- Add Materials pipeline (PBR + IBL (only with skybox reflection at first, see S.Lagarde cubemaps for real Environnement probes))
- Add Lights creation/display
- Add Shadows ( CSM for DirectionalLight // PCSS + FTS later ? )
- Optimize Deferred Rendering (compressed normals + light frustrum volume rasterizing )  Move to clustered deferred rendering ?
- Add SSAA (FXAA, maybe TAA ?) // SSAO ( Test to implements VXAO ? Seems really interesting ) // SSR
- PostFX : lens flare, chromatic aberration, eye adaptation, vigneting,HDR, Bloom, Blur, Light shaft (maybe not in PostFX => polygonal volumetric light scattering ?)
- create simple Particles pipeline
- Tranparent/Tranluscent objects without forward rendering ?
- Add clean terrain tesselation
- Physically Based Atmospheric (Fallout 4 polygonal based ?) / Sky rendering / Clouds ( 2015 Horizon Game presentation = Perlin + worley noise and ray marching )
- Physically Based shading and physics of hair
- Clothes ?
- Optimize Driver Overhead ( Texture array / Texture atlas, MultiDrawIndirectArrays and CommandBuffers (OpenGL 4.5) )

- Editor specific things : Reflection / Code Generation / Events / Components / Modules (lumberyard talk @GDC2016)

- Get rid of all librairies ? Really if I have time, those library are good enough for demonstration purposes
- Start tinkering with Vulkan ? should be really hard and long in the beginning (AKA Memory management, using descriptors, etc) but not so hard to convert from OpenGL 4.5
- If voxelisation has been done, try do implement ParametricWaveField for sound ?

DONE :

- Add texture 2D creation
- Add FBO / VBO creation
- Add Skybox

