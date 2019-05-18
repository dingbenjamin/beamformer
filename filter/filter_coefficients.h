#ifndef FILTER_FILTER_COEFFICIENTS_H_
#define FILTER_FILTER_COEFFICIENTS_H_

/* Filter Structure

   in -> H(stg1) -> downsample(M1) -> H(stg2) -> downsample(M2) -> 
   -> H(stg3) -> downsample(M3) -> H(stg4) -> downsample(M4) -> out

*/

static const uint8_t decimation_factor_stg1 = 2;
static const float iir_stg1_num_1[] = {0.0138365, 0.0276730, 0.0138365};
static const float iir_stg1_den_1[] = {1.0000000, -1.6407759, 0.6961219};

static const uint8_t decimation_factor_stg2 = 3;
static const float iir_stg2_num_1[] = {0.0086774, 0.0173548, 0.0086774};
static const float iir_stg2_den_1[] = {1.0000000, -1.7197421, 0.7544517};

static const uint8_t decimation_factor_stg3 = 3;
static const float iir_stg3_gain = 0.0007189;
static const float iir_stg3_num_1[] = {1.0000000, 0.9990472, 0.0000000};
static const float iir_stg3_den_1[] = {1.0000000, -0.5604703, 0.0000000};
static const float iir_stg3_num_2[] = {1.0000000, 2.0015407, 1.0015416};
static const float iir_stg3_den_2[] = {1.0000000, -1.1995021, 0.4062280};
static const float iir_stg3_num_3[] = {1.0000000, 1.9994122, 0.9994131};
static const float iir_stg3_den_3[] = {1.0000000, -1.4690521, 0.7222330};

static const uint8_t decimation_factor_stg4 = 5;
static const float iir_stg4_gain_1 = 0.0002984;
static const float iir_stg4_gain_2 = 0.0010000;
static const float iir_stg4_num_1[] = {1.0000000, 1.0346697, 0.0000000};
static const float iir_stg4_den_1[] = {1.0000000, -0.6445602, 0.0000000};
static const float iir_stg4_num_2[] = {1.0000000, 2.0524836, 1.0536768};
static const float iir_stg4_den_2[] = {1.0000000, -1.3122503, 0.4408545};
static const float iir_stg4_num_3[] = {1.0000000, 2.0108794, 1.0120511};
static const float iir_stg4_den_3[] = {1.0000000, -1.3837383, 0.5193486};
static const float iir_stg4_num_4[] = {1.0000000, 1.9653564, 0.9665046};
static const float iir_stg4_den_4[] = {1.0000000, -1.5097483, 0.6577079};
static const float iir_stg4_num_5[] = {1.0000000, 1.9366110, 0.9377444};
static const float iir_stg4_den_5[] = {1.0000000, -1.6996075, 0.8661738};

#endif /*  FILTER_FILTER_COEFFICIENTS_H_ */
