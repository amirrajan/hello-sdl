pkill hello-sdl

brew install flac
brew install libmikmod
brew install libogg
brew install libvorbis
brew install smpeg
brew install smpeg
brew install freetype

pushd SDL
./configure
make clean
make
sudo make install
popd

pushd ./SDL_image
./configure
make clean
make
sudo make install
popd

pushd ./SDL_mixer
./configure
make clean
make
sudo make install
popd

pushd ./SDL_ttf
./configure
make clean
make
sudo make install
popd

pushd ./Chipmunk2D
cmake .
make clean
make
sudo make install
popd
