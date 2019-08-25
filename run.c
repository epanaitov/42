#include <stdio.h>
#include "eval_expr.h"
#include <assert.h> 

void main() {

    // test cases
    assert(eval_expr("2+3") == 5.0);
    assert(eval_expr("2+3*(2+1)") == 11.0);
    assert(eval_expr("2-3/(2+1)") == 1.0);
    assert(eval_expr("(2-3)/(3+1)") == -0.25);
    assert(eval_expr("3 + 42 * (1 - 2 / (3 + 4) - 1 % 21) + 1") == -8.0);
}

int their_main(int ac, char **av) {
    if (ac > 1) {
        //ft_putnbr(eval_expr(av[1]));
        //ft_putchar('\n');
    }
    return(0);
}