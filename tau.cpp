// LICENSE
// This software is free for use and redistribution while including this
// license notice, unless:
// 1. is used for commercial or non-personal purposes, or
// 2. used for a product which includes or associated with a blockchain or other
// decentralized database technology, or
// 3. used for a product which includes or associated with the issuance or use
// of cryptographic or electronic currencies/coins/tokens.
// On all of the mentioned cases, an explicit and written permission is required
// from the Author (Ohad Asor).
// Contact ohad@idni.org for requesting a permission. This license may be
// modified over time by the Author.
#include "fof.h"
#include "seq.h"
#include <iostream>

term<Bool> var_term(int v) {
	return term<Bool>(v);
//	stringstream ss;
//	ss << "x[" << v << "]";
//	return term<Bool>(ss.str());
}

bf<Bool> var(int v) { return bf<Bool>(minterm<Bool>(true, v)); }

term<Bool> fapp(const string& fname, const vector<sym_t>& args) {
	vector<term<Bool>::arg> v;
	for (const sym_t& s : args) v.emplace_back(term<Bool>(s));
	return term<Bool>(fname, v);
}

term<Bool> fapp(size_t i, size_t j, size_t v) {
	stringstream ss;
	ss << "f[" << i << ',' << j << ']';
	vector<term<Bool>::arg> a;
	for (size_t n = 0; n != v; ++n) a.push_back(var_term(n));
	return term<Bool>(ss.str(), a);
}

template<typename B> bf<B> generic(size_t nv, size_t c = 0) {
	bf<B> r;
	for (size_t v = 0; v != (((size_t)1) << nv); ++v) {
		minterm<B> m(true, term<B>(-v-1-c));
		for (size_t k = 0; k != nv; ++k)
			m[(v & (1 << k)) ? 0 : 1].insert(term<B>(k));
		r.insert(m);
	}
	return r;
}

template<typename B> fof<B> generic(size_t nc, size_t csz, size_t nv) {
	fof<B> f(false);
	for (size_t k = 0; k != nc; ++k) {
		clause<B> c;
		for (size_t n = 0; n != csz; ++n)
			//c = clause<B>(!n, term<bf<B>>(bf<B>(fapp(k,n,nv)))) & c;
			c = clause<B>(!n,
			term<bf<B>>(generic<B>(nv, (1<<nv) * (nc * n + k)))) & c;
		//cout << "c: " << c << endl;
		f = c | f;
		//cout << "f: " << f << endl;
	}
	return f;
}

int main() {
//	cout << generic<Bool>(2, 2, 2) << endl;
	//cout << generic<Bool>(2) << endl;
	//cout << generic<Bool>(3) << endl;
//	return 0;
//	bf<Bool> f(fapp("f", {0,1}));
//	bf<Bool> g(fapp("g", {1,2}));
//	cout << (~(f & g)) << endl;
//	cout << ex(ex(~(~fof<Bool>(f) | fof<Bool>(g)), 0),1) << endl;
//	cout << (generic(2, 3, 2) & generic(1,1,1)) << endl;
//	cout << fapp(0, 0, 1) << endl;
//	bf<B> f = subst(fapp(0, 0, 1), string("x[0]"), bf<B>(term<Bool>(string("y"))));
//	f = subst(fapp(0, 0, 1), string("x[0]"), bf<B>(fapp(1,1,2)));
//	cout << f << endl;
//	cout << var(1) << endl;
//	cout << (~var(1)) << endl;
//	cout << (var(1) | (~var(1))) << endl;
//	cout << ((var(1) & (~var(2))) | (var(1) & var(2))) << endl;
//	cout << generic<Bool>(2,2,1) << endl; return 0;
//	auto f = [](sym_t v)->sym_t { return 1+v; };
//	cout << transform_vars(generic<Bool>(2,2,3), f) << endl;
//	cout << generic<Bool>(1,1,1) << endl;
//	cout << generic<Bool>(1,1,1).subst(0,term<Bool>(1)) << endl;
//	cout << all(generic<Bool>(2),0) << endl;
	seq<Bool>((generic<Bool>(1,2,2)));
	return 0;
/*	cout << generic(2, 2, 2) << endl;
	return 0;
	//auto f = ~(var(-1) & var(-2) & var(-3) & var(-4));
	auto f = ~(var(1) & var(2));
//	cout << f << endl;
	auto g = var(3) & var(4);
	//auto g = ~(var(-1) & var(-2)) | (var(-3) & var(-4));
//	cout << g << endl;
	clause c1(true, f);
	//clause c2(false, g);
	clause c2(true, g);
	fof f1(c1);
	fof f2(c2);
	cout << c1 << endl;
	cout << c2 << endl;
	cout << (f1 & f2) << endl;
//	bf<Bool> f;
//	cout << f << endl;*/
}
