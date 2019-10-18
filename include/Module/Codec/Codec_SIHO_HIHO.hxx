#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <string>

#include "Module/Codec/Codec_SIHO_HIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Codec_SIHO_HIHO<B,Q>
::Codec_SIHO_HIHO(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Codec     <B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_SIHO<B,Q>(K, N_cw, N, tail_length, n_frames),
  Codec_HIHO<B,Q>(K, N_cw, N, tail_length, n_frames)
{
	const std::string name = "Codec_SIHO_HIHO";
	this->set_name(name);
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>
::reset()
{
	this->get_decoder_siho()->reset();
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>
::set_decoder_siho_hiho(std::shared_ptr<Decoder_SIHO_HIHO<B,Q>> dec)
{
	this->set_decoder_siho(dec);
	this->set_decoder_hiho(dec);
}

template <typename B, typename Q>
void Codec_SIHO_HIHO<B,Q>
::set_decoder_siho_hiho(Decoder_SIHO_HIHO<B,Q>* dec)
{
	this->set_decoder_siho_hiho(std::shared_ptr<Decoder_SIHO_HIHO<B,Q>>(dec));
}

}
}

#endif