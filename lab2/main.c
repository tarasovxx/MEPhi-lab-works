#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#define PROMPT ">>>> "
// 2 a/1/20 b/1/15 c/2/10 d/5/8 e/6/5 f/6/9

//#include "queue_list.h"
//#include "queue_vector.h" //-Dname=value флаг компиляции

//#if defined (queue_vector)
//#include "queue_vector.h"  //gcc ------- -D queue_list -o lab2
//#elif defined (queue_list)
//#include "queue_list.h"
//#endif

#include "func.h"

#define CAPACITY 3


int main() {
    int racks = 0;
    int len = 0;
    printf("Enter the number of check-in counters: ");
    //scanf("%d", &racks);
    getInt(&racks);
    printf("Enter the values in the format id/ta/ts\n");
    char *s = readline(PROMPT);
    Item *tempArr = NULL;
    input(s, &tempArr, &len);
    Queue *mas = (Queue *) calloc(racks, sizeof(Queue)); //очереди к каждой стойке  
    reading(tempArr, racks, mas, len, CAPACITY);
    queue_deleteFull(mas, racks);
    free(s);
    return 0;
}

