/**
 * @mainpage Лабораторная работа №1.
 * Перевод записи вещественного числа между системами счисления.
 */

#include <stdio.h>
#include "solution.h"


/**
 * @function Точка входа в программу. Объявляются переменные, из файла
 * INPUT_TXT задаются начальные значение, осуществляется проверка
 * на корректность. В случае успеха осеществляется перевод числа x
 * из системы счисления с основанием base1 в систему счисления с
 * основанием base2. Если проверка не пройдена, то в файл
 * OUTPUT_TXT выводится сообщение о некорректности входных данных
 * "bad input"
 * @return 0 - если не было ошибок со стороны файловой системы
 * @return 1 - если не удалось открыть файл INPUT_TXT
 * @return 2 - если не удалось создать файл OUTPUT_TXT
 * @return 3 - если не удалось ни открыть, ни создать нужный файл
 */
int main(void)
{
    int err = 0; /*Индикатор ошибки*/
    char x[MAX_LENGTH] = "";
    int b1 = 0, b2 = 0;

    err += get_input(x, &b1, &b2);

    if (check_input(x, &b1, &b2))
    {
        int h = 0, i = 0;
        convert(x, &b1, &b2);
        h = str_len(x);

        /*Дополняем 12-нулями, если было подано вещественное число
        вида 00000.0*/
        if (x[h-1] == '.')
        {
            for (i = 0; i < MAX_DEPTH; i++)
            {
                x[h + i] = '0';
            }
        }

        err += set_output(x);
    }
    else
    {
        err += set_output("bad input");
    }

    return err;
}


/**
 * @function Реализация логики преобразования:
 * <ol>
 * <li> Если переданные основания равны - возвращаем данное число
 * <li> Если исходное основание не равно 10 - переводим число в
 * 10-ую систему счисления
 * <li> Из 10-ой системы счисления осуществляем перевод в
 * base2-систему
 * </ol>
 * @param value - число, которое необходимо преобразовать
 * @param base1 - основание исходной системы счисления
 * @param base2 - основание результирующей системы счисления
 */
void convert(char *value, int *base1, int *base2)
{
    if ((*base1 != *base2) && (*base1 != 10))
    {
        to_dec(value, base1);
    }

    if ((*base1 != *base2) && (*base1 == 10))
    {
        from_dec(value, base2);
    }
}


/**
 * @function Осуществляется преобразование числа value из base-системы
 * счисления в 10-ую систему. Сначала, переводим целую часть, она
 * будет присутствовать всегда, так как первым символом не может
 * быть точка. Затем, если в исходном числе не было вещественной
 * части, то заменяем значение value на результат преобразования.
 * Иначе ставим точку и преобразовываем вещественную часть и
 * заменяем value на сумму результатов преобразований целой и
 * вещественной частей. Указываем, что теперь число записано в 10-й
 * системе счисления.
 * @param value - число, которое необходимо преобразовать
 * @param base - основание исходной системы счисления
 */
void to_dec(char *value, int *base)
{
    int dot = get_dot(value), i = 0;
    int fl = (dot == str_len(value)); /*Наличие вещественной части*/
    long rez1 = to_int(value[0]);
    double rez2 = 0.0;

    /*Преобразуем целую часть*/
    for (i = 1; i < dot; i++)
    {
        rez1 *= *base;
        rez1 += to_int(value[i]);
    }

    /*При наличии вещественной части выполняем ее преобразование*/
    if (fl)
    {
        sprintf(value,"%li", rez1);
    }
    else
    {
        for (i = dot + 1; i < str_len(value); i++)
        {
            rez2 += to_int(value[i]) * double_pow(*base, -i + dot);
        }
        sprintf(value, "%.13f", rez1 + rez2);
    }

    /*Теперь число в 10-й системе счисления*/
    *base = 10;
}


