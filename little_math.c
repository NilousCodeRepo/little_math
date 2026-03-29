#include <stdio.h>

#define LITTLE_MATH_STRIP_PREFIX
#define LITTLE_MATH
#include "little_math.h"

int main(int argc, char** argv)
{
    for(int idx = 0; idx <= 720; ++idx)
        printf( "SIN OF %d: [%lf]\tCOS OF %d: [%lf]\tTAN OF %d: [%lf]\n", idx, sin((double)idx), idx, cos((double)idx), idx, tan((double)idx) );
    
    return 0;

}
