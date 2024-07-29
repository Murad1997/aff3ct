#include <algorithm>
#include <string>

#include "Module/Decoder/LDPC/BP/Horizontal_layered/Decoder_LDPC_BP_horizontal_layered.hpp"
#include "Tools/Perf/common/hard_decide.h"
#include "Tools/general_utils.h"

namespace aff3ct
{
namespace module
{
template<typename B, typename R, class Update_rule>
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::Decoder_LDPC_BP_horizontal_layered(
  const int K,
  const int N,
  const int n_ite,
  const tools::Sparse_matrix& _H,
  const std::vector<unsigned>& info_bits_pos,
  const Update_rule& up_rule,
  const bool enable_syndrome,
  const int syndrome_depth)
  : Decoder_SISO<B, R>(K, N)
  , Decoder_LDPC_BP(K, N, n_ite, _H, enable_syndrome, syndrome_depth)
  , info_bits_pos(info_bits_pos)
  , up_rule(up_rule)
  , var_nodes(this->n_frames, std::vector<R>(N))
  , messages(this->n_frames, std::vector<R>(this->H.get_n_connections()))
  , contributions(this->H.get_cols_max_degree())
{
    const std::string name = "Decoder_LDPC_BP_horizontal_layered<" + this->up_rule.get_name() + ">";
    this->set_name(name);
    for (auto& t : this->tasks)
        t->set_replicability(true);

    tools::check_LUT(info_bits_pos, "info_bits_pos", (size_t)K);

    this->reset();
}

template<typename B, typename R, class Update_rule>
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>*
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::clone() const
{
    auto m = new Decoder_LDPC_BP_horizontal_layered(*this);
    m->deep_copy(*this);
    return m;
}

template<typename B, typename R, class Update_rule>
void
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::_reset(const size_t frame_id)
{
    std::fill(this->messages[frame_id].begin(), this->messages[frame_id].end(), (R)0);
    std::fill(this->var_nodes[frame_id].begin(), this->var_nodes[frame_id].end(), (R)0);
}

template<typename B, typename R, class Update_rule>
void
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::_load(const R* Y_N, const size_t frame_id)
{
    for (auto v = 0; v < (int)var_nodes[frame_id].size(); v++)
        this->var_nodes[frame_id][v] += Y_N[v]; // var_nodes contain previous extrinsic information
}

template<typename B, typename R, class Update_rule>
int
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::_decode_siso(const R* Y_N1,
                                                                    int8_t* CWD,
                                                                    R* Y_N2,
                                                                    const size_t frame_id)
{
    // memory zones initialization
    this->_load(Y_N1, frame_id);

    // actual decoding
    auto status = this->_decode(frame_id);

    // prepare for next round by processing extrinsic information
    for (auto v = 0; v < this->N; v++)
        Y_N2[v] = this->var_nodes[frame_id][v] - Y_N1[v];

    // copy extrinsic information into var_nodes for next TURBO iteration
    std::copy(Y_N2, Y_N2 + this->N, this->var_nodes[frame_id].begin());

    CWD[0] = !status;
    return status;
}

template<typename B, typename R, class Update_rule>
int
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::_decode_siho(const R* Y_N,
                                                                    int8_t* CWD,
                                                                    B* V_K,
                                                                    const size_t frame_id)
{
    //	auto t_load = std::chrono::steady_clock::now(); // -----------------------------------------------------------
    // LOAD
    this->_load(Y_N, frame_id);
    //	auto d_load = std::chrono::steady_clock::now() - t_load;

    //	auto t_decod = std::chrono::steady_clock::now(); // --------------------------------------------------------
    // DECODE
    // actual decoding
    auto status = this->_decode(frame_id);
    //	auto d_decod = std::chrono::steady_clock::now() - t_decod;

    //	auto t_store = std::chrono::steady_clock::now(); // ---------------------------------------------------------
    // STORE
    // take the hard decision
    for (auto i = 0; i < this->K; i++)
    {
        const auto k = this->info_bits_pos[i];
        V_K[i] = !(this->var_nodes[frame_id][k] >= 0);
    }
    //	auto d_store = std::chrono::steady_clock::now() - t_store;

    //	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::load,   d_load);
    //	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::decode, d_decod);
    //	(*this)[dec::tsk::decode_siho].update_timer(dec::tm::decode_siho::store,  d_store);

    CWD[0] = !status;
    return status;
}

template<typename B, typename R, class Update_rule>
int
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::_decode_siho_cw(const R* Y_N,
                                                                       int8_t* CWD,
                                                                       B* V_N,
                                                                       const size_t frame_id)
{
    //	auto t_load = std::chrono::steady_clock::now(); // -----------------------------------------------------------
    // LOAD
    this->_load(Y_N, frame_id);
    //	auto d_load = std::chrono::steady_clock::now() - t_load;

    //	auto t_decod = std::chrono::steady_clock::now(); // --------------------------------------------------------
    // DECODE
    // actual decoding
    auto status = this->_decode(frame_id);
    //	auto d_decod = std::chrono::steady_clock::now() - t_decod;

    //	auto t_store = std::chrono::steady_clock::now(); // ---------------------------------------------------------
    // STORE
    tools::hard_decide(this->var_nodes[frame_id].data(), V_N, this->N);
    //	auto d_store = std::chrono::steady_clock::now() - t_store;

    //	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::load,   d_load);
    //	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::decode, d_decod);
    //	(*this)[dec::tsk::decode_siho_cw].update_timer(dec::tm::decode_siho_cw::store,  d_store);

    CWD[0] = !status;
    return status;
}

template<typename B, typename R, class Update_rule>
int
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::_decode(const size_t frame_id)
{
    this->up_rule.begin_decoding(this->n_ite);

    bool valid_synd = true;
    for (auto ite = 0; ite < this->n_ite; ite++)
    {
        this->up_rule.begin_ite(ite);
        this->_decode_single_ite(this->var_nodes[frame_id], this->messages[frame_id]);
        this->up_rule.end_ite();

        valid_synd = this->check_syndrome_soft(this->var_nodes[frame_id].data());
        if (valid_synd) break;
    }

    this->up_rule.end_decoding();

    return !valid_synd && this->enable_syndrome;
}

template<typename B, typename R, class Update_rule>
void
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::_decode_single_ite(std::vector<R>& var_nodes,
                                                                          std::vector<R>& messages)
{
    auto kr = 0;
    auto kw = 0;

    // horizontal layered scheduling
    const auto n_chk_nodes = (int)this->H.get_n_cols();
    for (auto c = 0; c < n_chk_nodes; c++)
    {
        const auto chk_degree = (int)this->H[c].size();
        this->up_rule.begin_chk_node_in(c, chk_degree);
        for (auto v = 0; v < chk_degree; v++)
        {
            this->contributions[v] = var_nodes[this->H[c][v]] - messages[kr++];
            this->up_rule.compute_chk_node_in(v, this->contributions[v]);
        }
        this->up_rule.end_chk_node_in();

        this->up_rule.begin_chk_node_out(c, chk_degree);
        for (auto v = 0; v < chk_degree; v++)
        {
            messages[kw] = this->up_rule.compute_chk_node_out(v, this->contributions[v]);
            var_nodes[this->H[c][v]] = this->contributions[v] + messages[kw++];
        }
        this->up_rule.end_chk_node_out();
    }
}

template<typename B, typename R, class Update_rule>
void
Decoder_LDPC_BP_horizontal_layered<B, R, Update_rule>::set_n_frames(const size_t n_frames)
{
    const auto old_n_frames = this->get_n_frames();
    if (old_n_frames != n_frames)
    {
        Decoder_SISO<B, R>::set_n_frames(n_frames);

        const auto vec_size = this->var_nodes[0].size();
        const auto old_var_nodes_size = this->var_nodes.size();
        const auto new_var_nodes_size = (old_var_nodes_size / old_n_frames) * n_frames;
        this->var_nodes.resize(new_var_nodes_size, std::vector<R>(vec_size));

        const auto vec_size2 = this->messages[0].size();
        const auto old_messages_size = this->messages.size();
        const auto new_messages_size = (old_messages_size / old_n_frames) * n_frames;
        this->messages.resize(new_messages_size, std::vector<R>(vec_size2));
    }
}

}
}
