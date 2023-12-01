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

#ifndef __NORMALIZER2_H__
#define __NORMALIZER2_H__

#include <string>
#include <optional>

#include "rewriting.h"
#include "formula.h"

namespace idni::tau {

// tau system library, used to define the tau system of rewriting rules
#define RULE(name, code) const std::string name = code;

// IDEA (MEDIUM) add commutative rule and halve the number of rules if is performance friendly

// bf rules
RULE(BF_SIMPLIFY_ONE_0, "( 1 | $X ) := 1.")
RULE(BF_SIMPLIFY_ONE_1, "( $X | 1 ) := 1.")
RULE(BF_SIMPLIFY_ONE_2, "( 1 & $X ) := $X.")
RULE(BF_SIMPLIFY_ONE_3, "( $X & 1 ) := $X.")
RULE(BF_SIMPLIFY_ONE_4, "~ 1 := 0.")
RULE(BF_SIMPLIFY_ZERO_0, "( 0 & $X ) := 1.")
RULE(BF_SIMPLIFY_ZERO_1, "( $X & 0 ) := 0.")
RULE(BF_SIMPLIFY_ZERO_2, "( 0 | $X ) := $X.")
RULE(BF_SIMPLIFY_ZERO_3, "( $X | 0 ) := $X.")
RULE(BF_SIMPLIFY_ZERO_4, "~ 0 := 1.")
RULE(BF_SIMPLIFY_SELF_0, "( $X & $X ) := $X.")
RULE(BF_SIMPLIFY_SELF_1, "( $X | $X ) := $X.")
RULE(BF_SIMPLIFY_SELF_2, "( $X & ~ $X ) := 0.")
RULE(BF_SIMPLIFY_SELF_3, "( $X | ~ $X ) := 1.")
RULE(BF_SIMPLIFY_SELF_4, "( ~ $X & $X ) := 0.")
RULE(BF_SIMPLIFY_SELF_5, "( ~ $X | $X ) := 1.")

RULE(BF_FUNCTIONAL_QUANTIFIERS_0, "fall $X $Y := ( bf_subs_cb $X 0 $Y & bf_subs_cb $X 1 $Y).")
RULE(BF_FUNCTIONAL_QUANTIFIERS_1, "fex $X $Y := ( bf_subs_cb $X 0 $Y | bf_subs_cb $X 1 $Y).")
RULE(BF_SKIP_CONSTANTS_0, "({ $X } & $Y) := ($Y & { $X }).")

// bf definitions
RULE(BF_DEF_XOR, "( $X + $Y ) := (( $X & ~ $Y ) | ( ~ $X & $Y )).")

// bf callbacks
RULE(BF_CALLBACK_AND, "( { $X } & { $Y } ) := { $X bf_and_cb $Y }.")
RULE(BF_CALLBACK_OR, "( { $X } | { $Y } ) := { $X bf_or_cb $Y }.")
RULE(BF_CALLBACK_XOR, "( { $X } + { $Y } ) := { $X bf_xor_cb $Y }.")
RULE(BF_CALLBACK_NEG, "~ { $X } := { bf_neg_cb $X }.")
RULE(BF_CALLBACK_LESS, "( { $X } < { $Y } ) := bf_less_cb $X $Y T F.")
RULE(BF_CALLBACK_LESS_EQUAL, "( { $X } <= { $Y } ) := bf_less_equal_cb $X $Y T F.")
RULE(BF_CALLBACK_GREATER, "( { $X } > { $Y } ) := bf_greater_cb $X $Y T F.")
RULE(BF_CALLBACK_IS_ZERO, "{ $X } := bf_is_zero_cb { $X } 0.") // (T|F) is bf_(t|f)
RULE(BF_CALLBACK_IS_ONE, "{ $X } := bf_is_one_cb { $X } 1.") // (T|F) is bf_(t|f)

// wff callbacks
RULE(BF_CALLBACK_EQ, "( { $X } = 0 ) := bf_eq_cb $X T F.") // (T|F) is wff_(t|f)
RULE(BF_CALLBACK_NEQ, "( { $X } != 0 ) := bf_neq_cb $X T F.") // (T|F) is wff_(t|f)

// speed up callbacks
RULE(BF_CALLBACK_CLASHING_SUBFORMULAS_0, "( $X & $Y ) :=  bf_has_clashing_subformulas_cb ( $X & $Y ) 0.")
RULE(BF_CALLBACK_HAS_SUBFORMULA_0, "( $X & $Y ) := bf_has_subformula_cb ( $X & $Y ) 0 0.")
RULE(WFF_CALLBACK_CLASHING_SUBFORMULAS_0, "( $X && $Y ) :=  wff_has_clashing_subformulas_cb ( $X && $Y ) F.")
RULE(WFF_CALLBACK_HAS_SUBFORMULA_0, "( $X && $Y ) := wff_has_subformula_cb ( $X && $Y ) F F.")

// wff rules
RULE(WFF_DISTRIBUTE_0, "(($X || $Y) && $Z) := (($X && $Y) || ($X && $Z)).")
RULE(WFF_DISTRIBUTE_1, "($X && ($Y || $Z)) := (($X && $Y) || ($X && $Z)).")
RULE(WFF_PUSH_NEGATION_INWARDS_0, "! ($X && $Y) := (! $X || ! $Y).")
RULE(WFF_PUSH_NEGATION_INWARDS_1, "! ($X || $Y) := (! $X && ! $Y).")
RULE(WFF_PUSH_NEGATION_INWARDS_2, "! ($X = 0) := ($X != 0).")
RULE(WFF_PUSH_NEGATION_INWARDS_3, "! ($X != 0) := ($X = 0).")
RULE(WFF_ELIM_DOUBLE_NEGATION_0, "! ! $X :=  $X.")
RULE(WFF_ELIM_FORALL, "all $X $Y := ! ex $X ! $Y.")
RULE(WFF_SIMPLIFY_ONE_0, "( T || $X ) := T.")
RULE(WFF_SIMPLIFY_ONE_1, "( $X || T ) := T.")
RULE(WFF_SIMPLIFY_ONE_2, "( T && $X ) := $X.")
RULE(WFF_SIMPLIFY_ONE_3, "( $X && T ) := $X.")
RULE(WFF_SIMPLIFY_ONE_4, " ! T := F.")
RULE(WFF_SIMPLIFY_ZERO_0, "( F && $X ) := F.")
RULE(WFF_SIMPLIFY_ZERO_1, "( $X && F ) := F.")
RULE(WFF_SIMPLIFY_ZERO_2, "( F || $X ) := $X.")
RULE(WFF_SIMPLIFY_ZERO_3, "( $X || F ) := $X.")
RULE(WFF_SIMPLIFY_ZERO_4, "! F := T.")
RULE(WFF_SIMPLIFY_SELF_0, "( $X && $X ) := $X.")
RULE(WFF_SIMPLIFY_SELF_1, "( $X || $X ) := $X.")
RULE(WFF_SIMPLIFY_SELF_2, "( $X && ! $X ) := F.")
RULE(WFF_SIMPLIFY_SELF_3, "( $X || ! $X ) := T.")
RULE(WFF_SIMPLIFY_SELF_4, "( ! $X && $X ) := F.")
RULE(WFF_SIMPLIFY_SELF_5, "( ! $X || $X ) := T.")

// wff definitions of xor, ->, <- and <->.
RULE(WFF_DEF_XOR, "( $X ^ $Y ) := (( $X && ! $Y ) || ( ! $X && $Y )).")
RULE(WFF_DEF_IMPLY, "( $X -> $Y ) := ( ! $X || $Y).")
RULE(WFF_DEF_COIMPLY, "( $X wff_coimply $Y ) := ( $Y -> $X).")
RULE(WFF_DEF_EQUIV, "( $X <-> $Y ) := (( $X -> $Y ) && ( $Y -> $X )).")

// TODO (LOW) rename to (N)EQ_SIMPLYFY
RULE(BF_TRIVIALITY_0, "( 0 = 0 ) := T.")
RULE(BF_TRIVIALITY_1, "( 1 = 0 ) :=  F.")
RULE(BF_TRIVIALITY_2, "( 0 != 0 ) := F.")
RULE(BF_TRIVIALITY_3, "( 1 != 0 ) := T.")
// TODO (HIGH) add rules for <, <= and >

RULE(BF_POSITIVE_LITERAL_UPWARDS_0, "(($X != 0) && (($Y = 0) && ($Z != 0))) := (($Y = 0) && (($X != 0) && ($Z != 0))).")
RULE(BF_POSITIVE_LITERAL_UPWARDS_1, "(($X != 0) && (($Y != 0) && ($Z = 0))) := (($Z = 0) && (($X != 0) && ($Y != 0))).")
RULE(BF_POSITIVE_LITERAL_UPWARDS_2, "((($X = 0) && ( $Y != 0)) && ($Z != 0)) := (($X = 0) && (($Y != 0) && ($Z != 0))).")
RULE(BF_POSITIVE_LITERAL_UPWARDS_3, "((($X != 0) && ( $Y = 0)) && ($Z != 0)) := (($Y = 0) && (($X != 0) && ($Z != 0))).")
RULE(BF_POSITIVE_LITERAL_UPWARDS_4, "(($X != 0) && ( $Y = 0)) := (($Y = 0) && ($X != 0)).")
RULE(BF_SQUEEZE_POSITIVES_0, "(( $X = 0 ) && ($Y = 0)) := (( $X | $Y ) = 0).")
RULE(WFF_REMOVE_EX_0, "ex $X $Y := wff_remove_existential_cb $X $Y.")

// TODO (MEDIUM) delete trivial quantified formulas (i.e. ∀x. F = no_x..., ).

// bf defs are just callbacks
template<typename... BAs>
// TODO (LOW) rename library with rwsys or another name
static auto apply_defs = make_library<BAs...>(
	// wff defs
	WFF_DEF_XOR
	+ WFF_DEF_IMPLY
	+ WFF_DEF_COIMPLY
	+ WFF_DEF_EQUIV
	// bf defs
	+ BF_DEF_XOR
);

template<typename... BAs>
static auto elim_for_all = make_library<BAs...>(
	WFF_ELIM_FORALL
);

template<typename... BAs>
static auto to_dnf_wff = make_library<BAs...>(
	WFF_DISTRIBUTE_0
	+ WFF_DISTRIBUTE_1
	+ WFF_PUSH_NEGATION_INWARDS_0
	+ WFF_PUSH_NEGATION_INWARDS_1
	+ WFF_PUSH_NEGATION_INWARDS_2
	+ WFF_PUSH_NEGATION_INWARDS_3
	+ WFF_ELIM_DOUBLE_NEGATION_0
);

template<typename... BAs>
static auto simplify_bf = make_library<BAs...>(
	BF_SIMPLIFY_ONE_0
	+ BF_SIMPLIFY_ONE_1
	+ BF_SIMPLIFY_ONE_2
	+ BF_SIMPLIFY_ONE_3
	+ BF_SIMPLIFY_ONE_4
	+ BF_SIMPLIFY_ZERO_0
	+ BF_SIMPLIFY_ZERO_1
	+ BF_SIMPLIFY_ZERO_2
	+ BF_SIMPLIFY_ZERO_3
	+ BF_SIMPLIFY_ZERO_4
	+ BF_SIMPLIFY_SELF_0
	+ BF_SIMPLIFY_SELF_1
	+ BF_SIMPLIFY_SELF_2
	+ BF_SIMPLIFY_SELF_3
	+ BF_SIMPLIFY_SELF_4
	+ BF_SIMPLIFY_SELF_5
);

template<typename... BAs>
static auto simplify_wff = make_library<BAs...>(
	WFF_SIMPLIFY_ONE_0
	+ WFF_SIMPLIFY_ONE_1
	+ WFF_SIMPLIFY_ONE_2
	+ WFF_SIMPLIFY_ONE_3
	+ WFF_SIMPLIFY_ONE_4
	+ WFF_SIMPLIFY_ZERO_0
	+ WFF_SIMPLIFY_ZERO_1
	+ WFF_SIMPLIFY_ZERO_2
	+ WFF_SIMPLIFY_ZERO_3
	+ WFF_SIMPLIFY_ZERO_4
	+ WFF_SIMPLIFY_SELF_0
	+ WFF_SIMPLIFY_SELF_1
	+ WFF_SIMPLIFY_SELF_2
	+ WFF_SIMPLIFY_SELF_3
	+ WFF_SIMPLIFY_SELF_4
	+ WFF_SIMPLIFY_SELF_5
);

template<typename... BAs>
static auto apply_cb = make_library<BAs...>(
	BF_CALLBACK_AND
	+ BF_CALLBACK_OR
	+ BF_CALLBACK_XOR
	+ BF_CALLBACK_NEG
	+ BF_CALLBACK_LESS
	+ BF_CALLBACK_LESS_EQUAL
	+ BF_CALLBACK_GREATER
	+ BF_CALLBACK_EQ
 	+ BF_CALLBACK_NEQ
);

template<typename... BAs>
static auto apply_speed_up_cb = make_library<BAs...>(
	BF_CALLBACK_CLASHING_SUBFORMULAS_0
	+ BF_CALLBACK_HAS_SUBFORMULA_0
	+ WFF_CALLBACK_CLASHING_SUBFORMULAS_0
	+ WFF_CALLBACK_HAS_SUBFORMULA_0
);

template<typename... BAs>
static auto clause_simplify_bf = make_library<BAs...>(
	BF_CALLBACK_CLASHING_SUBFORMULAS_0
	+ BF_CALLBACK_HAS_SUBFORMULA_0
);

template<typename... BAs>
static auto clause_simplify_wff = make_library<BAs...>(
	WFF_CALLBACK_CLASHING_SUBFORMULAS_0
	+ WFF_CALLBACK_HAS_SUBFORMULA_0
);

template<typename... BAs>
static auto squeeze_positives = make_library<BAs...>(
	BF_SQUEEZE_POSITIVES_0
);

template<typename... BAs>
static auto wff_remove_existential = make_library<BAs...>(
	WFF_REMOVE_EX_0
);

template<typename... BAs>
static auto bf_elim_quantifiers = make_library<BAs...>(
	BF_FUNCTIONAL_QUANTIFIERS_0
	+ BF_FUNCTIONAL_QUANTIFIERS_1
);

template<typename... BAs>
static auto trivialities = make_library<BAs...>(
	BF_TRIVIALITY_0
	+ BF_TRIVIALITY_1
	+ BF_TRIVIALITY_2
	+ BF_TRIVIALITY_3
);

template<typename... BAs>
static auto bf_positives_upwards = make_library<BAs...>(
	BF_POSITIVE_LITERAL_UPWARDS_0
	+ BF_POSITIVE_LITERAL_UPWARDS_1
	+ BF_POSITIVE_LITERAL_UPWARDS_2
	+ BF_POSITIVE_LITERAL_UPWARDS_3
	+ BF_POSITIVE_LITERAL_UPWARDS_4
);

// TODO (MEDIUM) clean execution api code
template<typename... BAs>
struct step {
	step(library<BAs...> lib): lib(lib) {}

