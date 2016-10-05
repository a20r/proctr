mkdir third_party
cd third_party

git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake ..
make
sudo make install

cd ../..

git clone https://github.com/rogersce/cnpy
cd cnpy
mkdir build
cd build
cmake ..
make
sudo make install

cd ../../../
