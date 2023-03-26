/*
 * This program is designed to test SANE's FX2C (extended-to-comp)
 * conversion function.  It can demonstrate a bug where certain
 * values are converted incorrectly.
 *
 * This can be run as a S16 program or a shell executable.
 *
 * If run as a shell executable and given a command-line argument,
 * it will set most bytes of the SANE direct page (all except the
 * ones that must be preserved) to the argument value.  This can
 * help detect and illustrate problems that are dependent on these
 * direct page values (which SANE should not care about).
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <memory.h>

int do_setDP = 0;
unsigned char dp_val;

void setDP(unsigned char val) {
        int dummy;
        char *saneDP;
        
        saneDP = (char*)*FindHandle((void*)&dummy);
        
        for (unsigned i = 0; i < 256; i++) {
                if (i < 24 || i > 29) {
                        saneDP[i] = val;
                }
        }
}

void convert(extended e) {
        if (do_setDP) {
                setDP(dp_val);
        }

        comp c = e;
        printf("%22.1f converts to %20.0f", e, c);
        
        if (c == e) {
                printf(" (correct)");
        } else if ((long long)e == e && e != LLONG_MIN) {
                printf(" (incorrect)");
        }
        printf("\n");
}

int main(int argc, char *argv[]) {
        char buf[80];
        extended val;
        char *endptr;

        static extended e[] = {
                -8388608.0,
                -65536000.0,
                -134217728.0,
                -536870912.0,
                -655360000.0,
                -1073741824.0,
                -2147483648.0,
                -3276800000.0,
                -53019223785472.0,
                -53021371269120.0,
                -53021371334656.0,
                -140737488289792.0,
                -140737488355328.0, //(-2^47)
                -140737488420864.0,
                -211106232532992.0, //(-2^47-2^46)
                -281470681743360.0,
                -281474976710656.0,
                -72057594037927936.0,
                -100000000000000000.0,
                -8000000000000000000.0,
                
                /* The below values have not shown problems in my testing */
                //-65536.0,
                //-524288.0,
                //-1048576.0,
                //-2097152.0,
                //-4194304.0,
                //-6557465476465.0,
                //-53019223818240.0,
                //-10000000000000000.0,
        };

        if (argc >= 2) {
                dp_val = strtoul(argv[1], NULL, 0);
                do_setDP = 1;
        }
        
        printf(
                "This program demonstrates a bug in SANE conversion of certain numbers from\n"
                "floating-point to integer formats. If it has not been fixed, at least some\n"
                "of the numbers below will probably be converted incorrectly:\n");
        
        for (int i = 0; i < sizeof(e)/(sizeof(*e)); i++) {
                convert(e[i]);
        }
        
        do {
                printf("Type another number to try converting, or RETURN to quit: ");
                fflush(stdout);

                if (fgets(buf, sizeof(buf), stdin) == NULL)
                        return 0;

                val = strtold(buf, &endptr);
                if (endptr == buf)
                        return 0;
                
                convert(val);
        } while (1);
}
