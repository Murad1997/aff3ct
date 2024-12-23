#include "Tools/Exception/exception.hpp"
#include "Tools/Documentation/documentation.h"
#include "Module/Encoder/Polar/Encoder_polar.hpp"
#include "Module/Encoder/Polar/Encoder_polar_sys.hpp"
#include "Factory/Module/Encoder/Polar_PAC/Encoder_polar_PAC.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

const std::string aff3ct::factory::Encoder_polar_name   = "Encoder Polar";
const std::string aff3ct::factory::Encoder_polar_prefix = "enc";

Encoder_polar_PAC
::Encoder_polar_PAC(const std::string &prefix)
: Encoder(Encoder_polar_name, prefix)
{
	this->type = "POLAR_PAC";
}

Encoder_polar_PAC* Encoder_polar_PAC
::clone() const
{
	return new Encoder_polar_PAC(*this);
}

void Encoder_polar_PAC
::get_description(cli::Argument_map_info &args) const
{
	Encoder::get_description(args);

	auto p = this->get_prefix();
	const std::string class_name = "factory::Encoder_polar::";

	cli::add_options(args.at({p+"-type"}), 0, "POLAR");

	tools::add_arg(args, p, class_name+"p+no-sys",
		cli::None());
}

void Encoder_polar_PAC
::store(const cli::Argument_map_value &vals)
{
	Encoder::store(vals);
}

void Encoder_polar_PAC
::get_headers(std::map<std::string,tools::header_list>& headers, const bool full) const
{
	Encoder::get_headers(headers, full);
}

template <typename B>
module::Encoder_polar_PAC<B>* Encoder_polar_PAC
::build(const std::vector<bool> &frozen_bits) const
{
	if (this->type == "POLAR" && !this->systematic) return new module::Encoder_polar    <B>(this->K, this->N_cw, frozen_bits);
	if (this->type == "POLAR" &&  this->systematic) return new module::Encoder_polar_sys<B>(this->K, this->N_cw, frozen_bits);

	throw tools::cannot_allocate(__FILE__, __LINE__, __func__);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template aff3ct::module::Encoder_polar_PAC<B_8 >* aff3ct::factory::Encoder_polar_PAC::build<B_8 >(const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_PAC<B_16>* aff3ct::factory::Encoder_polar_PAC::build<B_16>(const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_PAC<B_32>* aff3ct::factory::Encoder_polar_PAC::build<B_32>(const std::vector<bool>&) const;
template aff3ct::module::Encoder_polar_PAC<B_64>* aff3ct::factory::Encoder_polar_PAC::build<B_64>(const std::vector<bool>&) const;
#else
template aff3ct::module::Encoder_polar_PAC<B>* aff3ct::factory::Encoder_polar_PAC::build<B>(const std::vector<bool>&) const;
#endif
// ==================================================================================== explicit template instantiation
