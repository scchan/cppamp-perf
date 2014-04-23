

#ifdef ENABLE_CPPAMP
  #define AMP_RESTRICT restrict(cpu,amp)
  #define EXPF(x)   fast_math::expf(x)
  #define FABSF(x)  fast_math::fabsf(x)
  #define LOGF(x)   fast_math::logf(x)
  #define SQRTF(x)  fast_math::sqrtf(x)
#else
  #define AMP_RESTRICT
  #define EXPF(x)   exp(x)
  #define FABSF(x)  fabs(x)
  #define LOGF(x)   log(x)
  #define SQRTF(x)  sqrt(x)
#endif


#define S_LOWER_LIMIT 10.0f
#define S_UPPER_LIMIT 100.0f
#define K_LOWER_LIMIT 10.0f
#define K_UPPER_LIMIT 100.0f
#define T_LOWER_LIMIT 1.0f
#define T_UPPER_LIMIT 10.0f
#define R_LOWER_LIMIT 0.01f
#define R_UPPER_LIMIT 0.05f
#define SIGMA_LOWER_LIMIT 0.01f
#define SIGMA_UPPER_LIMIT 0.10f

inline static void phi(float X, float* phi) AMP_RESTRICT
{
    float y;
    float absX;
    float t;
    float result;

    const float c1 = (float)0.319381530f;
    const float c2 = (float)-0.356563782f;
    const float c3 = (float)1.781477937f;
    const float c4 = (float)-1.821255978f;
    const float c5 = (float)1.330274429f;

    const float zero = (float)0.0f;
    const float one = (float)1.0f;
    const float two = (float)2.0f;
    const float temp4 = (float)0.2316419f;

    const float oneBySqrt2pi = (float)0.398942280f;

    absX = FABSF(X);
    t = one/(one + temp4 * absX);

    y = one - oneBySqrt2pi * EXPF(-X*X/two) * t 
        * (c1 + t
              * (c2 + t
                    * (c3 + t
                          * (c4 + t * c5))));

    result = (X < zero)? (one - y) : y;

    *phi = result;
}


inline static void calculateBlackScholes(const float in, float* call, float* put) AMP_RESTRICT {
    float d1, d2;
    float phiD1, phiD2;
    float sigmaSqrtT;
    float KexpMinusRT;
    
    float two = (float)2.0f;
    float inRand = in;
    float S = S_LOWER_LIMIT * inRand + S_UPPER_LIMIT * (1.0f - inRand);
    float K = K_LOWER_LIMIT * inRand + K_UPPER_LIMIT * (1.0f - inRand);
    float T = T_LOWER_LIMIT * inRand + T_UPPER_LIMIT * (1.0f - inRand);
    float R = R_LOWER_LIMIT * inRand + R_UPPER_LIMIT * (1.0f - inRand);
    float sigmaVal = SIGMA_LOWER_LIMIT * inRand + SIGMA_UPPER_LIMIT * (1.0f - inRand);

    sigmaSqrtT = sigmaVal * SQRTF(T);

    d1 = (LOGF(S/K) + (R + sigmaVal * sigmaVal / two)* T)/ sigmaSqrtT;
    d2 = d1 - sigmaSqrtT;

    KexpMinusRT = K * EXPF(-R * T);
    phi(d1, &phiD1);
    phi(d2, &phiD2);
    *call = S * phiD1 - KexpMinusRT * phiD2;
    phi(-d1, &phiD1);
    phi(-d2, &phiD2);
    *put = KexpMinusRT * phiD2 - S * phiD1;
}


#ifdef OCL_BLACKSCHOLES

__kernel
void
blackScholes(const __global float *in,
             __global float *call,
             __global float *put)
{
    float c;
    float p;
    float i = in[get_global_id(0)];
    calculateBlackScholes(i, &c, &p);
    call[get_global_id(0)] = c;
    put [get_global_id(0)] = p;
}

#endif



