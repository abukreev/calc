#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE float
#define PLACEHOLDER "%f"

#define BOOL unsigned int
#define FALSE 0
#define TRUE  1

enum { RET_OK, RET_SYNTAX, RET_DIVZERO };

int calculate_p(char* pbg, char** pend, TYPE* res, int level,
                BOOL mul_div_only) {

    TYPE tmp;
    *res = 0;
    char *p = pbg;
    int ret;

    if (strlen(pbg) == 0) {
        return RET_SYNTAX;
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
                    return RET_SYNTAX;
                }
            } else {
                if (mul_div_only) {
                    *pend = p;
                    return RET_OK;
                }
                if ((ret = calculate_p(p + 1, &p, &tmp, level, FALSE)) != 0) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return ret;
                }
                *res += tmp;
            }
        } if ('-' == *p) {
            if (p == pbg) {
                if (level > 1) {
                    if ((ret = calculate_p(p + 1, &p, &tmp, level, TRUE)) != 0) {
                        fprintf(stderr, "%d\n", __LINE__);
                        *res = p - pbg + 1 + tmp;
                        return ret;
                    }
                    *res = -tmp; 
                } else {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = 0;
                    return RET_SYNTAX;
                }
            } else {
                if (mul_div_only) {
                    *pend = p;
                    return RET_OK;
                }
                if ((ret = calculate_p(p + 1, &p, &tmp, level, FALSE)) != 0) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return ret;
                }
                *res -= tmp;
            }
        } if ('*' == *p) {
            if (p == pbg) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = 0;
                return RET_SYNTAX;
            } else {
                if ((ret = calculate_p(p + 1, &p, &tmp, level, TRUE)) != 0) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return ret;
                }
                *res *= tmp;
            }
        } if ('/' == *p) {
            if (p == pbg) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg;
                return RET_SYNTAX;
            } else {
                if ((ret = calculate_p(p + 1, &p, &tmp, level, TRUE)) != 0) {
                    fprintf(stderr, "%d\n", __LINE__);
                    *res = p - pbg + 1 + tmp;
                    return ret;
                }
                if (0 == tmp) {
                    fprintf(stderr, "%d: Division by zero\n", __LINE__);
                    return RET_DIVZERO;
                } else {
                    *res /= tmp;
                }
            }
        } if ('(' == *p) {
            if ((ret = calculate_p(p + 1, &p, &tmp, level + 1, FALSE)) != 0) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg + 1 + tmp;
                return ret;
            }
            *res = tmp;
            ++p; // eat ')'
        } if (')' == *p) {
            if (level == 1) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg;
                return ret;
            } else {
                *pend = p; // return ')' to the caller
            }
            return RET_OK;
        } if (' ' == *p || '\n' == *p || '\r' == *p) {
            ++p;
        } if ('\0' == *p) {
            if (level > 1) {
                fprintf(stderr, "%d\n", __LINE__);
                *res = p - pbg;
                return RET_SYNTAX;
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
        return RET_SYNTAX;
    }

    return RET_OK;
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
        return EXIT_FAILURE;
    }

    TYPE result;

    fprintf(stderr, "argv[1] = \"%s\"\n", argv[1]);


    ret = calculate(argv[1], &result);
    if (RET_OK == ret) {
        printf(PLACEHOLDER"\n", result);
    } else {
        if (RET_SYNTAX == ret) {
            fprintf(stderr, "Syntax error at %d\n", (int) result);
            fprintf(stderr, "%s\n", argv[1]);
            for (i = 0; i < result; ++i) {
                fprintf(stderr, " ");
            }
            fprintf(stderr, "^\n");
        } else if (RET_DIVZERO == ret) {
            fprintf(stderr, "Divizion by zero\n");
        }
    }

    return EXIT_SUCCESS;
}
