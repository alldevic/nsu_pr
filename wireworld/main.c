#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

//Реализация оффлайного режима. 1 - вывода в консоль не будет
#define OFFLINE 0

/* Константы для обозначения состояний клеток автомата:
 * EMPTY - пустая клетка
 * HEAD - голова электрона
 * TAIL - хвост электрона
 * WIRE - провод, по которому движется электрон
 */
typedef enum CellStates {
    EMPTY = '=', HEAD = '*', TAIL = '-', WIRE = '#'
} State;

//Реализация декодирования поля из строки формата RLE
char *rle_decode(char *str) {
    //newstr - строка для хранения выходной последовательности
    //itmp - строка для вычисления длины числа i
    char *newstr = calloc(2 * strlen(str), sizeof(char)), c, itmp[3];
    //Если не удалось выделить память - возвращаем NULL
    if (!newstr)
        return NULL;
    int i, j;

    //Пока можем считать пары вида числа-символ
    while (sscanf(str, "%d%c", &i, &c) == 2) {
        //Вычисляем длину числа из этой пары
        sprintf(itmp, "%d", i);
        //И сдвигаем указатель на текующий символ в строке
        str += strlen(itmp) + 1;
        //Затем дописываем в выходную строку нужное количество символов
        for (j = 0; j < i; j++)
            sprintf(newstr, "%s%c", newstr, c);
    }
    //Возвращаем готовую строку
    return newstr;
}

//Кодирование поля в строку формата RLE
char *rle_encode(char *str) {
    //Счетчик текущего символа
    int cnt = 0;
    //encode - итоговая строка, tmp - временная строка для формирования пар число-символ
    char smb, *encode = calloc(strlen(str), sizeof(char)), *tmp = calloc(strlen(str), sizeof(char));
    //Если не удалось выделить память - возвращаем NULL
    if ((!encode)||(!tmp))
        return NULL;

    smb = str[0];

    //Проходим по всем символам строки
    for (int i = 0; i <= strlen(str); i++) {
        //Если текущий совпадает с предыдущим
        if (str[i] == smb)
            //Увеличиваем значение счетчика
            cnt++;
        else {
            //Иначе формируем пару число-символ
            sprintf(tmp, "%d%c", cnt, smb);
            //И дописываем ее в итоговую строку
            strcat(encode, tmp);
            //Затем меняем старый символ на новый
            smb = str[i];
            //И присваиваем значение счетчика 1
            cnt = 1;
        }
    }
    //Возвращаем готовую строку
    return encode;
}

//Генерация следующего поколения (new) из текущего (old)
void nextWorld(char *old, char *new, int height, int width) {
    int i;
    //Проходим все клетки
    for (i = 0; i < width * height; i++) {
        //И делаем замену согласно правилам:
        switch (old[i]) {
            case EMPTY:
                //Пустая клетка остается пустой
                new[i] = EMPTY;
                break;
            case TAIL:
                //Хвост становится проводом
                new[i] = WIRE;
                break;
            case HEAD:
                //Голова - хвостом
                new[i] = TAIL;
                break;
            case WIRE: {
                //hc - количество голов в окрестности данной клетки
                //Отнимая width делаем поле тороидальным
                int hc = (old[i - width - 1] == HEAD) + (old[i - width] == HEAD) + (old[i - width + 1] == HEAD) +
                         (old[i - 1] == HEAD) +                                    (old[i + 1] == HEAD) +
                         (old[i + width - 1] == HEAD) + (old[i + width] == HEAD) + (old[i + width + 1] == HEAD);
                //Если количество голов в окрестности клетки равно 1 или 2, то текущая клетка становится головой
                //Иначе ее значение не изменяется
                new[i] = (hc == 1 || hc == 2) ? HEAD : WIRE;
                break;
            }
            default:
                break;
        }
    }
    new[i] = old[i];
}

//Точка входа в программу
//Вызов: ./wireworld <входной файл> <выходной файл> <количество итераций>
int main(int argc, char **argv) {
    //Открываем входной файл для чтения
    FILE *file = fopen(argv[1], "r");
    //Если не удалось открыть файл - завершаем программу с кодом ошибки 1
    if (!file)
        return 1;
    //Считываем длину и ширину поля
    int height, width;
    //Если не удалось считать один из параметров - завершаем программу с 2
    if (fscanf(file, "%d", &height) < 0)
        return 2;
    if (fscanf(file, "%d", &width) < 0)
        return 2;
    //Выделяем память под 2 поля автомата: для текущего и для нового поколения
    char *str = malloc(height * width * sizeof(char));
    char *tmpstr = malloc(height * width * sizeof(char));
    //Если не удалось выделить память - завершаем программу с кодом ошибки 3
    if (!str || !tmpstr)
        return 3;
    //Если не удалось считать поле - возвращаем 4
    if (fscanf(file, "%s", str) < 0)
        return 4;
    //Закрываем файл
    fclose(file);

    //Записываем количество поколений в переменную
    int count = atoi(argv[3]);

    //Расшифровываем RLE последовательность
    str = rle_decode(str);
    //Если расшифровка провалилась - возвращаем 5
    if (!str)
        return 5;

    //Если выключен оффлайновый режим
    if (!OFFLINE) {
        //То проходим по всем клеткам поля
        for (int i = 0; i < height * width; i++) {
            //Обрабатываем конец строки поля
            if ((i % width == 0) && (i != 0))
                printf("\n");
            //И выводим значение текущей клетки
            printf("%c", str[i]);
        }
        //Делаем отступ для следующего поколения
        printf("\n");
    }

    //Пока число поколений для вывода > 0
    while (count > 0) {
        //Если выключен оффлайновый режим - делаем отсуп
        if (!OFFLINE)
            printf("\n");
        //Записываем в tmpstr новое поколение поля
        nextWorld(str, tmpstr, height, width);

        //Проходим по всем клеткам строки
        for (int i = 0; i < height * width; i++) {
            //Делаем новое поколение - текущим
            str[i] = tmpstr[i];
            //Если есть необходимость - переводим курсор на новую строку
            if (!OFFLINE && (i % width == 0) && (i != 0) )
                printf("\n");
            //И выводим значение текущей ячейки
            if (!OFFLINE)
                printf("%c", str[i]);
        }
        //Делаем отступ
        if (!OFFLINE)
            printf("\n");

        //Уменьшаем количество требуемых для генерации поколений
        count--;
    }

    //Открываем выходной файл для записи
    file = fopen(argv[2], "w");
    //Если не удалось открыть - завершаем программу с кодом ошибки 6
    if (!file)
        return 6;
    //Выводим значения длины, ширины  и сжатую схему поля
    fprintf(file, "%d %d\n%s", height, width, rle_encode(str));
    //Закрываем выходной файл
    fclose(file);
    return 0;
}
