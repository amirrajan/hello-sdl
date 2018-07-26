brew install pkill
brew install flac
brew install libmikmod
brew install libogg
brew install libvorbis
brew install smpeg
brew install smpeg
brew install freetype

pkill hello-sdl

pushd mruby
make clean
make
popd

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
