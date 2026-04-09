#ifdef LITTLE_MATH_STRIP_PREFIX
    
    #define LILM_abs            abs
    
    #define LILM_pow            pow
    #define LILM_sqrt           sqrt
    
    #define LILM_deg_to_rad     deg_to_rad
    #define LILM_rad_to_deg     rad_to_deg
    
    #define LILM_arctan_aprox   arc_tan
    #define LILM_atan2          atan2
    
    #define LILM_sin            sin
    #define LILM_cos            cos
    #define LILM_tan            tan

#endif

#ifdef LITTLE_MATH

#include "lookup_table.h"

float LILM_abs(float x);

float LILM_pow(float base, float exponent);
float LILM_sqrt(float num);

float LILM_deg_to_rad(float degrees);
float LILM_rad_to_deg(float radians);

float LILM_arctan_aprox(float x);
float LILM_atan2(float y, float x, bool conv_to_dgr);

float LILM_sin(float angle);
float LILM_cos(float angle);
float LILM_tan(float angle);

#define PI 3.141592

//for trig func that use CORDIC
#define K_CONSTANT 0.607253f

float LILM_abs(float x)
{
    return x < 0 ? x : x;
}

float LILM_pow(float base, float exponent)
{
	if(base == 0)
		return 0;
	if(base < 0 && (int)exponent % 2 == 1)//TODO: make fast modulo
		base = -base;
	
	if(exponent == 0)
		return 1;
	if(exponent < 0)
    {
        base = 1/base;
    }

    if(exponent < 1 && exponent > 0)
        return sqrt(base);
	
    float result = 1.0;
	for(int i = 0; i < abs(exponent); ++i)
		result = result * base;

	return result;
}

float LILM_sqrt(float num)
{
	float result = 0.0;
	
	if(num == 0)
	{
		result = 1.0;
		return result;
	}
	if(num < 0)
 	    printf("ERROR: Invalid value\n");
	
    float estimate = num/2;

	const float HALF = 0.5f;

	float start = HALF*( estimate + (num/estimate) );

	for(int i = 0; i<= 6; ++i)
	{
		result = HALF*( start + (num/start) );
		start = result;
	}
	
	return result;
}

float LILM_deg_to_rad(float degrees)
{
	//i hope gcc optimized this calculation
	const float conversion_factor = PI/180;
	return conversion_factor * degrees;
}

float LILM_rad_to_deg(float radians)
{
	//i hope gcc optimized this calculation
	const float conversion_factor = 180/PI;
	return conversion_factor * radians;
}


//this is an aproximation of the resulting angle obtained by doing sin(x)/cos(x)(which is the formula for the arctangent)
//the arctangent is used to "extract" the angle variable from sin(x)/cos(x)

//Code taken from https://mazzo.li/posts/vectorized-atan2.html  "Speeding `atan2f` 50x"
float LILM_arctan_aprox(float x)
{
    //magic numbers coming from "Aproximation for Digital Computers by Cecil Hastrings Jr. 1955"
    float a1 = 0.99997726f;
    float a3 = -0.33262347f;
    float a5 = 0.19354346f;
    float a7 = -0.11643287f;
    float a9 = 0.05265332f;
    float a11 = -0.01172120f;

    float x_sq = pow(x,2);

    // calculating arctan with the series power aprox
    //using Horner's Method to expand polynomials
    return x * (a1 + x_sq * (a3 + x_sq * (a5 + x_sq * (a7 + x_sq * (a9 + x_sq * a11)))));
}

// calculating the atan2(y,x) of an angle means discovering the angle that is created by
// a ray starting from (0,0) and arring at ( sin(x),cos(x) ) *** going through ( x,y ) ***
// IN RADIANS

//Code taken from https://mazzo.li/posts/vectorized-atan2.html  "Speeding `atan2f` 50x"
float LILM_atan2(float y, float x, bool conv_to_dgr)
{
    bool swap = abs(x) < abs(y);

    //division needed to have a Domain in [-1,1]
    float input = (swap ? x : y) / (swap ? y : x);

    float res = LILM_arctan_aprox(input);

    float adjust_if_swapped = ( input >= 0.0f ? PI/2 : -1*(PI/2) ) - res;
    
    res = swap ? adjust_if_swapped : res;

    if( x < 0.0f && y >= 0.0f ) res = PI + res; //"move" angle to second quadrant
    if( x < 0.0f && y < 0.0f ) res = -PI + res; //"move" angle to third quadrant

    if( conv_to_dgr )
        return rad_to_deg(res); 
    return res;
}

