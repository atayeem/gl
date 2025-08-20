#include <unistd.h>

#include "api.hpp"

int main() {
    preload();
    

    while (true) {
        draw();
        usleep(1000000 / 60);
    }
    return 0;
}