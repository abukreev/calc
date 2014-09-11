#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

int calculate(char* pbegin, char** pend, int* res, BOOL deep) {

    int value;
    int d;
    char* p = pbegin;
    *res = 0;

    while (1) {
        fprintf(stderr, "%d: *p = \'%c\' (0x%02x)\n", __LINE__, *p, (unsigned) *p);
        if ('0' <= *p && *p <= '9') {
            d = ((int) *p) - 0x30;
            fprintf(stderr, "%d: d = %d\n", __LINE__, d);
            *res = 10 * *res + d;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
        } else if ('+' == *p) {
            if (!deep) {
                *pend = p - 1;
                return 0;
            }
            if (calculate(p + 1, &p, &value, TRUE) != 0) {
                return 1;
            }
            *res = *res + value;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
        } else if ('-' == *p) {
            if (!deep) {
                *pend = p - 1;
                return 0;
            }
            if (calculate(p + 1, &p, &value, TRUE) != 0) {
                return 1;
            }
            *res = *res - value;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
        } else if ('*' == *p) {
            if (calculate(p + 1, &p, &value, FALSE) != 0) {
                return 1;
            }
            *res = *res * value;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
        } else if ('(' == *p) {
            if (calculate(p + 1, &p, &value, TRUE) != 0) {
                return 1;
            }
            *res = value;
            ++p;
        } else if (')' == *p || '\0' == *p) {
            fprintf(stderr, "%d:\n", __LINE__);
            if (NULL != pend) {
               *pend = p - 1;
            }
            return 0;
        } else {
            return 1;
        }
        ++p;
        if (NULL != pend) {
            *pend = p;
        }
        fprintf(stderr, "%d: ++p\n", __LINE__);
    }
    
    return 1;
}

int main(int argc, char* argv[]) {

     if (argc != 2) {
        fprintf(stderr, "Usage:\n");
        return 1;
    }

    int result;

    fprintf(stderr, "argv[1] = \"%s\"\n", argv[1]);

    if (calculate(argv[1], NULL, &result, TRUE) == 0) {
        printf("= %d\n", result);
    } else {
        fprintf(stderr, "Error\n");
    }

    return 0;
}
