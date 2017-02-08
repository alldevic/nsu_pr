/**
 * Максимально количество цифр после запятой<br>
 * По условию задачи: <b>12</b>
 */
#define MAX_DEPTH 12

/**
 * Файл с входными данными<br>
 * По условию задачи: <b>"in.txt"</b> 
 */
#define INPUT_TXT "in.txt"

/**
 * Файл с выходными данными<br>
 * По условию задачи: <b>"out.txt"</b>
 */
#define OUTPUT_TXT "out.txt"

/**
 * Минимальое основание для системы счисления<br>
 * По условию задачи: <b>2</b>
 */
#define MIN_BASE 2

/**
 * Максимальное основание для системы счисления<br>
 * По условию задачи: <b>16</b>
 */
#define MAX_BASE 16

/**
 * Максимальная длина заданного числа<br>
 * По условию задачи: <b>13</b>
 */
#define MAX_INP_LENGTH 13

/**
 * Максимальная длина производного числа<br>
 * В условиях задачи ее описания нет
 */
#define MAX_LENGTH (MAX_INP_LENGTH-1)*(MAX_BASE/MIN_BASE)

int get_input(char *value, int *base1, int *base2);
int set_output(char *string);
int check_input(char *value, int *base1, int *base2);

void convert(char *value, int *base1, int *base2);
void to_dec(char *value, int *base);
void from_dec(char *value, int *base);
int to_int(char c);
char to_char(int x);
int get_dot(char *value);
long get_dec_int(char *value, int dot);
double get_dec_frac(char *value, int dot);

int str_len(char *str);
void str_revert(char *str, int end);
long double double_pow(int base, int exp);

