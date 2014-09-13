

void accelerated_lib(float* output, float* inputA, float* inputB, const unsigned num);

float compute_restrict(float a, float b) restrict(amp);