	sp_tau_node<BAs...> operator()(const sp_tau_node<BAs...>& n) const {
		return tau_apply(lib, n);
	}

	library<BAs...> lib;
};

template<typename step_t, typename...BAs>
struct steps {

	steps(std::vector<step_t> libraries) : libraries(libraries) {}
	steps(step_t library) {
		libraries.push_back(library);
	}

	sp_tau_node<BAs...> operator()(const sp_tau_node<BAs...>& n) const {
		if (libraries.empty()) return n;
		auto nn = n;
		for (auto& lib : libraries) nn = lib(nn);
		return nn;
	}

	std::vector<step_t> libraries;
};

template<typename step_t, typename... BAs>
struct repeat_each {

	repeat_each(steps<step_t, BAs...> s) : s(s) {}
	repeat_each(step_t s) : s(steps<step_t, BAs...>(s)) {}

	sp_tau_node<BAs...> operator()(const sp_tau_node<BAs...>& n) const {
		auto nn = n;
		for (auto& l: s.libraries) {
			std::set<sp_tau_node<BAs...>> visited;
			while (true) {
				nn = l(nn);
				if (visited.find(nn) != visited.end()) break;
				visited.insert(nn);
			}
		}
		return nn;
	}

	steps<step_t, BAs...> s;
};

template<typename step_t, typename... BAs>
struct repeat_all {

