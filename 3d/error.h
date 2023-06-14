#ifndef LAB3G_ERROR_H
#define LAB3G_ERROR_H

enum Errors {
    OK = 0,
    DUPLICATE_KEY,
    TABLE_OVERFLOW
};

const char *errMsgs[] = {"Ok", "Duplicate key", "Table overflow"};

#endif //LAB3G_ERROR_H
