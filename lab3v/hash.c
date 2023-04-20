#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

int hash(const char *s) {
    int l = strlen(s);
    int h = 0;
    for (int i = 0; i < l; ++i) {
        h = 37 * h + s[i];
    }
    return abs(h);
}