	repeat_all(steps<step_t, BAs...> s) : s(s) {}
	repeat_all(step_t s) : s(steps<step_t, BAs...>(s)) {}

	sp_tau_node<BAs...> operator()(const sp_tau_node<BAs...>& n) const {
		auto nn = n;
		std::set<sp_tau_node<BAs...>> visited;
		while (true) {
			nn = s(nn);
			if (visited.contains(nn)) break;
			visited.insert(nn);
		}
		return nn;
	}

	steps<step_t, BAs...> s;
};

template<typename step_t, typename... BAs>
struct repeat_once {

	repeat_once(steps<step_t, BAs...> s) : s(s) {}
	repeat_once(step_t s) : s(steps<step_t, BAs...>(s)) {}

	sp_tau_node<BAs...> operator()(const sp_tau_node<BAs...>& n) const {
		auto nn = n;
		for(auto& l: s.libraries) {
			nn = l(nn);
		}
		return nn;
	}

	steps<step_t, BAs...> s;
};


template<typename...BAs>
steps<step<BAs...>, BAs...> operator|(const library<BAs...>& l, const library<BAs...>& r) {
	auto s = steps<step<BAs...>, BAs...>(step<BAs...>(l));
	s.libraries.push_back(r);
	return s;
}

template<typename step_t, typename...BAs>
steps<repeat_each<step_t, BAs...>, BAs...> operator|(const repeat_each<step_t, BAs...>& l, const repeat_each<step_t, BAs...>& r) {
	auto s = steps<repeat_each<step_t, BAs...>, BAs...>(l);
	s.libraries.push_back(r);
	return s;
}

template<typename step_t, typename...BAs>
steps<repeat_all<step_t, BAs...>, BAs...> operator|(const repeat_all<step_t, BAs...>& l, const repeat_all<step_t, BAs...>& r) {
	auto s = steps<repeat_all<step_t, BAs...>, BAs...>(l);
	s.libraries.push_back(r);
	return s;
}

template<typename step_t, typename... BAs>
steps<step_t, BAs...> operator|(const steps<step_t, BAs...>& s, const step_t& l) {
	auto ns = s;
	ns.libraries.push_back(l);
	return ns;
}

template<typename step_t, typename... BAs>
steps<step_t, BAs...> operator|(const steps<step_t, BAs...>& s, const library<BAs...>& l) {
	auto ns = s;
	ns.libraries.push_back(l);
	return ns;
}

template<typename... BAs>
steps<step<library<BAs...>, BAs...>, BAs...> operator|(const steps<step<library<BAs...>, BAs...>, BAs...>& s, const library<BAs...>& l) {
	auto ns = s;
	ns.libraries.push_back(l);
	return ns;
}

template<typename step_t, typename... BAs>
sp_tau_node<BAs...> operator|(const sp_tau_node<BAs...>& n, const steps<step_t, BAs...>& s) {
	return s(n);
}

template<typename... BAs>
sp_tau_node<BAs...> operator|(const sp_tau_node<BAs...>& n, const library<BAs...>& l) {
	auto s = steps<library<BAs...>, BAs...>(l);
	return s(n);
}

template<typename step_t, typename... BAs>
sp_tau_node<BAs...> operator|(const sp_tau_node<BAs...>& n, const repeat_once<step_t, BAs...>& r) {
	return r(n);
}

template<typename step_t, typename... BAs>
sp_tau_node<BAs...> operator|(const sp_tau_node<BAs...>& n, const repeat_all<step_t, BAs...>& r) {
	return r(n);
}

template<typename step_t, typename... BAs>
sp_tau_node<BAs...> operator|(const sp_tau_node<BAs...>& n, const repeat_each<step_t, BAs...>& r) {
	return r(n);
}

template <typename... BAs>
formula<BAs...> normalizer_step(formula<BAs...>& form) {

	#ifdef OUTPUT_APPLY_RULES
	std::cout << "(I): -- Begin normalizer step" << std::endl;
	std::cout << "(F): " << form.main << std::endl;
	#endif // OUTPUT_APPLY_RULES

	auto nmain = form.main
			| repeat_all<step<BAs...>, BAs...>(
				step<BAs...>(form.rec_relations))
			| repeat_all<step<BAs...>, BAs...>(
				step<BAs...>(apply_defs<BAs...>))
			| repeat_all<step<BAs...>, BAs...>(
				step<BAs...>(elim_for_all<BAs...>))
			| repeat_all<step<BAs...>, BAs...>(
				to_dnf_wff<BAs...>
				| simplify_wff<BAs...>
				| clause_simplify_wff<BAs...>)
			| repeat_all<step<BAs...>, BAs...>(
				bf_positives_upwards<BAs...>
				| squeeze_positives<BAs...>
				| wff_remove_existential<BAs...>)
			| repeat_all<step<BAs...>, BAs...>(
				bf_elim_quantifiers<BAs...>
				| simplify_bf<BAs...>
				| apply_cb<BAs...>
				| clause_simplify_bf<BAs...>
				| trivialities<BAs...>
				| to_dnf_wff<BAs...>
				| simplify_wff<BAs...>
				| clause_simplify_wff<BAs...>);
	#ifdef OUTPUT_APPLY_RULES
	std::cout << "(I): -- End normalizer step" << std::endl;
	#endif // OUTPUT_APPLY_RULES

	return { form.rec_relations, nmain };
}

// REVIEW could we assume we are working with the product algebra?

// this should be used in conjuction with std::set. it must provide
// a strict weak ordering in such a way that equivalent formulas are
// considered equal.

// executes the normalizer on the given source code taking into account the
// bindings provided.
template<typename... BAs>
formula<BAs...> normalizer(std::string& source, bindings<BAs...>& binds) {
	auto form_source = make_tau_source(source);
	auto form = make_formula_using_bindings(form_source, binds);
	return normalizer(form);
}

// executes the normalizer on the given source code taking into account the
// provided factory.
template<typename factory_t, typename... BAs>
formula<BAs...> normalizer(std::string& source, factory_t& factory) {
	auto form_source = make_tau_source(source);
	auto form = make_formula_using_factory(form_source, factory);
	return normalizer(form);
}


template <typename... BAs>
struct is_equivalent_predicate {

