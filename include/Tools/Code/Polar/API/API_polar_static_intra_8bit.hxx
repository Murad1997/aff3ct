#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <cstdint>
#include <memory>
#include <vector>
#include <mipp.h>

#include "Tools/Code/Polar/decoder_polar_functions.h"
#include "Tools/Code/Polar/API/functions_polar_inter_intra.h"
#include "Tools/Code/Polar/API/functions_polar_intra_8bit.h"
#include "Tools/Code/Polar/API/functions_polar_seq.h"
#include "Tools/Code/Polar/API/API_polar.hpp"
#include "Tools/Code/Polar/API/API_polar_static_intra_8bit.hpp"

namespace aff3ct
{
namespace tools
{

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
constexpr int API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::get_n_frames()
{
	return 1;
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <typename T>
bool API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::isAligned(const T *ptr)
{
	return mipp::isAligned(ptr);
}

// ------------------------------------------------------------------------------------------------------------------ f

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::f(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	f_intra_8bit<R, F, FI, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::f(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c, const int n_elmts)
{
	const R *__restrict l_a = l.data() + off_l_a;
	const R *__restrict l_b = l.data() + off_l_b;
	      R *__restrict l_c = l.data() + off_l_c;

	f_intra_8bit<R, F, FI, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ g

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::g(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	g_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::g(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
    const int off_l_c, const int n_elmts)
{
	const R *__restrict l_a = l.data() + off_l_a;
	const R *__restrict l_b = l.data() + off_l_b;
	const B *__restrict s_a = s.data() + off_s_a;
	      R *__restrict l_c = l.data() + off_l_c;

	g_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- g0

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::g0(const R *__restrict l_a, const R *__restrict l_b, R *__restrict l_c, const int n_elmts)
{
	g0_intra_8bit<R, G0, G0I, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::g0(std::vector<R,A> &l, const int off_l_a, const int off_l_b, const int off_l_c , const int n_elmts)
{
	const R *__restrict l_a = l.data() + off_l_a;
	const R *__restrict l_b = l.data() + off_l_b;
	      R *__restrict l_c = l.data() + off_l_c;

	g0_intra_8bit<R, G0, G0I, N_ELMTS>::apply(l_a, l_b, l_c, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- gr

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::gr(const R *__restrict l_a, const R *__restrict l_b, const B *__restrict s_a, R *__restrict l_c, const int n_elmts)
{
	gr_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::gr(const std::vector<B,AB> &s, std::vector<R,AR> &l, const int off_l_a, const int off_l_b, const int off_s_a,
     const int off_l_c, const int n_elmts)
{
	const R *__restrict l_a = l.data() + off_l_a;
	const R *__restrict l_b = l.data() + off_l_b;
	const B *__restrict s_a = s.data() + off_s_a;
	      R *__restrict l_c = l.data() + off_l_c;

	gr_intra_8bit<B, R, G, GI, N_ELMTS>::apply(l_a, l_b, s_a, l_c, n_elmts);
}

// ------------------------------------------------------------------------------------------------------------------ h

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::h(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	h_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::h(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R *__restrict l_a = l.data() + off_l_a;
	      B *__restrict s_a = s.data() + off_s_a;

	h_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- h0

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::h0(B *__restrict s_a, const int n_elmts)
{
	h0_inter_intra<B, N_ELMTS>::apply(s_a, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::h0(std::vector<B,A> &s, const int off_s_a, const int n_elmts)
{
	B *__restrict s_a = s.data() + off_s_a;

	h0_inter_intra<B, N_ELMTS>::apply(s_a, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- rep

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::rep(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	rep_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::rep(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R *__restrict l_a = l.data() + off_l_a;
	      B *__restrict s_a = s.data() + off_s_a;

	rep_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- spc

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
bool API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::spc(const R *__restrict l_a, B *__restrict s_a, const int n_elmts)
{
	return spc_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class AB, class AR>
bool API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::spc(std::vector<B,AB> &s, const std::vector<R,AR> &l, const int off_l_a, const int off_s_a, const int n_elmts)
{
	const R *__restrict l_a = l.data() + off_l_a;
	      B *__restrict s_a = s.data() + off_s_a;

	return spc_intra_8bit<B, R, H, HI, N_ELMTS>::apply(l_a, s_a, n_elmts);
}

// ----------------------------------------------------------------------------------------------------------------- xo

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::xo(const B *__restrict s_a, const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo_intra_8bit<B, XO, XOI, N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::xo(std::vector<B,A> &s, const int off_s_a, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B *__restrict s_a = s.data() + off_s_a;
	const B *__restrict s_b = s.data() + off_s_b;
	      B *__restrict s_c = s.data() + off_s_c;

	xo_intra_8bit<B, XO, XOI, N_ELMTS>::apply(s_a, s_b, s_c, n_elmts);
}

// ---------------------------------------------------------------------------------------------------------------- xo0

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::xo0(const B *__restrict s_b, B *__restrict s_c, const int n_elmts)
{
	xo0_intra_8bit<B, N_ELMTS>::apply(s_b, s_c, n_elmts);
}

template <typename B, typename R, proto_f  <R> F , proto_g  <B,R> G , proto_g0  <R> G0 , proto_h  <B,R> H , proto_xo  <B> XO,
                                  proto_f_i<R> FI, proto_g_i<B,R> GI, proto_g0_i<R> G0I, proto_h_i<B,R> HI, proto_xo_i<B> XOI>
template <int N_ELMTS, class A>
void API_polar_static_intra_8bit<B,R,F,G,G0,H,XO,FI,GI,G0I,HI,XOI>
::xo0(std::vector<B,A> &s, const int off_s_b, const int off_s_c, const int n_elmts)
{
	const B *__restrict s_b = s.data() + off_s_b;
	      B *__restrict s_c = s.data() + off_s_c;

	xo0_intra_8bit<B, N_ELMTS>::apply(s_b, s_c, n_elmts);
}

}
}

#endif
