#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOL unsigned int
#define FALSE 0
#define TRUE  1

int calculate_p(char* pbg, char** pend, int* res, BOOL mul_div_only,
                BOOL in_braces) {

    int tmp;
    *res = 0;
    char *p = pbg;
    while (1) {
        fprintf(stderr, "%d: *p = \'%c\' (02%02x)\n", __LINE__, *p, *p);
        if ('0' <= *p && *p <= '9') {
            *res = 10 * *res + (((int) *p) - 0x30);
            fprintf(stderr, "%d: *res = %d\n", __LINE__, *res);
            ++p;
        } if ('+' == *p) {
            if (p == pbg) {
                if (in_braces) {
                    ++p;
                } else {
                    fprintf(stderr, "%d\n", __LINE__);
                    return 1;
                }
            } else {
                if (mul_div_only) {
                    *pend = p;
                    return 0;
                }
                if (calculate_p(p + 1, &p, &tmp, FALSE, FALSE)) {
                    fprintf(stderr, "%d\n", __LINE__);
                    return 1;
                }
                *res += tmp;
            }
        } if ('-' == *p) {
            if (p == pbg) {
                if (in_braces) {
                    if (calculate_p(p + 1, &p, &tmp, TRUE, FALSE)) {
                        fprintf(stderr, "%d\n", __LINE__);
                        return 1;
                    }
                    *res = -tmp; 
                } else {
                    fprintf(stderr, "%d\n", __LINE__);
                    return 1;
                }
            } else {
                if (mul_div_only) {
                    *pend = p;
                    return 0;
                }
                if (calculate_p(p + 1, &p, &tmp, FALSE, FALSE)) {
                    fprintf(stderr, "%d\n", __LINE__);
                    return 1;
                }
                *res -= tmp;
            }
        } if ('*' == *p) {
            if (p == pbg) {
                fprintf(stderr, "%d\n", __LINE__);
                return 1;
            } else {
                if (calculate_p(p + 1, &p, &tmp, TRUE, FALSE)) {
                    fprintf(stderr, "%d\n", __LINE__);
                    return 1;
                }
                *res *= tmp;
            }
        } if ('(' == *p) {
            if (calculate_p(p + 1, &p, &tmp, FALSE, TRUE)) {
                fprintf(stderr, "%d\n", __LINE__);
                return 1;
            }
            *res = tmp;
            ++p; // eat ')'
        } if (')' == *p) {
            *pend = p; // return ')' to the caller
            return 0;
        } if (' ' == *p || '\n' == *p || '\r' == *p) {
        } if ('\0' == *p) {
            fprintf(stderr, "%d\n", __LINE__);
            *pend = p;
            break;
        } else {
        }
    }

    return 0;
}

int calculate(char* pbg, int* res) {

    char* dummy;
    return calculate_p(pbg, &dummy, res, FALSE, TRUE);
}

int main(int argc, char* argv[]) {

     if (argc != 2) {
        fprintf(stderr, "Usage:\n");
        return 1;
    }

    int result;

    fprintf(stderr, "argv[1] = \"%s\"\n", argv[1]);

    if (calculate(argv[1], &result) == 0) {
        printf("%d\n", result);
    } else {
        fprintf(stderr, "Error\n");
    }

    return 0;
}
