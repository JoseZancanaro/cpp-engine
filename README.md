# cpp-engine
Graphics Engine using Simple and Fast Multimedia Library 

Authors:  
Israel Efraim de Oliveira [@efraim](https://github.com/IsraelEfraim)  
José Carlos Zancanaro [@zancanaro](https://github.com/JoseZancanaro)  

## Running

### Build

```shell
$ conan remote add <NAME> https://api.bintray.com/conan/bincrafters/public-conan 
```
```shell
$ conan profile new default --detect 
$ conan profile update settings.compiler.libcxx=libstdc++11 
```
```shell
$ cd build
$ conan install .. --build=missing
```
```shell
(win)
$ cmake .. -G "Visual Studio 16" -DCMAKE_BUILD_TYPE=Release
$ cmake --build . --config Release

(linux, mac)
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

### Execute

```shell
$ ./cmake-build-release/meme-engine
```