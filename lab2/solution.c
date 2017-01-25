    /**
 * @mainpage Лабораторная работа №2.
 * Перечисление перестановок по алгоритму Дейкстры.
 */

#include <stdio.h>
#include "solution.h"


/**
 * @function Точка входа в программу. На начальном этапе
 * объявляются и инициализируются переменные, задаются начальные
 * значения из файла INPUT_TXT. После проверки входных данных на
 * корректность нужное количество раз вызывается функция по
 * генерации следующей перестановки и осуществляется вывод
 * полученного значения в файл OUTPUT_TXT. Если введенные данные
 * оказались некорректными, то в файл OUTPUT_TXT выводится
 * сообщение "bad input".
 * @return Код NO_ERR - если не было ошибок со стороны файловой
 * системы
 * @return Соответствующий код ошибки
 */
int main(void)
{
    FILE *f_out;
    int err = NO_ERR, n = 0, i = 0, len = 0;
    char str[MAX_STR] = "";

    err += set_input(str, &n);
    err += (f_out = fopen(OUTPUT_TXT, "w")) ? NO_ERR : ERR_CREATE;

    if ((is_correct(str)) && (!err))
    {
        while (str[len]!=0)
            len++;
        while ((i < n) && !permutation(str, len) && !err)
        {
            err += (fprintf(f_out, "%s\n", str) < 0) ? ERR_PRINT : NO_ERR;
            i++;
        }
    } else
        fprintf(f_out, "bad input");

    fclose(f_out);

    return err;
}


/**
 * @function Считывание входных данных из файла INPUT_TXT
 * @param str - заданная перестановка P
 * @param n - целое неотрицательное N - сколько перестановок
 * генерировать, начиная с P.
 * @return Код ошибки
 */
int set_input(char *str, int *n)
{
    FILE *f_in;
    int err = NO_ERR;

    if ((f_in = fopen(INPUT_TXT, "r")) != NULL)
    {
        err += (fscanf(f_in, "%s\n%i", str, n) < 0) ? ERR_SCAN : NO_ERR;
        fclose(f_in);
    } else
        err = ERR_OPEN;

    return err;
}


/**
 * @function Проверка исходных данных на корректность. В первом
 * цикле проверяется допустимость символов строки в заданном
 * алфавите ALPH, при втором проходе проверяется, есть ли в
 * заданной перестановке повторяющиеся элементы. По условию задачи
 * их быть не должно.
 * в подставновке
 * @param str - перестановка P
 * @return 1 - перестановка корректна
 * @return 0 - перестановка некорректна
 */
int is_correct(char *str)
{
    int corr = 0, i = 0, len = 0;
    int m[ALPH_LEN] = {0};

    while ((str[len]!=0) && !corr)
    {
        corr += ((str[len] < ALPH[ALPH_LEN - 1]) || (str[len] > ALPH[0]));
        m[str[len] - '0']++;
        len++;
    }

    while ((i < ALPH_LEN) && !corr)
    {
        corr += (m[i] > 1);
        i++;
    }

    return !corr;
}


/**
 * @function Замена i-го символа строки str на j-й символ
 * @param str - строка, для которой требуется замена символов
 * @param i, j - позиции заменяемых символов.
 */
void swap(char *str, int i, int j)
{
    str[i] = str[i] + str[j];
    str[j] = str[i] - str[j];
    str[i] = str[i] - str[j];
}


/**
 * @function Реализация алгоритма перестановки элементов в
 * лексикографическом порядке. При первом проходе заданной
 * перестановки P, определяем, найдется ли такой i, что  p[i] <
 * p[i+1], а элементы P[i+1]..P[n] образуют убывающую
 * последовательность. Если такого элемента не существует, то на
 * вход подана наибольшая в лексикографическом смысле перестановка
 * и выполнение процедуры можно прервать. Иначе, ищем наименьшее
 * P[j], такое, что  P[i] < P[j] и P[j] находится правее P[i], т. е.
 * j > i. Затем меняем местами P[i] и P[j]. Затем все элементы
 * правее P[i] сортируем по возрастанию. Перестановка P + 1
 * сгенерирована.
 * @param str - перестановка P
 * @param len - длина строки (количество элементов в перестановке)
 * @return 1 - заданная перестановка P является наибольшей
 * @return 0 - перестановка P + 1 успешно сгенерирована
 */
int permutation(char *str, int len)
{
    int i = len - 2, j = 0, k = 0;

    while (i >= 0 && str[i] >= str[i + 1])
        i--;

    if (i >= 0)
    {
        j = i + 1;
        while (str[i] < str[j + 1])
            j++;
        swap(str, i, j);
    }

    for (j = i + 1, k = len - 1; (j < k) && (i >= 0); j++, k--)
        swap(str, j, k);

    return i < 0;
}
