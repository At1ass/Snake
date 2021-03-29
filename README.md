# Snake build

#For build game you need GCC and libncurses

#Install ncurses and build-essentials

sudo apt install build-essentials libncurses

#Compile game

gcc hello.c snake_body.c -lncurses -o snake
