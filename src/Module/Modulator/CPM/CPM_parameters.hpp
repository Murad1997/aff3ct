#ifndef CPM_PARAMETERS_HPP_
#define CPM_PARAMETERS_HPP_

#include <fstream>
#include <string>
#include "Tools/Perf/MIPP/mipp.h"

template <typename SIN, typename SOUT>
class CPM_parameters
{
public :
	CPM_parameters(const std::string& cpm_standard,
	               const int L,
	               const int k,
	               const int p,
	               const int n_b_per_s,
	               const int s_factor,
	               const std::string& filters_type,
	               const std::string& wave_shape);
	virtual ~CPM_parameters(){}

	const std::string cpm_standard; // the selection of a default CPM standard hardly implemented {GSM}

	int L;                          // CPM pulse width or CPm memory
	int k;                          // modulation index numerator
	int p;                          // modulation index denumerator
	int n_b_per_s;                  // number of bits per symbol
	int s_factor;                   // sampling factor

	const std::string filters_type; // Choose receiver filters bank: {TOTAL}
	const std::string wave_shape;   // Choose wave shape : {GMSK, REC, RCOS}

	int m_order;                    // modulation order of the cpm
	int n_bits_p;                   // number of bits used to code p
	int tl;                         // tail length for the modulation

	int n_wa;                       // number of wave forms used by the CPM
	int n_bits_wa;                  // number of bits used to code n_wa
	int max_wa_id;                  // maximum possible index of a trellis state

	int n_st;                       // number of states in the trellis
	int n_bits_st;                  // number of bits used to code n_st
	int max_st_id;                  // maximum possible index of a trellis state

	mipp::vector<SIN>  transition_to_binary;          // gives the binary code for a transition.
	                                                  // For each transition there are n_b_per_s bits separately given
	                                                  // (separate memory cases), in msb to lsb order

	mipp::vector<SIN>  binary_to_transition;          // gives the transition related to the binary code

	mipp::vector<int>  allowed_states;                // possible used state indexes
	                                                  // (there are jumps in the indexes because of p)

	mipp::vector<SOUT> allowed_wave_forms;            // possible used wave forms indexes
	                                                  // (there are jumps in the indexes because of p)

	mipp::vector<int>  trellis_next_state;            // from a given state and transition, gives the next state

	mipp::vector<SOUT> trellis_related_wave_form;     // from a given state and transition, gives the related wave form

	mipp::vector<int>  anti_trellis_original_state;   // from a given state and transition index,
	                                                  // gives the original state from where comes the transition

	mipp::vector<SIN>  anti_trellis_input_transition; // from a given state and transition index,
	                                                  // gives the related transition value
};

#include "CPM_parameters.hxx"

#endif /* CPM_PARAMETERS_HPP_ */