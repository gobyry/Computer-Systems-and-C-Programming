#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh" //test case chars

int main(int argc, char **argv) {
    int opt = 0;
    int stats = 0; //statistics
    double output = 0; //for function values
    double library = 0; //for math.h values
    double diff = 0; //for difference between function and math.h
    int term_output = 0; //for function term values
    int e_true = 0;
    int euler_true = 0;
    int bbp_true = 0;
    int madhava_true = 0;
    int viete_true = 0;
    int newton_true = 0;
    int h_true = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //get opt
        switch (opt) {
        case 'a': //runs all tests
            e_true = 1;
            euler_true = 1;
            bbp_true = 1;
            madhava_true = 1;
            viete_true = 1;
            newton_true = 1;
            break;
        case 'e': //runs e test
            e_true = 1;
            break;
        case 'b': //runs bailey test
            bbp_true = 1;
            break;
        case 'm': //runs madhava test
            madhava_true = 1;
            break;
        case 'r': //runs euler test
            euler_true = 1;
            break;
        case 'v': //runs viete test
            viete_true = 1;
            break;
        case 'n': //runs newton test
            newton_true = 1;
            break;
        case 's': //enables statistic printing
            stats = 1;
            break;
        case 'h': //display help message
            h_true = 1;

        default: //program default
            printf("SYNOPSIS\n");
            printf("   A test harness for the small numerical library.\n");
            printf("\n");
            printf("USAGE\n");
            printf("   ./mathlib-test [-aebmrvnsh]\n");
            printf("\n");
            printf("OPTIONS\n");
            printf("  -a   Runs all tests.\n");
            printf("  -e   Runs e test.\n");
            printf("  -b   Runs BBP pi test.\n");
            printf("  -m   Runs Madhava pi test.\n");
            printf("  -r   Runs Euler pi test.\n");
            printf("  -v   Runs Viete pi test.\n");
            printf("  -n   Runs Newton square root tests.\n");
            printf("  -s   Print verbose statistics.\n");
            return 1;
            break;
        }
    }
    if (h_true == 1) {
        printf("SYNOPSIS\n");
        printf("   A test harness for the small numerical library.\n");
        printf("\n");
        printf("USAGE\n");
        printf("   ./mathlib-test [-aebmrvnsh]\n");
        printf("\n");
        printf("OPTIONS\n");
        printf("  -a   Runs all tests.\n");
        printf("  -e   Runs e test.\n");
        printf("  -b   Runs BBP pi test.\n");
        printf("  -m   Runs Madhava pi test.\n");
        printf("  -r   Runs Euler pi test.\n");
        printf("  -v   Runs Viete pi test.\n");
        printf("  -n   Runs Newton square root tests.\n");
        printf("  -s   Print verbose statistics.\n");
        printf("  -h   Display program synopsis and usage.\n");
    }
    if (e_true == 1) {
        output = e();
        library = M_E;
        diff = output - library;
        if (stats == 1) {
            printf(
                "e() = %16.15f, M_E = %16.15f, diff = %16.15f\n", output, library, absolute(diff));
            term_output = e_terms();
            printf("e() terms = %d\n", term_output);
        } else {
            printf(
                "e() = %16.15f, M_E = %16.15f, diff = %16.15f\n", output, library, absolute(diff));
        }
    }
    if (euler_true == 1) {
        output = pi_euler();
        library = M_PI;
        diff = output - library;
        if (stats == 1) {
            printf("pi_euler() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
            term_output = pi_euler_terms();
            printf("pi_euler() terms = %d\n", term_output);
        } else {
            printf("pi_euler() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
        }
    }
    if (bbp_true == 1) {
        output = pi_bbp();
        library = M_PI;
        diff = output - library;
        if (stats == 1) {
            printf("pi_bbp() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
            term_output = pi_bbp_terms();
            printf("pi_bbp() terms = %d\n", term_output);
        } else {
            printf("pi_bbp() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
        }
    }
    if (madhava_true == 1) {
        output = pi_madhava();
        library = M_PI;
        diff = output - library;
        if (stats == 1) {
            printf("pi_madhava() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
            term_output = pi_madhava_terms();
            printf("pi_madhava() terms = %d\n", term_output);
        } else {
            printf("pi_madhava() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
        }
    }
    if (viete_true == 1) {
        output = pi_viete();
        library = M_PI;
        diff = output - library;
        if (stats == 1) {
            printf("pi_viete() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
            term_output = pi_viete_factors();
            printf("pi_viete() terms = %d\n", term_output);
        } else {
            printf("pi_viete() = %16.15f, M_PI = %16.15f, diff = %16.15f\n", output, library,
                absolute(diff));
        }
    }
    if (newton_true == 1) {
        for (double i = 0.0; i < 10; i += 0.10) {
            output = sqrt_newton(i);
            library = sqrt(i);
            diff = output - library;
            if (stats == 1) {
                printf("sqrt_newton(%.6f) = %16.15f, sqrt(%.6f) = %16.15f, diff = "
                       "%16.15f\n",
                    i, output, i, library, absolute(diff));
                term_output = sqrt_newton_iters();
                printf("sqrt_newton() terms = %.d\n", term_output);
            } else {

                printf("sqrt_newton(%.6f) = %16.15f, sqrt(%.6f) = %16.15f, diff = "
                       "%16.15f\n",
                    i, output, i, library, absolute(diff));
            }
        }
    }

    return 0;
}
