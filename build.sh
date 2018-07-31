pkill hello-sdl

echo "build started."

cat ./app/* > ./main_ruby.rb
../mruby/bin/mrbc -Bmain_ruby ./main_ruby.rb

clang adr_game.c sdl_context.c draw.c main.c -I./ \
      -I../mruby/include \
      -I./SDL/include \
      -I./SDL_image \
      -I./SDL_mixer \
      -I./SDL_ttf \
      -I./Chipmunk2D/include/chipmunk \
      -o./hello-sdl -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lmruby -L../mruby/build/host/lib/

echo "lit."

touch ./.build-completed
