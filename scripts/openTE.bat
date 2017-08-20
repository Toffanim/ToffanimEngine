cd C:\Users\toffanim\Documents\GitHub\ToffanimEngine\build 
cmake .. -G "Visual Studio 14 2015 Win64"
cd .. && devenv build\ToffanimEngine.sln
ctags -R .
gvim src\**
