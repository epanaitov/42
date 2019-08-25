#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


bool belongs_to_subset(char subset[], char ch) {
    int i = 0;
    while (i < strlen(subset)) {
        if (ch == subset[i]) {
            return true;
        }
        i++;
    }
    return false;
}


bool is_operator(char ch) {
    char ops[] = "+-/*%";
    return belongs_to_subset(ops, ch);
}


bool is_parens(char ch) {
    char parens[] = "()";
    return belongs_to_subset(parens, ch);
}


int get_order(char ch) {

    if (ch == '%') {
        return 5;
    } 
    if (ch == '*') {
        return 4;
    }
    if (ch == '/') {
        return 3;
    }
    if (ch == '+') {
        return 2;
    }
    if (ch == '-') {
        return 1;
    }
}

bool is_before(char op1, char op2) {
    //printf("is_before(%c, %c)\n", op1, op2);
    return get_order(op1) >= get_order(op2);
}


double eval_expr(char *expr) {

    // This is Dijkstra's Shunting Yard 

    // tokenize expression
    int i = 0;
    char tokens[1000][50];
    int token_counter = 0;
    int token_len = 0; 
    while (i < strlen(expr)) {
        char ch = expr[i];
        if (ch == ' ') {
            i++;
            continue;
        }
        if (is_operator(ch) || is_parens(ch)) {
            if (token_len > 0) {
                // there was a number before, terminate the string
                //tokens[token_counter][token_len] = '\n';
                token_len = 0;
                token_counter++;
            }
            tokens[token_counter][0] = ch; 
            //tokens[token_counter][1] = '\n';
            token_counter++;
        } else {
            tokens[token_counter][token_len] = ch;
            token_len++;
        }
        i++;
    }

    char queue[1000][50];
    char stack[1000][50];
    int q_ptr = -1;
    int s_ptr = -1;


    i = 0;
    while (i <= token_counter) {
        // printf("%s \n", tokens[i]);
        // printf("%c \n", tokens[i][0]);
        if (is_operator(tokens[i][0])) {
            if (s_ptr >= 0) {
               //printf("%s\n", stack[s_ptr]);
                while (s_ptr >= 0 && stack[s_ptr][0] != '(' && is_before(stack[s_ptr][0], tokens[i][0])) {
                    strcpy(queue[++q_ptr], stack[s_ptr--]);
                    //printf("%s\n", stack[s_ptr+1]);
                }
            }
            strcpy(stack[++s_ptr], tokens[i]);
        } else {
            if (is_parens(tokens[i][0])) {
                if (tokens[i][0] == ')') {
                    if (s_ptr >= 0) {
                        while (s_ptr >= 0 && stack[s_ptr][0] != '(') {
                            strcpy(queue[++q_ptr], stack[s_ptr--]);
                        }
                    }
                    if (s_ptr >= 0 && stack[s_ptr][0] == '(') {
                        s_ptr--;
                    }
                } else {
                    // opening (
                    strcpy(stack[++s_ptr], tokens[i]);
                }
            } else {
                // number
                strcpy(queue[++q_ptr], tokens[i]);
            }
        }
        i++;
    }

    // i=0;
    // while (i <= q_ptr) {
    //     printf("%s,", queue[i]);
    //     i++;
    // }
    // printf("\n");

    while (s_ptr >= 0) {
        strcpy(queue[++q_ptr], stack[s_ptr]);
        s_ptr--;
    }

    i=0;
    while (i <= q_ptr) {
        printf("%s,", queue[i]);
        i++;
    }
    printf("\n");

    // compute

    i=0;
    while (i <= q_ptr) {
        //printf("%s ", queue[i]);

        // this is bug - FIXME
        if (strlen(queue[i]) == 0) {
            i++;
            continue;
        }

        if (is_operator(queue[i][0])) {
            char *right = stack[s_ptr--];
            char *left = stack[s_ptr--];
            float li = atof(left);
            float ri = atof(right);
            //printf("%f %f \n", li, ri);

            float res = 0;

            switch (queue[i][0]) {
                case '+':
                    res = li + ri;
                    break;

                case '-':
                    res = li - ri;
                    break;

                case '*':
                    res = li * ri;
                    break;

                case '/':
                    res = li / ri;
                    break;

                case '%':
                    res = (float)((int)li % (int)ri);
            }

            char resstr[50];
            sprintf(resstr, "%g", res);
            strcpy(stack[++s_ptr], resstr);
        } else {
            strcpy(stack[++s_ptr], queue[i]);
        }

        // DEBUG
        // int j = 0;
        // for (j = 0; j<=s_ptr; j++) {
        //     printf("%s ", stack[j]);
        // }
        // printf("\n");

        i++;
    }

    return atof(stack[0]);
}