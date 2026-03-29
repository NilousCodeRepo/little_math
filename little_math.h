double LILM_abs(double x);

double LILM_pow(double base, double exponent);
double LILM_sqrt(double num);

double LILM_deg_to_rad(double degrees);
double LILM_rad_to_deg(double radians);

double LILM_arctan_aprox(double x);
double LILM_atan2(double y, double x, bool conv_to_dgr);

double LILM_sin(double angle);
double LILM_cos(double angle);
double LILM_tan(double angle);

#ifdef LITTLE_MATH_STRIP_PREFIX
    
    #define LILM_vec2           vec2
    
    #define LILM_abs            abs
    
    #define LILM_pow            pow
    #define LILM_sqrt           sqrt
    
    #define LILM_deg_to_rad     deg_to_rad
    #define LILM_rad_to_deg     rad_to_deg
    
    #define LILM_arctan_aprox   arc_tan
    #define LILM_atan2          atan2
    
    #define LILM_sin            sin
    #define LILM_cos            cos
    #define LILM_tan           tan

#endif

#ifdef LITTLE_MATH

#define PI 3.141592

//for trig func that use CORDIC
#define K_CONSTANT 0.607253f

typedef struct
{
    double x;
	double y;
}LILM_vec2;

double LILM_abs(double x)
{
    return x < 0 ? x : x;
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
double LILM_arctan_aprox(double x)
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

    double res = LILM_arctan_aprox(input);

    double adjust_if_swapped = ( input >= 0.0f ? PI/2 : -1*(PI/2) ) - res;
    
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

double LILM_sin(double angle)
{
    //negative angles are bullshit
    if(angle < 0) angle *= -1;

    //looses precision after 360, but the angle does not care about how many times you turn it in circles
    while(angle > 360)
    {
        angle -= 360;
    }

    angle = deg_to_rad(angle);
    bool rad_to_dgr = false;
    int rotation;
    
    double angle_table[ITERS] = {0};   
    double P2i = 1.0f; // idk it was in the wikipedia example
    double current_angle = 0.0f;
    
    
    vec2 coords = {
                    .x = 1.0f, 
                    .y = 0.0f
                  };
    
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
        
        double old_x = coords.x;
        double old_y = coords.y;

        coords.x = old_x - rotation * old_y * P2i;
        coords.y = rotation * old_x * P2i + old_y;
        
        P2i = P2i / 2;
    }

    coords.y = coords.y * K_CONSTANT;

    return coords.y;
}

double LILM_cos(double angle)
{
    //negative angles are bullshit
    if(angle < 0) angle *= -1;

    //looses precision after 360, but the angle does not care about how many times you turn it in circles
    while(angle > 360)
    {
        angle -= 360;
    }

    angle = deg_to_rad(angle);
    bool rad_to_dgr = false;
    int rotation;
    
    double angle_table[ITERS] = {0};   
    double P2i = 1.0f; // idk it was in the wikipedia example
    double current_angle = 0.0f;
    
    vec2 coords = {
                    .x = 1.0f, 
                    .y = 0.0f
                  };
    
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
        
        double old_x = coords.x;
        double old_y = coords.y;

        coords.x = old_x - rotation * old_y * P2i;
        coords.y = rotation * old_x * P2i + old_y;
        
        P2i = P2i / 2;
    }

    coords.x = coords.x * K_CONSTANT;

    //can't have negative cos, but the computer is dumb, and I more than him
    if(coords.x < 0) coords.x *= -1;

    return coords.x;
}

//TODO: make research to make this faster
double LILM_tan(double angle)
{
    return LILM_sin(angle)/LILM_cos(angle);
}

#endif
