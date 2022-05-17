/****************************************************************
 * $ID: smonde.h      Thu, Nov  6 18:52:08 2003 +0800  meihui $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  ∑∂√¿ª‘(MeiHui FAN)  <mhfan@ustc.edu>            *
 *              Laboratory of Structural Biology                *
 *              School of Life Science                          *
 *              Univ. of Sci.& Tech. of China (USTC)            *
 *              People's Republic of China (PRC)                *
 *                                                              *
 * CopyLeft (c)  2003~2013  M.H.Fan                             *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it              *
 *   under the terms of the GNU General Public Licence (GPL).   *
 *                                                              *
 * No warranty, no liability; use this at your own risk!        *
 ****************************************************************/
//#ifndef wtx_simple_model_of_nondeterministic_evaluations_smonde_h_
//#define wtx_simple_model_of_nondeterministic_evaluations_smonde_h_
#ifndef SMONDE_H
#define SMONDE_H

namespace Smonde {

template <typename Universe>
struct Multiverse { virtual void result(Universe&) = 0; };
//struct Rule { static void apply(Universe& u, Multiverse& m); };
// Universe: problem space, Multiverse: answer space

struct Zero {	// Zero(u) = {}
    template <typename Universe>
    static void apply (Universe&, Multiverse<Universe>&) {}
};

struct One {	// One(u) = {u}
    template <typename Universe>
    static void apply (Universe& u, Multiverse<Universe>& m) { m.result(u); }
};

template <typename Universe, typename Rule>
struct Applying_multiverse: Multiverse<Universe> {
    Applying_multiverse(Multiverse<Universe>& m): m_(m) {}

    void result(Universe& u) { Rule::apply(u, m_); }

private:
    Multiverse<Universe>& m_;
};

template <typename A=One, typename B=One, typename C=One, typename D=One,
	  typename E=One, typename F=One, typename G=One, typename H=One,
	  typename I=One, typename J=One, typename K=One, typename L=One,
	  typename M=One, typename N=One, typename O=One, typename P=One,
	  typename Q=One, typename R=One, typename S=One, typename T=One,
	  typename U=One, typename V=One, typename W=One, typename X=One,
	  typename Y=One, typename Z=One>
struct Product {
    template <typename Universe>
    static void apply (Universe& u, Multiverse<Universe>& m) {
	Applying_multiverse<Universe,
			    Product<B, C, D, E, F, G, H, I, J, K, L, M, N, O,
				    P, Q, R, S, T, U, V, W, X, Y, Z> > tm(m);
	A::apply(u, tm);
    }
};

template <>
struct Product<One, One, One, One, One, One, One, One, One,
	       One, One, One, One, One, One, One, One, One,
	       One, One, One, One, One, One, One, One>: One {};
// public inherent struct One

template <typename A=Zero, typename B=Zero, typename C=Zero, typename D=Zero,
	  typename E=Zero, typename F=Zero, typename G=Zero, typename H=Zero,
	  typename I=Zero, typename J=Zero, typename K=Zero, typename L=Zero,
	  typename M=Zero, typename N=Zero, typename O=Zero, typename P=Zero,
	  typename Q=Zero, typename R=Zero, typename S=Zero, typename T=Zero,
	  typename U=Zero, typename V=Zero, typename W=Zero, typename X=Zero,
	  typename Y=Zero, typename Z=Zero>
struct Sum {
    template <typename Universe>
    static void apply (Universe& u, Multiverse<Universe>& m) {
	Universe v(u);
	A::apply(u, m);

	Sum<B, C, D, E, F, G, H, I, J, K, L, M,
	    N, O, P, Q, R, S, T, U, V, W, X, Y, Z>::apply(v, m);
    }
};

template <>
struct Sum<Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero,
	   Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero,
	   Zero, Zero, Zero, Zero, Zero, Zero, Zero, Zero>: Zero {};

template <typename A,	  typename B=One, typename C=One, typename D=One,
	  typename E=One, typename F=One, typename G=One, typename H=One,
	  typename I=One, typename J=One, typename K=One, typename L=One,
	  typename M=One, typename N=One, typename O=One, typename P=One,
	  typename Q=One, typename R=One, typename S=One, typename T=One,
	  typename U=One, typename V=One, typename W=One, typename X=One,
	  typename Y=One, typename Z=One>   // Star<R> = One | R Star<R>
struct Star: Sum<One,
		 Product<Product<A, B, C, D, E, F, G, H, I, J, K, L, M,
				 N, O, P, Q, R, S, T, U, V, W, X, Y, Z>,
			 Star<A, B, C, D, E, F, G, H, I, J, K, L, M,
			      N, O, P, Q, R, S, T, U, V, W, X, Y, Z> > > {};

template <typename A,	  typename B=One, typename C=One, typename D=One,
	  typename E=One, typename F=One, typename G=One, typename H=One,
	  typename I=One, typename J=One, typename K=One, typename L=One,
	  typename M=One, typename N=One, typename O=One, typename P=One,
	  typename Q=One, typename R=One, typename S=One, typename T=One,
	  typename U=One, typename V=One, typename W=One, typename X=One,
	  typename Y=One, typename Z=One>   // Cross<R> = ???
struct Cross: Sum<Product<A, B, C, D, E, F, G, H, I, J, K, L, M,
			  N, O, P, Q, R, S, T, U, V, W, X, Y, Z>,
		  Product<Product< A, B, C, D, E, F, G, H, I, J, K, L, M,
				   N, O, P, Q, R, S, T, U, V, W, X, Y, Z>,
			  Cross<A, B, C, D, E, F, G, H, I, J, K, L, M,
				N, O, P, Q, R, S, T, U, V, W, X, Y, Z> > > {};

}   // namespace Smonde

#endif//SMONDE_H
// vim:sts=4:ts=8:
