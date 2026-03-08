//TODO: #define double as RATIONAL and UNSIGNED INT as NATURAL 

double LILM_pow(double* base, double* exponent);
double LILM_sqrt(double* num);

#ifdef LITTLE_MATH_IMPLEMENTATION
    #define LILM_vec2           vec2
    
    #define LILM_abs            abs
    
    #define LILM_pow            pow
    #define LILM_sqrt           sqrt
    
    #define LILM_deg_to_rad     deg_to_rad
    #define LILM_arctan         arctan

#endif

#ifdef LITTLE_MATH

#define LILM_PI 3.141592

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
		//1 = 1*3 = 3 ->
		//3 = 3 * 3 = 9 -> 
		//9 = 9 * 3 = 27

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
	const double conversion_factor = LILM_PI/180;
	return conversion_factor * degrees;
}

#endif
