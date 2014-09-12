#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

int calculate(char* pbegin, char** pend, int* res, BOOL deep, int level) {

    int value;
    int d;
    char* p = pbegin;
    *res = 0;

    while (1) {
        fprintf(stderr, "%d: *p = \'%c\' (0x%02x); level = %d\n", __LINE__, *p, (unsigned) *p, level);
        if ('0' <= *p && *p <= '9') {
            d = ((int) *p) - 0x30;
            fprintf(stderr, "%d: d = %d\n", __LINE__, d);
            *res = 10 * *res + d;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
            ++p;
        } else if ('+' == *p) {
            if (!deep && p != pbegin) {
                *pend = p - 1;
                return 0;
            }
            if (calculate(p + 1, &p, &value, TRUE, level + 1) != 0) {
                return 1;
            }
            *res = *res + value;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
            ++p;
        } else if ('-' == *p) {
            if (!deep && p != pbegin) {
                *pend = p - 1;
                return 0;
            }
            if (calculate(p + 1, &p, &value, TRUE, level + 1) != 0) {
                return 1;
            }
            *res = *res - value;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
            ++p;
        } else if ('*' == *p) {
            if (calculate(p + 1, &p, &value, FALSE, level + 1) != 0) {
                return 1;
            }
            *res = *res * value;
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
            ++p;
        } else if ('(' == *p) {
            if (calculate(p + 1, &p, &value, TRUE, level + 1) != 0) {
                return 1;
            }
            *res = value;
            fprintf(stderr, "%d:\n", __LINE__);
            p += 2; // eat ')'
        } else if (')' == *p) {
            fprintf(stderr, "%d:\n", __LINE__);
            if (level == 1) {
                fprintf(stderr, "%d:\n", __LINE__);
                return 1;
            }
            if (NULL != pend) {
               *pend = p - 1; // return '0' to the upper level
            }
            return 0;
        } else if ('\0' == *p) {
            fprintf(stderr, "%d:\n", __LINE__);
            if (NULL != pend) {
               *pend = p - 1;
            }
            return 0;
        } else {
            return 1;
        }
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

    if (calculate(argv[1], NULL, &result, TRUE, 1) == 0) {
        printf("%d\n", result);
    } else {
        fprintf(stderr, "Error\n");
    }

    return 0;
}