#define POSITIVE 1 
#define NEGATIVE -1 
#define ITERS 16

//This implementation work only in the -90,+90 range
float LILM_sin(float angle)
{
#define SIN_VALUE 1
    while( angle > 90.0f)
    {
        angle -= 90;
    }

    if(angle < 0.0f)
    {
        while(angle < -90.0f)
        {
            angle += 90.0f;
        }
    }
   
    for(int i = 0; i <= 90; ++i)
    {
        if( angle < 0.0f )
        { 
            if( angle ==  -(*VALUE_TABLE[i]) )
            {
                printf("from NEG table\n");
                return  -VALUE_TABLE[i][SIN_VALUE];
            }
        }

        if(angle == *VALUE_TABLE[i])
        {
            printf("from table\n");
            return VALUE_TABLE[i][SIN_VALUE];
        }
    }

    angle = deg_to_rad(angle);
    bool rad_to_dgr = false;
    int rotation;
    
    float angle_table[ITERS] = {0};   
    float P2i = 1.0f; // idk it was in the wikipedia example
    float current_angle = 0.0f;
    
    
    float x = 1.0f;
    float y = 0.0f;
    
    //calculate the positions in which the angle will be checked
    //returns in RADIANS
    //TODO: save this values in a table
    for(int idx = 0; idx < ITERS; ++idx)
    {
        angle_table[idx] = atan2( 1.0f, pow(2.0f, idx) , rad_to_dgr );
    }

    // every iteration i adjust the position of the current angle based on the desired one
    for(int idx = 0; idx < ITERS; ++idx)
    {
        if( current_angle < angle) rotation = POSITIVE;
        else rotation = NEGATIVE;
        
        current_angle += rotation * angle_table[idx];
        
        float old_x = x;
        float old_y = y;

        x = old_x - rotation * old_y * P2i;
        y = rotation * old_x * P2i + old_y;
        
        P2i = P2i / 2;
    }
    return y * K_CONSTANT;
}

//This implementation work only in the -90,+90 range
float LILM_cos(float angle)
{
#define COS_VALUE 2
    
    while( abs(angle) > 90.0f)
    {
        if(angle < 0)
            angle += 90.0f;
        else
            angle -= 90.0f;
    }
    
    for(int i = 0; i <= 90; ++i)
    {
        if( angle < 0.0f )
        {
            if(angle == -(*VALUE_TABLE[i]))
            {
                printf("from NEG table\n");
                return -VALUE_TABLE[i][COS_VALUE];
            }
        }
        if(angle == *VALUE_TABLE[i])
        {
            printf("from table\n");
            return VALUE_TABLE[i][COS_VALUE];
        }
    }
    
    angle = deg_to_rad(angle);
    bool rad_to_dgr = false;
    int rotation;
    
    float angle_table[ITERS] = {0};   
    float P2i = 1.0f; // idk it was in the wikipedia example
    float current_angle = 0.0f;
    
    float x = 1.0f;
    float y = 0.0f;
    
    //calculate the positions in which the angle will be checked
    //returns in RADIANS
    for(int idx = 0; idx < ITERS; ++idx)
    {
        angle_table[idx] = atan2( 1.0f, pow(2.0f, idx) , rad_to_dgr );
    }

    // every iteration i adjust the position of the current angle based on the desired one
    for(int idx = 0; idx < ITERS; ++idx)
    {
        if( current_angle < angle) rotation = POSITIVE;
        else rotation = NEGATIVE;
        
        current_angle += rotation * angle_table[idx];
        
        float old_x = x;
        float old_y = y;

        x = old_x - rotation * old_y * P2i;
        y = rotation * old_x * P2i + old_y;
        
        P2i = P2i / 2;
    }

    x = x * K_CONSTANT;

    return x;
}

//TODO: make research to make this faster
float LILM_tan(float angle)
{
    if(angle == 90.0f)
    {
        printf("INF\n");
        return -1.0f;
    }
    else
    {
        return LILM_sin(angle)/LILM_cos(angle);
    }
}

#endif