	is_equivalent_predicate(sp_tau_node<BAs...> node) : node(node) {
		node_free_variables = free_variables(node);
	}

	bool operator()(sp_tau_node<BAs...>& n) {
		std::set<sp_tau_node<BAs...>> free_vars = free_variables(n);
		free_vars.insert(node_free_variables.begin(), node_free_variables.end());
		sp_tau_node<BAs...> wff = build_wff_equiv<BAs...>(node, n);
		for(auto& v: free_vars) wff = build_wff_all<BAs...>(v, wff);
		rules<BAs...> rls;
		formula<BAs...> form{rls, wff};
		auto norm_form = normalizer(form);
		auto check = norm_form.main | tau_parser::wff | tau_parser::wff_t;
		return check.has_value();
	}

	sp_tau_node<BAs...> node;
	std::set<sp_tau_node<BAs...>> node_free_variables;
private:

	std::set<sp_tau_node<BAs...>> free_variables(sp_tau_node<BAs...>& n) {
		auto captures = select_all(n, is_non_terminal<tau_parser::capture, BAs...>);
		std::set<sp_tau_node<BAs...>> vars(captures.begin(), captures.end());
		return vars;
	}
};

template <typename... BAs>
formula<BAs...> normalizer(formula<BAs...>& form) {

	#ifdef OUTPUT_APPLY_RULES
	std::cout << "(I): -- Begin normalizer" << std::endl;
	#endif // OUTPUT_APPLY_RULES

	std::vector<sp_tau_node<BAs...>> previous;
	formula<BAs...> current = normalizer_step(form);
	auto is_equivalent = is_equivalent_predicate<BAs...>(current.main);
	while (std::find_if(previous.rend(), previous.rbegin(),  is_equivalent) == previous.rend()) {
		previous.push_back(current.main);
		current = normalizer_step(current);
	}

	#ifdef OUTPUT_APPLY_RULES
	std::cout << "(I): -- End normalizer" << std::endl;
	std::cout << "(O): " << current.main << std::endl;
	#endif // OUTPUT_APPLY_RULES

	return current;
}

} // namespace idni::tau

#endif // __NORMALIZER2_H__
