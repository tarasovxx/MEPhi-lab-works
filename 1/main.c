#include <stdio.h>

#include "matrix.h"
#include "line.h"

int main() {
    matrix mn = {0, NULL};
    if (input(&mn) == 0) {
        printf("%s\n", "EOF!");
        return 1;
    }
    output("Source matrix\n", mn);
    task(&mn);
    output("Modified matrix\n", mn);
    erase(&mn);
    return 0;
}
