#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "hash.h"

unsigned long long hash(const char *s) {
    unsigned long long h = INT_MAX;
    for (int i = 0; i < strlen(s); ++i) {
        h = 37 * h + s[i];
    }
    return h;
}