/**
 * @function Осуществляется преобразование char в int
 * @param char - символ
 * @return Числовой эквивалент для введенного числа:
 * <ul>
 * <li> 0 .. 9 - для цифр
 * <li> 10 - 15 - для символов 'a'..'f', независимо от их регистра
 * <li> -1 - для точки
 * </ul>
 */
int to_int(char c)
{
    int x = (c - '0') % 39;

    if (c == '.')
    {
        x += 1;
    }

    if ((c >= 'A') && (c <= 'F'))
    {
        x -= 7;
    }

    return x;
}


/**
 * @function Преобразование числа в соответствующий ему символ
 * @param x - число от 0 до 15
 * @return Символьный эквивалент поданного числа:
 * <ul>
 * <li> '0' .. '9' - для цифр
 * <li> 'a' .. 'f' - для чисел 10-15
 * </ul>
 */
char to_char(int x)
{
    char h = (char)(x + '0');

    if ((x >= 10) && (x <= 15))
    {
        h += 39;
    }

    return h;
}


/**
 * @function Возвращает позицию точки в числе
 * @param value - число, для которого надо найти позицию
 * @return i - позицию символа '.'
 * @return str_len - длину переданной строки
 */
int get_dot(char *value)
{
    int dot = str_len(value), i = 0;

    while ((value[i] != '.') && (i < dot))
    {
        i++;
    }

    return i;
}


/**
 * @function Возвращает целую часть переданного числа
 * @param value - число, для которого необходимо выделить целую часть
 * @param dot - позиция точки
 * @return int_part - целая часть числа
 */
long get_dec_int(char *value, int dot)
{
    long int_part = 0, i = 0;

    for (i = 0; i < dot; i++)
    {
        int_part *= 10;
        int_part += to_int(value[i]);
    }

    return int_part;
}


/**
 * @function Получение вещественной части переданного числа
 * @param value - число, у которого необходимо выделить дробую часть
 * @param dot - позиция точки
 * @return frac_part - вещественная част числа
 * @return 0 - если передано целое число
 */
double get_dec_frac(char *value, int dot)
{
    double frac_part = 0.0;
    int i = 0;

    for (i = dot + 1; i < str_len(value); i++)
    {
        frac_part += to_int(value[i]) * double_pow(10, -i + dot);
    }

    return frac_part;
}


/**
 * @function Вычисление дляны для переданной строки
 * @param str - строка, для которой необходимо вычислить длину
 * @return len - длина строки
 */
int str_len(char *str)
{
    int len = 0;

    while(str[len] != '\0')
    {
        len++;
    }

    return len;
}


/**
 * @function Меняет местами символы с 0 по end для заданной строки
 * @param str - строка, фрагмент которой необходимо инвертировать
 * @param end - конечная позиция инвертирования
 */
void str_revert(char *str, int end)
{
    int i = 0;

    for (i = 0; i < end / 2; i++)
    {
        /*swap без выделения дополнительной памяти*/
        str[i] += str[end - i - 1];
        str[end - i - 1] = str[i] - str[end - i - 1];
        str[i] = str[i] - str[end - i - 1];
    }
}


/**
 * @function Возведение числа base в отрицательную степень pow,
 * @param base - основание степени
 * @param exp - показатель степени
 * @return вещественное число в диапазоне [0..1]
 */
long double double_pow(int base, int exp)
{
    long rez = 1;
    int i = 0;

    for (i = 0; i < -exp; i++)
    {
        rez *= base;
    }

    return 1.0 / rez;
}


/**
 * @function Преобразование числа из 10-й системы счисления в систему
 * счисления с основанием base. Перевод включает в себя три этапа:
 * подготовка, переод целой и перевод дробной частей. На этапе
 * подготовки вычисляется позиция точки и факт ее наличия, число
 * представляется в виде целой и веществонной частей. На втором
 * этапе преобразуется целая часть числа и заисывается в строку.
 * Затем, из-за ограничений алгоритма полученное значение
 * необходимо записать с обратным порядком цифр. Если в числе была
 * вещественная часть, то ставится точка. Оставшаяся часть строки
 * зануляется. На третьем этапе происходит перевод вещественной
 * части числа в требуемую систему счисления и запись результата в
 * строку.
 * @param value - число в 10-й системе счисления
 * @param base - основание конечной системы счисления
 */
