#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "func.h"
#include "error.h"


int D_Add(Table *t) {
    int k, rc, n, par;
    puts("Enter key: -->");
    n = getInt(&k);
    if (n == 0) return 0; //Обнаружен конец файла
    puts("Enter par (If you don't want to specify enter 0): --->");
    n = getInt(&par);
    if (n == 0) return 0;
    while ((par != 0) && (find(t, par) < 0)) {
        if (par == k) break;
        puts("There is no such key in the table yet, please enter a new one, or 0\n");
        n = getInt(&par);
        if (n == 0) return 0;
    }
    scanf("%*c");
    char *info = getStr("Enter info: ");
    if(info == NULL) return 0;
    rc = insert(t, k, par, info);
    free(info); //если элемент вставляется в таблицу - вставляется его копия
    printf(errMsgs[rc]);
    return 1;
}

int insert(Table *t, int k, int par, char* info) {
    int i = find(t, k);
    if (i >= 0) {
        return 1; // Отказ дублирования ключей
    }
    fseek(t->fd, 0, SEEK_SET);
    int msize;
    fread(&msize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
    int csize;
    fread(&csize, sizeof(int), 1, t->fd);  // Читаем размер таблицы из файла
    if (csize >= msize) {
        int f = D_Reorganization(t);
        if (msize <= f) {
            return 2; // Отказ переполнения
        }
    }
    int len = strlen(info) + 1; // Длина информации, включая нуль-байт
    fseek(t->fd, 0, SEEK_SET);
    // Записываем информацию в конец зарезервированного места в файле
    fseek(t->fd, sizeof(int) * 2 + csize * (sizeof(int) * 5), SEEK_SET);
    int busy = 1;
    fwrite(&busy, sizeof(int), 1, t->fd); // Записываем busy
    fwrite(&k, sizeof(int), 1, t->fd); // Записываем key
    fwrite(&par, sizeof(int), 1, t->fd); // Записываем parentKey
    fseek(t->fd, 0, SEEK_END);
    int offset = ftell(t->fd); // Смещение информации в файле
    fseek(t->fd, sizeof(int) * 5 + csize * (sizeof(int) * 5), SEEK_SET);
    fwrite(&offset, sizeof(int), 1, t->fd); // Записываем offset
    fwrite(&len, sizeof(int), 1, t->fd); // Записываем len
    fseek(t->fd, 0, SEEK_END);
    fwrite(info, sizeof(char), len, t->fd); // Записываем саму информацию

    csize++;
    fseek(t->fd, 0, SEEK_SET);
    fseek(t->fd, sizeof(int), SEEK_SET);
    fwrite(&csize, sizeof(int), 1, t->fd);  // Записываем значение msize в файл

    return 0; // Успех
}

//int insert(Table *t, int k, int par, char* info) {
//    int i = find(t, k);
//    if (i >= 0) {
//        return 1; // Отказ дублирования ключей
//    }
//    fseek(t->fd, 0, SEEK_SET);
//    int msize;
//    fread(&msize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
//    int csize;
//    fread(&csize, sizeof(int), 1, t->fd);  // Читаем размер таблицы из файла
//    if (csize >= msize) {
//        int f = D_Reorganization(t);
//        if (msize <= f) {
//            return 2; // Отказ переполнения
//        }
//    }
//    int len = strlen(info) + 1; // Длина информации, включая нуль-байт
//
//    // Записываем информацию в конец зарезервированного места в файле
//    fseek(t->fd, sizeof(int) + csize * (sizeof(int) * 4 + sizeof(char)), SEEK_SET);
//    int busy = 1;
//    fwrite(&busy, sizeof(int), 1, t->fd); // Записываем busy
//    fwrite(&k, sizeof(int), 1, t->fd); // Записываем key
//    fwrite(&par, sizeof(int), 1, t->fd); // Записываем parentKey
//    int offset = ftell(t->fd); // Смещение информации в файле
//    fwrite(&offset, sizeof(int), 1, t->fd); // Записываем offset
//    fwrite(&len, sizeof(int), 1, t->fd); // Записываем len
//    fwrite(info, sizeof(char), len, t->fd); // Записываем саму информацию
//
//    csize++;
//    fseek(t->fd, 0, SEEK_SET);
//    fseek(t->fd, sizeof(int), SEEK_SET);
//    fwrite(&csize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
//
//    return 0; // Успех
//}

int D_Delete(Table *t) {
    int k, n;
    puts("Enter key, which you want to delete -->");
    n = getInt(&k);
    if (n == 0) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    int i = find(t, k);
    if (i >= 0)
        delete(t, k, i);
    if (i < 0) {
        puts("This key was not found\n");
        return -1; //Такого ключа нет
    }
    return 1; //Good
}

int D_Find(Table *t) {
    int k, n, i;
    char *info = NULL;
    puts("Enter the key you want to find -->");
    n = getInt(&k);
    if(n == 0) {
        puts("You have entered an invalid value\n");
        return 0;
    }
    info = findInfo(t, k);
    if (info) {
        printf("table[%d] = info \"%s\"\n", k, info);
        free(info);
    }
    else {
        printf("Item %d was not found\n", k);
        return -1; //Не найден
    }
    return 1;
}

int find(const Table *t, int k) { //Функция поиска в таблице элемента заданного ключом
    fseek(t->fd, 0, SEEK_SET);
    int msize;
    fread(&msize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
    int csize;
    fread(&csize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
    int keySpaceSize = sizeof(int) * 5;
    for (int i = 0; i < csize; ++i) {
        fseek(t->fd, sizeof(int) * 2 + keySpaceSize * i, SEEK_SET);
        int busy;
        fread(&busy, sizeof(int), 1, t->fd);
        if (busy == 1) {
            int key;
            fread(&key, sizeof(int), 1, t->fd);
            if (key == k)
                return i;
        }

    }
    return -1;
}

int D_Show(Table *t) {
    puts("busy ||| key ||| parentKey |||   info");
    int csize, msize;
    fseek(t->fd, 0, SEEK_SET);
    fread(&msize, sizeof(int), 1, t->fd);
    fread(&csize, sizeof(int), 1, t->fd);
    int keySpaceSize = sizeof(int) * 5; // Размер записи в файле (busy, key, par, offset, len)
    fseek(t->fd, 0, SEEK_SET);
    for (int i = 0; i < csize; ++i) {
        int busy, key, par, offset, len;
        fseek(t->fd, sizeof(int) * 2 + keySpaceSize * i, SEEK_SET);
        fread(&busy, sizeof(int), 1, t->fd);
        fread(&key, sizeof(int), 1, t->fd);
        fread(&par, sizeof(int), 1, t->fd);
        fread(&offset, sizeof(int), 1, t->fd);
        fread(&len, sizeof(int), 1, t->fd);
        printf("%d \t %d \t %d \t\t", busy, key, par);
        fseek(t->fd, offset, SEEK_SET);
        char buf[1024];
        fread(buf, sizeof(char), len, t->fd);
        //fgets(buf, len, t->fd);
        printf("%s\n", buf);
    }
    return 1;
}

int findParent(const Table *t, int k) {
    fseek(t->fd, 0, SEEK_SET);
    int msize;
    fread(&msize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
    int csize;
    fread(&csize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
    int keySpaceSize = sizeof(int) * 5;
    for (int i = 0; i < csize; ++i) {
        fseek(t->fd, sizeof(int) * 2 + keySpaceSize * i, SEEK_SET);
        int busy;
        fread(&busy, sizeof(int), 1, t->fd);
        if (busy == 1) {
            int key, par;
            fread(&key, sizeof(int), 1, t->fd);
            fread(&par, sizeof(int), 1, t->fd);
            if (par == k)
                return i;
        }

    }
    return -1;
}

char *findInfo(const Table *t, int k) { //Функция поиска в таблице информации для элемента, заданного ключом
    char *info = NULL;
    fseek(t->fd, 0, SEEK_SET);  // Пропуск размеров msize и csize
    int msize, csize;
    fread(&msize, sizeof(int), 1, t->fd);
    fread(&csize, sizeof(int), 1, t->fd);

    for (int i = 0; i < csize; i++) {
        int busy;
        fread(&busy, sizeof(int), 1, t->fd);
        if (busy == 1) {
            int key;
            fread(&key, sizeof(int), 1, t->fd);
            if (key == k) {
                int offset;
                fread(&offset, sizeof(int), 1, t->fd);
                int len;
                fread(&len, sizeof(int), 1, t->fd);

                info = (char *) malloc(len);
                fseek(t->fd, offset, SEEK_SET);
                fread(info, sizeof(char), len, t->fd);

                break;
            }
        }

        // Пропуск остальных полей
        fseek(t->fd, sizeof(int) * 3, SEEK_CUR);
    }

    return info;
}

int D_Reorganization(Table *t) {
    int j = reorganize(t);
    fseek(t->fd, sizeof(int), SEEK_SET);
    int msize;
    fread(&msize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
    if (j >= msize) {
        puts("The table was reorganized successfully, but the table is still full\n");
        return -1;
    }
    puts("Table reorganization is successful\n");
    return 1;
}

int load(Table *t, char *fname) { //Загрузка таблицы из существующего файла!
//Открываем существующий файл таблицы на чтение и запись
    //fopen_s(&(t->fd), fname, "r+b");
    //fopen_s(&(t->fd), fname, "r+b");
    t->fd = (FILE *) fopen(fname, "r+b");
    if (t->fd == NULL)
        return 0;
// Файл открыт, можно читать; Считываем размер вектора
    return 1;
}

int create(Table *t, char *fname, int sz) {
// Должны создать пустую таблицу размером csize
// новый файл и зарезервировать в нём место для таблицы
    int msize = sz;
    int csize = 0;
    t->fd = fopen(fname, "w+b");
    if (t->fd == NULL) {
        return 0;
    }
    // Записываем в начало файла размер
    fwrite(&msize, sizeof(int), 1, t->fd);
    // Размер таблицы
    fwrite(&csize, sizeof(int), 1, t->fd);
    // Саму таблицу
    int k = msize;
    while (k >= 0) {
        int busy = 0;
        fwrite(&busy, sizeof(int), 1, t->fd);
        int key = 0;
        fwrite(&key, sizeof(int), 1, t->fd);
        int par = 0;
        fwrite(&par, sizeof(int), 1, t->fd);
        int offset = 0;
        fwrite(&offset, sizeof(int), 1, t->fd);
        int len = 0;
        fwrite(&len, sizeof(int), 1, t->fd);
        --k;
    }

    return 1;
}

int D_Load(Table *t) {
    int SZ;
    char *fname = NULL;
    printf("Enter file name: --> ");
    fname = getStr("");
    if(fname == NULL)
        return 0;
    if (load(t, fname) == 0) {
        printf("Enter possible vector size: -->");
        if (getInt(&SZ) == 0 || create(t, fname, SZ) == 0)
            return 0;
    }
    free(fname);
    return 1;
}


int save(Table *t) {
    int msize;
    fseek(t->fd, 0, SEEK_SET);
    fread(&msize, sizeof(int), 1, t->fd);  // Читаем значение msize из файла
    int csize;
    fread(&csize, sizeof(int), 1, t->fd);  // Читаем размер таблицы из файла
    int k = msize;
    while (k >= 0) {
        int busy;
        fread(&busy, sizeof(int), 1, t->fd);  // Читаем значение поля busy из файла
        fwrite(&busy, sizeof(int), 1, t->fd);

        int key;
        fread(&key, sizeof(int), 1, t->fd);  // Читаем значение поля key из файла
        fwrite(&key, sizeof(int), 1, t->fd);

        int par;
        fread(&par, sizeof(int), 1, t->fd);  // Читаем значение поля par из файла
        fwrite(&par, sizeof(int), 1, t->fd);

        int offset;
        fread(&offset, sizeof(int), 1, t->fd);  // Читаем значение поля offset из файла
        fwrite(&offset, sizeof(int), 1, t->fd);

        int len;
        fread(&len, sizeof(int), 1, t->fd);  // Читаем значение поля len из файла
        fwrite(&len, sizeof(int), 1, t->fd);

        --k;
    }

    fclose(t->fd);
    t->fd = NULL;
    return 1;
}


int to_int(char *s) {
    int i = strlen(s), j = 0, ans = 0;
    while (j != i) {
        ans = (ans * 10) + (s[j] - '0');
        ++j;
    }
    return ans;
}

void delTable(Table *t) {
    free(t);
}

int reorganize(Table *t) {
    int msize, csize;
    fseek(t->fd, 0, SEEK_SET);
    fread(&msize, sizeof(int), 1, t->fd);
    fread(&csize, sizeof(int), 1, t->fd);

    int i = 0, j = 0, len = csize;

    int recordSize = sizeof(int) * 5;
    int headerSize = sizeof(int) * 2;
    int recordOffset = headerSize + i * recordSize;
    int nextRecordOffset = headerSize + j * recordSize;

    while (i < len) {
        int busy, key, par, offset, recLen;
        fseek(t->fd, recordOffset, SEEK_SET);
        fread(&busy, sizeof(int), 1, t->fd);
        if (busy == 1) {
            fread(&key, sizeof(int), 1, t->fd);
            fread(&par, sizeof(int), 1, t->fd);
            fread(&offset, sizeof(int), 1, t->fd);
            fread(&recLen, sizeof(int), 1, t->fd);

            if (recordOffset != nextRecordOffset) {
                fseek(t->fd, nextRecordOffset, SEEK_SET);
                fwrite(&busy, sizeof(int), 1, t->fd);
                fwrite(&key, sizeof(int), 1, t->fd);
                fwrite(&par, sizeof(int), 1, t->fd);
                fwrite(&offset, sizeof(int), 1, t->fd);
                fwrite(&recLen, sizeof(int), 1, t->fd);
            }
            j++;
            nextRecordOffset = headerSize + j * recordSize;
        } else {
            csize--;
        }
        i++;
        recordOffset = headerSize + i * recordSize;
    }

    fseek(t->fd, sizeof(int), SEEK_SET);
    fwrite(&csize, sizeof(int), 1, t->fd);
    fflush(t->fd);

    return j;  // Возвращаем новый размер таблицы
}


int delete(Table *t, int k, int i) {
    int msize, csize;
    fseek(t->fd, 0, SEEK_SET);
    fread(&msize, sizeof(int), 1, t->fd);
    fread(&csize, sizeof(int), 1, t->fd);

    int recordSize = sizeof(int) * 5;
    int headerSize = sizeof(int) * 2;
    int recordOffset = headerSize + i * recordSize;

    int busy = 0;
    fseek(t->fd, recordOffset, SEEK_SET);
    fwrite(&busy, sizeof(int), 1, t->fd);
    fflush(t->fd);

    int j = findParent(t, k);
    if (j != -1)
        delete(t, k, j);

    fseek(t->fd, sizeof(int), SEEK_SET);
    fwrite(&csize, sizeof(int), 1, t->fd);
    fflush(t->fd);
    return 0;
}

//int delete2(Table *t, int k, int i) {
//    t->ks[i].busy = 0;
//    int j = findParent(t, t->ks[i].key);
//    while (j != -1) {
//        delete(t, t->ks[j].key, j);
//        j = findParent(t, t->ks[j].key);
//    }
//}
