//TODO: #define double as RATIONAL and UNSIGNED INT as NATURAL 

double LILM_abs(double x);

double LILM_pow(double base, double exponent);
double LILM_sqrt(double num);

double LILM_deg_to_rad(double degrees);

#ifdef LITTLE_MATH_STRIP_PREFIX
    
    #define LILM_vec2           vec2
    
    #define LILM_abs            abs
    
    #define LILM_pow            pow
    #define LILM_sqrt           sqrt
    
    #define LILM_deg_to_rad     deg_to_rad
    #define LILM_rad_to_deg     rad_to_deg

    #define LILM_atan2          atan2

#endif

#ifdef LITTLE_MATH

#define PI 3.141592

//for trig func that use CORDIC
#define K_CONSTANT 1.646760

typedef struct
{
    double x;
	double y;
}LILM_vec2;

double LILM_abs(double x)
{
    return x < 0 ? -x : x;
}

double LILM_pow(double base, double exponent)
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

	double result = 1.0;
	for(int i = 0; i < abs(exponent); ++i)
		result = result * base;

	return result;
}


//implementa Heron e successivamente Newton
//sono i compromessi migliori
double LILM_sqrt(double num)
{
	double result = 0.0;
	
	if(num == 0)
	{
		result = 1.0;
		return result;
	}
	if(num < 0)
 	    printf("ERROR: Invalid value\n");
	
    double estimate = num/2;

	const float HALF = 0.5f;

	double start = HALF*( estimate + (num/estimate) );

	for(int i = 0; i<= 6; ++i)
	{
		result = HALF*( start + (num/start) );
		start = result;
	}
	
	return result;
}

double LILM_deg_to_rad(double degrees)
{
	//i hope gcc optimized this calculation
	const double conversion_factor = PI/180;
	return conversion_factor * degrees;
}

double LILM_rad_to_deg(double radians)
{
	//i hope gcc optimized this calculation
	const double conversion_factor = 180/PI;
	return conversion_factor * radians;
}

//this is an aproximation of the resulting angle obtained by doing sin(x)/cos(x)(which is the formula for the arctangent)
//the arctangent is used to "extract" the angle variable from sin(x)/cos(x)

//Code taken from https://mazzo.li/posts/vectorized-atan2.html  "Speeding `atan2f` 50x"
double arctan_scalar_aprox(double x)
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
double LILM_atan2(double y, double x, bool conv_to_dgr)
{
    bool swap = abs(x) < abs(y);

    //division needed to have a Domain in [-1,1]
    double input = (swap ? x : y) / (swap ? y : x);

    double res = arctan_scalar_aprox(input);

    double adjust_if_swapped = ( input >= 0.0f ? PI/2 : -1*(PI/2) ) - res;
    
    res = swap ? adjust_if_swapped : res;

    if( x < 0.0f && y >= 0.0f ) res = PI + res; //"move" angle to second quadrant
    if( x < 0.0f && y < 0.0f ) res = -PI + res; //"move" angle to third quadrant

    if( conv_to_dgr )
        return rad_to_deg(res); 
    return res;
}

#endif
