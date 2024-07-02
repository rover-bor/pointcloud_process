if [ -d "./build" ]; then
    echo "build exist, remove and remake!"
    rm -rf build
    mkdir build
else
    echo "./build do not exist, make!"
    mkdir build
fi

cd build
# cmake -DOpen3D_ROOT=${HOME}/open3d_install/lib/cmake/Open3D ..
cmake ..
make -j 12