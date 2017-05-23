#ifndef LAB5_ERRORS_H
#define LAB5_ERRORS_H

#include <errno.h>

/**
 * @def Macros for catching global errors
 */
#define ERR(x) {if (x) {perror(NULL); return errno;}};

#endif /*LAB5_ERRORS_H*/
