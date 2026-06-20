## Recriar o build e executar

```
Remove-Item -Recurse -Force build
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="C:/msys64/ucrt64/bin/ninja.exe"
cmake --build build
cd build
./passeio.exe
```
