#include "Tools/Factory/Factory_source.hpp"
#include "Tools/Factory/Factory_CRC.hpp"
#include "Tools/Factory/Factory_encoder_common.hpp"
#include "Tools/Factory/Factory_modulator.hpp"
#include "Tools/Factory/Factory_channel.hpp"
#include "Tools/Factory/Factory_quantizer.hpp"
#include "Tools/Factory/Coset/Factory_coset_real.hpp"
#include "Tools/Factory/Coset/Factory_coset_bit.hpp"

#include "Module/Puncturer/NO/Puncturer_NO.hpp"

#include "Simulation_BFER_std.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_std<B,R,Q>
::Simulation_BFER_std(const parameters& params, tools::Codec<B,Q> &codec)
: Simulation_BFER<B,R,Q>(params, codec),

  source     (params.simulation.n_threads, nullptr),
  crc        (params.simulation.n_threads, nullptr),
  encoder    (params.simulation.n_threads, nullptr),
  puncturer  (params.simulation.n_threads, nullptr),
  modulator  (params.simulation.n_threads, nullptr),
  channel    (params.simulation.n_threads, nullptr),
  quantizer  (params.simulation.n_threads, nullptr),
  coset_real (params.simulation.n_threads, nullptr),
  decoder    (params.simulation.n_threads, nullptr),
  coset_bit  (params.simulation.n_threads, nullptr),
  interleaver(params.simulation.n_threads, nullptr),

  rd_engine_seed(params.simulation.n_threads)
{
	for (auto tid = 0; tid < params.simulation.n_threads; tid++)
		rd_engine_seed[tid].seed(params.simulation.seed + tid);
}

template <typename B, typename R, typename Q>
Simulation_BFER_std<B,R,Q>
::~Simulation_BFER_std()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std<B,R,Q>
::build_communication_chain(const int tid)
{
	// build the objects
	source     [tid] = build_source     (tid); Simulation::check_errors(source     [tid], "Source<B>"     );
	crc        [tid] = build_crc        (tid); Simulation::check_errors(crc        [tid], "CRC<B>"        );
	interleaver[tid] = build_interleaver(tid);
	encoder    [tid] = build_encoder    (tid); Simulation::check_errors(encoder    [tid], "Encoder<B>"    );
	puncturer  [tid] = build_puncturer  (tid); Simulation::check_errors(puncturer  [tid], "Puncturer<B,Q>");
	modulator  [tid] = build_modulator  (tid); Simulation::check_errors(modulator  [tid], "Modulator<B,R>");
	channel    [tid] = build_channel    (tid); Simulation::check_errors(channel    [tid], "Channel<R>"    );
	quantizer  [tid] = build_quantizer  (tid); Simulation::check_errors(quantizer  [tid], "Quantizer<R,Q>");
	coset_real [tid] = build_coset_real (tid); Simulation::check_errors(coset_real [tid], "Coset<B,Q>"    );
	decoder    [tid] = build_decoder    (tid); Simulation::check_errors(decoder    [tid], "Decoder<B,Q>"  );
	coset_bit  [tid] = build_coset_bit  (tid); Simulation::check_errors(coset_bit  [tid], "Coset<B,B>"    );

	Simulation_BFER<B,R,Q>::build_communication_chain(tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER_std<B,R,Q>
::release_objects()
{
	const auto nthr = this->params.simulation.n_threads;
	for (auto i = 0; i < nthr; i++) if (source     [i] != nullptr) { delete source     [i]; source     [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (crc        [i] != nullptr) { delete crc        [i]; crc        [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (encoder    [i] != nullptr) { delete encoder    [i]; encoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (puncturer  [i] != nullptr) { delete puncturer  [i]; puncturer  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (modulator  [i] != nullptr) { delete modulator  [i]; modulator  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (channel    [i] != nullptr) { delete channel    [i]; channel    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (quantizer  [i] != nullptr) { delete quantizer  [i]; quantizer  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_real [i] != nullptr) { delete coset_real [i]; coset_real [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (decoder    [i] != nullptr) { delete decoder    [i]; decoder    [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (coset_bit  [i] != nullptr) { delete coset_bit  [i]; coset_bit  [i] = nullptr; }
	for (auto i = 0; i < nthr; i++) if (interleaver[i] != nullptr) { delete interleaver[i]; interleaver[i] = nullptr; }

	Simulation_BFER<B,R,Q>::release_objects();
}

template <typename B, typename R, typename Q>
Source<B>* Simulation_BFER_std<B,R,Q>
::build_source(const int tid)
{
	return Factory_source<B>::build(this->params, rd_engine_seed[tid]());
}

template <typename B, typename R, typename Q>
CRC<B>* Simulation_BFER_std<B,R,Q>
::build_crc(const int tid)
{
	return Factory_CRC<B>::build(this->params);
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_std<B,R,Q>
::build_encoder(const int tid)
{
	auto enc = Factory_encoder_common<B>::build(this->params, rd_engine_seed[tid]());
	if (enc == nullptr)
		enc = this->codec.build_encoder(tid, interleaver[tid]);

	return enc;
}

template <typename B, typename R, typename Q>
Puncturer<B,Q>* Simulation_BFER_std<B,R,Q>
::build_puncturer(const int tid)
{
	auto pct = this->codec.build_puncturer(tid);
	if (pct == nullptr)
		pct = new Puncturer_NO<B,Q>(this->params.code.K, this->params.code.N,
		                            this->params.simulation.inter_frame_level);

	return pct;
}

template <typename B, typename R, typename Q>
Interleaver<int>* Simulation_BFER_std<B,R,Q>
::build_interleaver(const int tid)
{
	return this->codec.build_interleaver(tid);
}

template <typename B, typename R, typename Q>
Modulator<B,R,R>* Simulation_BFER_std<B,R,Q>
::build_modulator(const int tid)
{
	return Factory_modulator<B,R,R>::build(this->params, this->sigma);
}

template <typename B, typename R, typename Q>
Channel<R>* Simulation_BFER_std<B,R,Q>
::build_channel(const int tid)
{
	return Factory_channel<R>::build(this->params, this->sigma, this->params.code.N_mod, rd_engine_seed[tid]());
}

template <typename B, typename R, typename Q>
Quantizer<R,Q>* Simulation_BFER_std<B,R,Q>
::build_quantizer(const int tid)
{
	return Factory_quantizer<R,Q>::build(this->params, this->sigma, this->params.code.N);
}

template <typename B, typename R, typename Q>
Coset<B,Q>* Simulation_BFER_std<B,R,Q>
::build_coset_real(const int tid)
{
	return Factory_coset_real<B,Q>::build(this->params);
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_std<B,R,Q>
::build_decoder(const int tid)
{
	return this->codec.build_decoder(tid, interleaver[tid], crc[tid]);
}

template <typename B, typename R, typename Q>
Coset<B,B>* Simulation_BFER_std<B,R,Q>
::build_coset_bit(const int tid)
{
	return Factory_coset_bit<B>::build(this->params);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_std<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_std<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_std<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
