$path= $PSScriptRoot + "\build"

if(Test-Path $path){
    Write-Host "Build Folder Already Exists"
}
else {
    mkdir build
    Write-Host "Build Folder Created Succesfully"
}

# mkdir -p build
cd build

Write-Host "Now Building Project"
cmake -G Ninja ${COMMON_CMAKE_CONFIG_PARAMS} ../
cmake --build . --config Debug

cd ..