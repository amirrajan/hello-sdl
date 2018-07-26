pkill hello-sdl

clang *.c -I./ \
      -I./SDL/include \
      -I./SDL_image \
      -I./SDL_mixer \
      -I./SDL_ttf \
      -I./Chipmunk2D/include/chipmunk \
      -o ./hello-sdl -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

echo "lit."
