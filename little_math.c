//TODO: make lookup table for commonly used values
// and make #defines to common values, so that the computations are done only on "non-common" angles

//TODO: Use a profiler and a static analizer and a linter

//TODO: Use 2double e 4double precision(Dekker method) for high precision ops
//https://www.youtube.com/watch?v=6OuqnaHHUG8       Double it like Dekker: A Remarkable Tecnique to Double Floating Point Precision (Part 1)
#include <stdio.h>

#define LITTLE_MATH
#define LITTLE_MATH_STRIP_PREFIX
#include "little_math.h"

#define ITERS 16 

double LILM_sin()
{
    vec2 v = {.x = 1.0f,
              .y = 0.0f
             };
    double theta_table[ITERS] = {0};
   
   //TODO: calculate the theta table with the gamma
    for(int i = 0; i <= sizeof(theta_table); ++i)
    {
        //theta_table[i] = arctan(i) ;
        printf("THETA OF %d: %lf\n",i,theta_table[i]);
    }
}

int main(int argc, char** argvi)
{

    return 0;

}
