/*!
 * \file
 * \brief Class factory::Encoder_polar.
 */
#ifndef FACTORY_ENCODER_POLAR_PAC_HPP
#define FACTORY_ENCODER_POLAR_PAC_HPP

#include <vector>
#include <string>
#include <map>
#include <cli.hpp>

#include "Tools/Factory/Header.hpp"
#include "Module/Encoder/Polar_PAC/Encoder_polar_PAC.hpp"
#include "Factory/Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Encoder_polar_name;
extern const std::string Encoder_polar_prefix;
class Encoder_polar_PAC : public Encoder
{
public:
	// ----------------------------------------------------------------------------------------------------- PARAMETERS
	// empty

	// -------------------------------------------------------------------------------------------------------- METHODS
	explicit Encoder_polar_PAC(const std::string &p = Encoder_polar_prefix);
	virtual ~Encoder_polar_PAC() = default;
	Encoder_polar_PAC* clone() const;

	// parameters construction
	void get_description(cli::Argument_map_info &args) const;
	void store          (const cli::Argument_map_value &vals);
	void get_headers    (std::map<std::string,tools::header_list>& headers, const bool full = true) const;

	// builder
	template <typename B = int>
	module::Encoder_polar_PAC<B>* build(const std::vector<bool> &frozen_bits) const;
};
}
}

#endif /* FACTORY_ENCODER_POLAR_HPP */
