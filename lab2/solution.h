/**
 * @def Файл с входными данными<br>
 * По условию задачи: <b>"in.txt"</b>
 */
#define INPUT_TXT "in.txt"

/**
 * @def Файл с выходными данными<br>
 * По условию задачи: <b>"out.txt"</b>
 */
#define OUTPUT_TXT "out.txt"

/**
 * @def Алфавит с элеменнтами которого осуществляются перестановки
 * По условию задачи: <b>"9876543210"</b>
 */
#define ALPH "9876543210"

/**
 * @def Мощность алфавита ALPH
 */
#define ALPH_LEN 10

/**
 * @def Максимальная длина введенной строки
 */
#define MAX_STR  ALPH_LEN + 1


/**
 * @def Код успешного завершения функции
 */
#define NO_ERR 0


/**
 * @def Ошибка при открытии файла INPUT_TXT
 */
#define ERR_OPEN 1

/**
 * @def Ошибка при создании файла OUTPUT_TXT
 */
#define ERR_CREATE 2

/**
 * @def Ошибка при записи в файл OUTPUT_TXT
 */
#define ERR_PRINT 3

/**
 * @def Ошибка при чтении из файла INPUT.TXT
 */
#define ERR_SCAN 4

int set_input(char *str, int *n);
int is_correct(char *str);
void swap(char *str, int i, int j);
int permutation(char *str, int len);
