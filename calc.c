#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculate(char** str, int* res) {

    char* pc = *str;
    int digit;
    int val = 0;
    char last_op = '\0';
    *res = 0;

    while (1) {
        
        fprintf(stderr, "*pc = \'%c\' (%02x)\n", *pc, (unsigned char) *pc);
        
        if (*pc >= '0' && *pc <= '9') {
            digit = ((int) *pc) - 48;
            fprintf(stderr, "digit = %d\n", digit);
            val = 10 * val + digit;
            fprintf(stderr, "val = %d\n", val);
        } else if (*pc == '+' || *pc == '-' || *pc == '*' || *pc == '\0' || *pc == ')') {
            if ('\0' == last_op) {
                *res = val;
                fprintf(stderr, "*res = %d\n", *res);
            } else if ('+' == last_op) {
                *res = *res + val;
                fprintf(stderr, "*res = %d\n", *res);
            } else if ('-' == last_op) {
                *res = *res - val;
                fprintf(stderr, "*res = %d\n", *res);
            } else if ('*' == last_op) {
                *res = *res * val;
                fprintf(stderr, "*res = %d\n", *res);
            } 
            val = 0;
            last_op = *pc;
            fprintf(stderr, "last_op = \'%c\'\n", last_op);
        } else if (*pc == '(') {
            ++pc;
            if (calculate(&pc, &val) == 0) {
                fprintf(stderr, "val = %d\n", val);
                ++pc;
                continue;
            } else {
                return 1;
            }
        } else if (*pc == ' ') {
        } else {
            fprintf(stderr, "Illegal symbol \'%c\' (%02x)\n", *pc, (unsigned char) *pc);
            return 1;
        }

        if ('\0' == *pc || ')' == *pc) {
            break;
        }
        ++pc;
    }

    *str = pc;

    return 0;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage:\n");
        return 1;
    }

    int result;

    fprintf(stderr, "argv[1] = \"%s\"\n", argv[1]);

    if (calculate(&argv[1], &result) == 0) {
        printf("= %d\n", result);
    } else {
        fprintf(stderr, "Error\n");
    }

    return 0;
}
