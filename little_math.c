//TODO: look up nob to see how to remove the LILM_ with defines
//TODO: Use a profiler and a static analizer and a linter

//TODO: Use 2double e 4double precision(Dekker method) for high precision ops
//https://www.youtube.com/watch?v=6OuqnaHHUG8       Double it like Dekker: A Remarkable Tecnique to Double Floating Point Precision (Part 1)
#include <stdio.h>

#define LITTLE_MATH
#define LITTLE_MATH_IMPLEMENTATION
#include "little_math.h"

//TODO: make lookup table for commonly used values
// and make #defines to common values, so that the computations are done only on "non-common" angles

#define ITERS 16 

//Taylor Aproximation, domain = [-1,1]
//TODO: if it does not work, use atan2 intel's x86_64 instruction in asm
double LILM_arctan(double y, double x)
{

}

double LILM_sin()//devi calcolarti l'arcotangete prova con taylor
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

int main(int argc, char** argv)
{
    printf("TAYLOR OF ARCTAN %lf\n", LILM_arctan(0.5f));
    return 0;

}
