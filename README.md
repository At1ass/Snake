# Snake build

#For build game you need GCC and libncurses

#Install ncurses and buil-essentials

sudo apt install build-essntials libncurses

#Compile game

gcc hello.c snake_body.c -lncurses -o snake