void from_dec(char *value, int *base)
{
    int dot = get_dot(value), len = 0, dep = 0, g = 0;
    int fl = (dot != str_len(value)); /*Наличие дробной части*/
    long rez1 = get_dec_int(value, dot);
    long double rez2 = get_dec_frac(value, dot);

    while (rez1 >= *base)
    {
        value[len++] = to_char((int)(rez1 % (*base)));
        rez1 /= *base;
    }

    /*Последний символ не обрабатывается в цикле*/
    value[len++] = to_char((int)rez1);
    str_revert(value, len);

    if (fl)
    {
        value[len++] = '.';
    }

    /*Зануление строки*/
    for (g = len; g < MAX_LENGTH; g++)
    {
        value[g] = '\0';
    }

    while ((dep < MAX_DEPTH) && (rez2 > double_pow(10, -12)))
    {
        rez2 *= *base;
        g = (int)(rez2 + 5 * double_pow(10, -13));

        if (g >= *base)
        {
            g = (int)rez2;
        }

        rez2 -= g;
        value[len++] = to_char(g);
        dep++;
    }
}


/**
 * @function Считывание входных данных из файла INPUT_TXT
 * @param value - заданное число
 * @param base1 - исходная система счисления
 * @param base2 - конечная система счисления
 * @return 0 - функция сработала без ошибок
 * @return 1 - если произошла ошибка при открытии файла
 */
int get_input(char *value, int *base1, int *base2)
{
    int err = 0; /*Индикатор ошибки*/
    FILE *f_in;

    if ((f_in = fopen(INPUT_TXT, "r")) != NULL)
    {
        fscanf(f_in, "%i %i\n%s", base1, base2, value);
        fclose(f_in);
    }
    else
    {
        err = 1;
    }

    return err;
}


/**
 * @function Вывод ответа в файл OUTPUT_TXT
 * @param x - строка, содержащая ответ
 * @return 0 - функция сработала без ошибок
 * @return 1 - если произошла ошибка при создании файла
 */
int set_output(char *x)
{
    int err = 0; /*Индикатор ошибки*/
    FILE *f_out;

    if ((f_out = fopen(OUTPUT_TXT, "w")) != NULL)
    {
        fprintf(f_out, "%s", x);
        fclose(f_out);
    }
    else
    {
        err = 2;
    }

    return err;
}


/**
 * @function Проверка исходных данных на корректность
 * @param x - заданное число
 * @param base1 - основание исходной системы счисления
 * @param base2 - основание конечной системы счисления
 * @return 1 - если исходные данные корректны
 * @return 0 - в противном случае
 */
int check_input(char *x, int *base1, int *base2)
{
    int i = 0;
    int dot_count = 0; /*Количество точек в записи числа*/
    /*Корректность представления в исходной системе счисления*/
    int incorrect = 0;
    /*MIN_BASE <= base1 <= MAX_BASE*/
    int bad_base1 = (*base1 < MIN_BASE) || (*base1 > MAX_BASE);
    /*MIN_BASE <= base2 <= MAX_BASE*/
    int bad_base2 = (*base2 < MIN_BASE) || (*base2 > MAX_BASE);
    /*Наличие точек первым и/или последним символом в строке*/
    int w_dots = ((x[str_len(x) - 1] == '.') || (x[0] == '.'));

    while ((!incorrect) && (i < str_len(x)))
    {
        if (x[i] == '.')
        {
            dot_count++;
        }

        if (to_int(x[i]) >= *base1)
        {
            incorrect = 1;
        }

        i++;
    }

    return !(incorrect || w_dots || (dot_count > 1) ||
            bad_base1 || bad_base2);
}
