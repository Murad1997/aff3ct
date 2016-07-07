#ifdef CHANNEL_MKL

#include <cmath>
#include <algorithm>
#include <cassert>

#include "Channel_AWGN_MKL_LLR.hpp"

#include "../../../Tools/bash_tools.h"

template <typename R>
Channel_AWGN_MKL_LLR<R>
::Channel_AWGN_MKL_LLR(const R& sigma, const int seed, const R& scaling_factor)
: sigma(sigma),
  scaling_factor(scaling_factor)
{
	//vslNewStream(&stream_state, VSL_BRNG_MT2203, seed);
	vslNewStream(&stream_state, VSL_BRNG_SFMT19937, seed);
}

template <typename R>
Channel_AWGN_MKL_LLR<R>
::~Channel_AWGN_MKL_LLR()
{
	vslDeleteStream(&stream_state);
}

template <typename R>
void Channel_AWGN_MKL_LLR<R>
::add_noise(const mipp::vector<R>& X_N, mipp::vector<R>& Y_N)
{
	assert(X_N.size()   == Y_N.size());
	assert(sigma        != 0         );
	assert(stream_state != nullptr   );

	std::cerr << bold_red("(EE) Adding white Gaussian noise is impossible on this type of data.") << std::endl;
	exit(-1);
}

template <>
void Channel_AWGN_MKL_LLR<float>
::add_noise(const mipp::vector<float>& X_N, mipp::vector<float>& Y_N)
{
	assert(X_N.size()   == Y_N.size());
	assert(sigma        != 0         );
	assert(stream_state != nullptr   );

	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              Y_N.size(),
	              Y_N.data(),
	              0.0,
	              sigma);
	/*
	vsRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              Y_N.size(),
	              Y_N.data(),
	              0.0,
	              sigma);
	*/

	auto size = Y_N.size();
	for (unsigned i = 0; i < size; i++)
		Y_N[i] = (X_N[i] + Y_N[i]) * scaling_factor;
}

template <>
void Channel_AWGN_MKL_LLR<double>
::add_noise(const mipp::vector<double>& X_N, mipp::vector<double>& Y_N)
{
	assert(X_N.size()   == Y_N.size());
	assert(sigma        != 0         );
	assert(stream_state != nullptr   );

	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2,
	              stream_state,
	              Y_N.size(),
	              Y_N.data(),
	              0.0,
	              sigma);
	/*
	vdRngGaussian(VSL_RNG_METHOD_GAUSSIAN_ICDF,
	              stream_state,
	              Y_N.size(),
	              Y_N.data(),
	              0.0,
	              sigma);
	*/

	auto size = Y_N.size();
	for (unsigned i = 0; i < size; i++)
		Y_N[i] = (X_N[i] + Y_N[i]) * scaling_factor;
}


// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Channel_AWGN_MKL_LLR<R_32>;
template class Channel_AWGN_MKL_LLR<R_64>;
#else
template class Channel_AWGN_MKL_LLR<R>;
#endif
// ==================================================================================== explicit template instantiation

#endif