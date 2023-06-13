#ifndef LAB5_FUNC_H
#define LAB5_FUNC_H

//Типы клеток
typedef enum cellType{
    USUAL,
    LABIRINT_ENTRY,
    LABIRINT_EXIT
} cellType;


typedef struct Pair {
    unsigned int x;
    unsigned int y;
} Pair;


#endif //LAB5_FUNC_H
