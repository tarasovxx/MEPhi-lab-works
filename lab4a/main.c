#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "binTree.h"
#include "userFunc.h"
#include "checkFunc.h"

// Альтернативы меню для организации диалога
const char *msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Lower Bound", "6. Import file", "7. Timing" };	// список альтернатив
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);	// количество альтернатив

// массив указателей на функции - для реализации выбора функции
// порядок перечисления функций должен соответствовать
// порядку указания альтернатив в списке альтернатив
int(*fptr[])(Node **) = {NULL, D_Add, D_Find, D_Delete, D_Show, D_FindSpecial, D_Import, D_Timing};




int main() {
	Node *root = NULL;
	int rc;
	while (rc = dialog(msgs, NMsgs))
        if (!fptr[rc](&root))
            break;
	printf("That's all. Bye!\n");
	delTree(&root);
	return 0;
}
