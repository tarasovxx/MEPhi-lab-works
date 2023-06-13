#ifndef LAB3_ERROR_H
#define LAB3_ERROR_H

enum Errors {
	KEY_NOT_FOUND = -1,
    OK = 0,
    DUPLICATE_KEY,
    TABLE_OVERFLOW
};

const char *errMsgs[] = {"Ok", "Duplicate key", "Table overflow"};


#endif //LAB3_ERROR_H
