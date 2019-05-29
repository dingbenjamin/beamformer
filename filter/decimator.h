#ifndef FILTER_DECIMATOR_H_
#define FILTER_DECIMATOR_H_

#include <filter/chain.h>
#include <filter/downsampler.h>
#include <filter/filter_coefficients.h>
#include <filter/gain.h>
#include <filter/iir3.h>
#include <filter/signal_processor.h>

class Decimator : public SignalProcessor {
    Iir3 filter_1{Iir3::MakeIir3(iir_stg1_den_1, iir_stg1_num_1)};
    DownSampler<decimation_factor_stg1> downsampler_1{};

    Iir3 filter_2{Iir3::MakeIir3(iir_stg2_den_1, iir_stg2_num_1)};
    DownSampler<decimation_factor_stg2> downsampler_2{};

    Iir3 filter_3_1{Iir3::MakeIir3(iir_stg3_den_1, iir_stg3_num_1)};
    Iir3 filter_3_2{Iir3::MakeIir3(iir_stg3_den_2, iir_stg3_num_2)};
    Iir3 filter_3_3{Iir3::MakeIir3(iir_stg3_den_3, iir_stg3_num_3)};
    Gain gain_3{iir_stg3_gain};
    DownSampler<decimation_factor_stg3> downsampler_3{};

    Iir3 filter_4_1{Iir3::MakeIir3(iir_stg4_den_1, iir_stg4_num_1)};
    Iir3 filter_4_2{Iir3::MakeIir3(iir_stg4_den_2, iir_stg4_num_2)};
    Iir3 filter_4_3{Iir3::MakeIir3(iir_stg4_den_3, iir_stg4_num_3)};
    Iir3 filter_4_4{Iir3::MakeIir3(iir_stg4_den_4, iir_stg4_num_4)};
    Iir3 filter_4_5{Iir3::MakeIir3(iir_stg4_den_5, iir_stg4_num_5)};
    Gain gain_4_1{iir_stg4_gain_1};
    Gain gain_4_2{iir_stg4_gain_1};
    DownSampler<decimation_factor_stg4> downsampler_4{};

    Chain chain{std::vector<SignalProcessor*>{
        &filter_1, &downsampler_1, &filter_2, &downsampler_2, &filter_3_1,
        &filter_3_2, &filter_3_3, &gain_3, &downsampler_3, &filter_4_1,
        &filter_4_2, &filter_4_3, &filter_4_4, &filter_4_5, &gain_4_1,
        &gain_4_2, &downsampler_4}};

   public:
    tl::optional<float> execute(tl::optional<float> sample);
};

#endif  // FILTER_DECIMATOR_H_
