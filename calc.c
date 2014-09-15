#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE float
#define PLACEHOLDER "%f"

#define BOOL unsigned int
#define FALSE 0
#define TRUE  1

int calculate_p(char* pbg, char** pend, TYPE* res, int level,
                BOOL mul_div_only) {

    TYPE tmp;
    *res = 0;
    char *p = pbg;

    if (strlen(pbg) == 0) {
        return 1;
    }

    while (1) {
        fprintf(stderr, "%d: *p = \'%c\' (02%02x)\n", __LINE__, *p, *p);
        if ('0' <= *p && *p <= '9') {
            *res = 10 * *res + (((TYPE) *p) - 0x30);
            fprintf(stderr, "%d: *res = "PLACEHOLDER"\n", __LINE__, *res);
            ++p;
        } if ('+' == *p) {
            if (p == pbg) {
                if (level > 1) {
                    ++p;
                } else {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = 0;
                    return 1;
                }
            } else {
                if (mul_div_only) {
                    *pend = p;
                    return 0;
                }
                if (calculate_p(p + 1, &p, &tmp, level, FALSE)) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return 1;
                }
                *res += tmp;
            }
        } if ('-' == *p) {
            if (p == pbg) {
                if (level > 1) {
                    if (calculate_p(p + 1, &p, &tmp, level, TRUE)) {
                        fprintf(stderr, "%d\n", __LINE__);
                        *res = p - pbg + 1 + tmp;
                        return 1;
                    }
                    *res = -tmp; 
                } else {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = 0;
                    return 1;
                }
            } else {
                if (mul_div_only) {
                    *pend = p;
                    return 0;
                }
                if (calculate_p(p + 1, &p, &tmp, level, FALSE)) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return 1;
                }
                *res -= tmp;
            }
        } if ('*' == *p) {
            if (p == pbg) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = 0;
                return 1;
            } else {
                if (calculate_p(p + 1, &p, &tmp, level, TRUE)) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return 1;
                }
                *res *= tmp;
            }
        } if ('/' == *p) {
            if (p == pbg) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg;
                return 1;
            } else {
                if (calculate_p(p + 1, &p, &tmp, level, TRUE)) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return 1;
                }
                if (0 == tmp) {
                    fprintf(stderr, "%d\n", __LINE__);
                    return 2;
                } else {
                    *res /= tmp;
                }
            }
        } if ('(' == *p) {
            if (calculate_p(p + 1, &p, &tmp, level + 1, FALSE)) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg + 1 + tmp;
                return 1;
            }
            *res = tmp;
            ++p; // eat ')'
        } if (')' == *p) {
            if (level == 1) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg;
                return 1;
            } else {
                *pend = p; // return ')' to the caller
            }
            return 0;
        } if (' ' == *p || '\n' == *p || '\r' == *p) {
            ++p;
        } if ('\0' == *p) {
            if (level > 1) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg;
                return 1;
            } else {
                fprintf(stderr, "%d\n", __LINE__);
                *pend = p;
                break;
            }
        } else {
        }
    }

    if (1 != level) {
        *res  = 0;
        return 1;
    }

    return 0;
}

int calculate(char* pbg, TYPE* res) {

    char* dummy;
    return calculate_p(pbg, &dummy, res, 1, FALSE);
}

int main(int argc, char* argv[]) {

    int i;
    int ret;

    if (argc != 2) {
        fprintf(stderr, "Usage:\n");
        return 1;
    }

    TYPE result;

    fprintf(stderr, "argv[1] = \"%s\"\n", argv[1]);


    ret = calculate(argv[1], &result);
    if (0 == ret) {
        printf(PLACEHOLDER"\n", result);
    } else {
        fprintf(stderr, "Error\n");
        if (1 == ret) {
            fprintf(stderr, "%s\n", argv[1]);
            for (i = 0; i < result; ++i) {
                fprintf(stderr, " ");
            }
            fprintf(stderr, "^\n");
        }
    }

    return 0;
}
