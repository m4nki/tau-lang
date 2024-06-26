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

// TODO (LOW) rename file to msnso_rr.h
// TODO (MEDIUM) fix proper types (alias) at this level of abstraction
//
// We should talk about statement, nso_rr (nso_with_rr?), library, rule, builder,
// bindings, etc... instead of sp_tau_node,...

#ifndef __NSO_RR_H__
#define __NSO_RR_H__

#include <map>
#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <algorithm>
#include <functional>
#include <ranges>
#include <variant>

//#include "tree.h"
#include "bool.h"
#include "rewriting.h"
#include "parser.h"
#include "../parser/tau_parser.generated.h"
#ifdef DEBUG
#	include "parser_instance.h"
#endif

using namespace idni::rewriter;

namespace idni::tau {


//
// types related to the tau language
//

// tau_source_node is the type of nodes we use to represent get from parsing tau language
using tau_source_sym = idni::lit<char, char>;
using tau_source_node = node<idni::lit<char, char>>;
using sp_tau_source_node = sp_node<idni::lit<char, char>>;

// node type for the tau language related programs, libraries and
// specifications trees.
template <typename... BAs>
using tau_sym = std::variant<tau_source_sym, std::variant<BAs...>, size_t>;

template <typename... BAs>
using sp_tau_node = sp_node<tau_sym<BAs...>>;

template<typename... BAs>
using nso = sp_tau_node<BAs...>;

template <typename node_t>
using rec_relation = rule<node_t>;

// IDEA maybe we could define a wrapper for recursive rules and rewriting rules that
// call the appropriate apply method. This would play also nice with the builders
// defined in the normalizer.

template <typename... BAs>
using builder = rule<nso<BAs...>>;

// defines a vector of rules in the tau language, the order is important as it defines
// the order of the rules in the rewriting process of the tau language.
template <typename node_t>
using rules = std::vector<rule<node_t>>;

// defines a vector of rec. relations in the tau language, the order is important as it defines
// the order of the rec relations in the rewriting process of the tau language.
template <typename node_t>
using rec_relations = std::vector<rec_relation<node_t>>;

// a library is a set of rules to be applied in the rewriting process of the tau
// language, the order of the rules is important.
template <typename node_t>
using library = rules<node_t>;

// bindings map tau_source constants (strings) into elements of the boolean algebras.
template<typename... BAs>
using bindings = std::map<std::string, std::variant<BAs...>>;

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// an rr is a set of rules and a main.
template<typename type_t>
struct rr {

	rr(const rules<type_t>& rec_relations, const type_t& main) : rec_relations(rec_relations), main(main) {};
	rr(const type_t& main) : main(main) {};

	auto operator<=>(const rr&) const = default;

	rules<type_t> rec_relations;
	type_t main;
};

//
// predicates and functions related to the tau language
//

// check if a node is a non terminal node
template<typename... BAs>
bool is_non_terminal_node(const sp_tau_node<BAs...>& n) {
	return std::holds_alternative<tau_source_sym>(n->value) && get<tau_source_sym>(n->value).nt();
};

// factory method for is_non_terminal_node predicate
template<typename... BAs>
std::function<bool(const sp_tau_node<BAs...>&)> is_non_terminal_node() {
	return [](const sp_tau_node<BAs...>& n) { return is_non_terminal_node<BAs...>(n); };
}

// check if the node is the given non terminal
template <typename...BAs>
bool is_non_terminal(const size_t nt, const sp_tau_node<BAs...>& n) {
	return is_non_terminal_node<BAs...>(n) && get<tau_source_sym>(n->value).n() == nt;
}

// check if the node is the given non terminal (template approach)
template <size_t nt, typename...BAs>
bool is_non_terminal(const sp_tau_node<BAs...>& n) {
	return is_non_terminal<BAs...>(nt, n);
}

// factory method for is_non_terminal predicate
template<typename... BAs>
std::function<bool(const sp_tau_node<BAs...>&)> is_non_terminal(const size_t nt) {
	return [nt](const sp_tau_node<BAs...>& n) { return is_non_terminal<BAs...>(nt, n); };
}

// check if a node is a terminal
template<typename... BAs>
bool is_terminal_node(const sp_tau_node<BAs...>& n) {
	return std::holds_alternative<tau_source_sym>(n->value) && !get<tau_source_sym>(n->value).nt();
};

// factory method for is_terminal_node predicate
template<typename... BAs>
std::function<bool(const sp_tau_node<BAs...>&)> is_terminal_node() {
	return [](const sp_tau_node<BAs...>& n) { return is_terminal_node<BAs...>(n); };
}

// check if the node is the given terminal (functional approach)
template <typename...BAs>
bool is_terminal(const char c, const sp_tau_node<BAs...>& n) {
	return is_terminal<BAs...>(n) && get<tau_source_sym>(n->value).n() == c;
};

// check if the node is the given terminal (template approach)
template <char c, typename...BAs>
bool is_terminal(const sp_tau_node<BAs...>& n) {
	return is_terminal<BAs...>(c, n);
};

// factory method for is_terminal predicate
template<typename... BAs>
std::function<bool(const sp_tau_node<BAs...>&)> is_terminal(char c) {
	return [c](const sp_tau_node<BAs...>& n) { return is_terminal<BAs...>(c, n); };
}

template<typename... BAs>
static const auto is_capture = [](const sp_tau_node<BAs...>& n) {
	return std::holds_alternative<tau_source_sym>(n->value)
		&& get<tau_source_sym>(n->value).nt()
		&& ( get<tau_source_sym>(n->value).n() == tau_parser::capture);
};

template<typename... BAs>
using is_capture_t = decltype(is_capture<BAs...>);

template<typename... BAs>
static const auto is_var_or_capture = [](const nso<BAs...>& n) {
	return std::holds_alternative<tau_source_sym>(n->value)
		&& get<tau_source_sym>(n->value).nt()
		&& (( get<tau_source_sym>(n->value).n() == tau_parser::capture)
			|| ( get<tau_source_sym>(n->value).n() == tau_parser::variable));
};

template<typename... BAs>
static const auto is_quantifier = [](const nso<BAs...>& n) {
	if (!std::holds_alternative<tau_source_sym>(n->value) || !get<tau_source_sym>(n->value).nt()) return false;
	auto nt = get<tau_source_sym>(n->value).n();
	return nt == tau_parser::bf_all
		|| nt == tau_parser::bf_ex
		|| nt == tau_parser::wff_all
		|| nt == tau_parser::wff_ex
		|| nt == tau_parser::wff_ball
		|| nt == tau_parser::wff_bex;
};

template<typename... BAs>
using is_var_or_capture_t = decltype(is_var_or_capture<BAs...>);

extern std::function<bool(const size_t n)> is_non_essential_terminal;

extern std::function<bool(const tau_source_sym&)> is_non_essential_sym;

extern std::function<bool(const sp_tau_source_node&)> is_non_essential_source;

using is_non_essential_source_t = decltype(is_non_essential_source);

template<typename...BAs>
auto is_non_essential = [] (const sp_tau_node<BAs...>& n) {
	if (!std::holds_alternative<tau_source_sym>(n->value)) return false;
	return is_non_essential_sym(std::get<tau_source_sym>(n->value));
};

template<typename...BAs>
using is_non_essential_t = decltype(is_non_essential<BAs...>);

template<typename...BAs>
static const auto is_callback = [](const sp_tau_node<BAs...>& n) {
	if (!std::holds_alternative<tau_source_sym>(n->value) || !get<tau_source_sym>(n->value).nt()) return false;
	auto nt = get<tau_source_sym>(n->value).n();
	return nt == tau_parser::bf_and_cb
		|| nt == tau_parser::bf_or_cb
		|| nt == tau_parser::bf_xor_cb
		|| nt == tau_parser::bf_neg_cb
		|| nt == tau_parser::bf_eq_cb
		|| nt == tau_parser::bf_neq_cb
		|| nt == tau_parser::bf_is_one_cb
		|| nt == tau_parser::bf_is_zero_cb
		|| nt == tau_parser::wff_has_clashing_subformulas_cb
		|| nt == tau_parser::bf_has_subformula_cb
		|| nt == tau_parser::wff_has_subformula_cb
		|| nt == tau_parser::wff_remove_existential_cb
		|| nt == tau_parser::wff_remove_buniversal_cb
		|| nt == tau_parser::wff_remove_bexistential_cb
		|| nt == tau_parser::bf_remove_funiversal_cb
		|| nt == tau_parser::bf_remove_fexistential_cb;
};

template<typename...BAs>
using is_callback_t = decltype(is_callback<BAs...>);


//
// functions to traverse the tree according to the specified non terminals
// and collect the corresponding nodes
//
//

// traverse the tree, depth first, according to the specified non
// terminals and return, if possible, the required non terminal node
template <typename... BAs>
std::optional<sp_tau_node<BAs...>> operator|(const sp_tau_node<BAs...>& n, const size_t nt) {
	auto v = n->child
		| std::ranges::views::filter(is_non_terminal<BAs...>(nt))
		| std::ranges::views::take(1);
	return v.empty() ? std::optional<sp_tau_node<BAs...>>() : std::optional<sp_tau_node<BAs...>>(v.front());
}

template <typename... BAs>
std::optional<sp_tau_node<BAs...>> operator|(const std::optional<sp_tau_node<BAs...>>& n, const size_t nt) {
	#ifdef DEBUG
	if (!n.has_value()) BOOST_LOG_TRIVIAL(warning)
		<< "value-less optional | tau_parser::"
		<< parser_instance<tau_parser>().name(nt);
	#endif // DEBUG
	return n ? n.value() | nt : n;
}

template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator|(const std::vector<sp_tau_node<BAs...>>& v, const size_t nt) {
	// IDEA use ::to to get a vector when gcc and clang implement it in the future
	std::vector<sp_tau_node<BAs...>> nv;
	for (const auto& n: v
			| std::ranges::views::transform(get_node<BAs...>(nt))
			| std::ranges::views::join)
		nv.emplace(std::move(n));
	return nv;
}

// traverse the tree, top down, and return all the nodes accessible according
// to the specified non terminals and return them
template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator||(const sp_tau_node<BAs...>& n, const tau_parser::nonterminal nt) {
	// IDEA use ::to to get a vector when gcc and clang implement it in the future
	std::vector<sp_tau_node<BAs...>> nv;
	nv.reserve(n->child.size());
	for (const auto& c: n->child
			| std::ranges::views::filter(is_non_terminal<BAs...>(nt)))
		nv.push_back(c);
	return nv;
}

template <typename... BAs>
std::vector<sp_tau_node<BAs...>>  operator||(const std::optional<sp_tau_node<BAs...>>& n, const tau_parser::nonterminal nt) {
	// IDEA use ::to to get a vector when gcc and clang implement it in the future
	if (n) return n.value() || nt;
	return {};
}

// TODO (LOW) remove get_nodes if possible and use operator|| instead
template <typename... BAs>
std::vector<sp_tau_node<BAs...>> get_nodes(const tau_parser::nonterminal nt, const sp_tau_node<BAs...>& n) {
	return n || nt;
}

template <size_t nt, typename... BAs>
std::vector<sp_tau_node<BAs...>> get_nodes(const sp_tau_node<BAs...>& n) {
	return n || nt;
}

template <typename... BAs>
auto get_nodes(const tau_parser::nonterminal nt) {
	return [nt](const sp_tau_node<BAs...>& n) { return get_nodes<BAs...>(nt, n); };
}

template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator||(const std::vector<sp_tau_node<BAs...>>& v, const tau_parser::nonterminal nt) {
	// IDEA use ::to to get a vector when gcc and clang implement it in the future
	std::vector<sp_tau_node<BAs...>> nv; nv.reserve(v.size());
	for (const auto& n: v
			| std::ranges::views::transform(get_nodes<BAs...>(nt))
			| std::ranges::views::join)
		nv.push_back(n);
	return nv;
}

//
// functions to extract components/informatino from nodes
//
//

// extract the value of the node
template<typename... BAs>
static const auto value_extractor = [](const sp_tau_node<BAs...>& n) -> tau_sym<BAs...> {
	return n->value;
};

template<typename... BAs>
using value_extractor_t = decltype(value_extractor<BAs...>);

template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator||(const std::vector<sp_tau_node<BAs...>>& v, const value_extractor_t<BAs...> e) {
	std::vector<std::variant<BAs...>> nv;
	for (const auto& n: v | std::ranges::views::transform(e)) if (n.has_value()) nv.push_back(n.value());
	return nv;
}

template <typename... BAs>
std::optional<char> operator|(const std::optional<sp_tau_node<BAs...>>& o, const value_extractor_t<BAs...> e) {
	// IDEA use o.transform(e) from C++23 when implemented in the future by gcc/clang
	return o.has_value() ? e(o.value()) : std::optional<char>();
}

// returns an optional containing the terminal of the node if possible
template<typename... BAs>
static const auto terminal_extractor = [](const sp_tau_node<BAs...>& n) -> std::optional<char> {
	auto value = n->value;
	if (!std::holds_alternative<tau_source_sym>(value)
			|| get<tau_source_sym>(value).nt()
			|| get<tau_source_sym>(value).is_null())
		return std::optional<char>();
	return std::optional<char>(get<tau_source_sym>(value).t());
};

template<typename... BAs>
using terminal_extractor_t = decltype(terminal_extractor<BAs...>);

template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator||(const std::vector<sp_tau_node<BAs...>>& v, const terminal_extractor_t<BAs...> e) {
	std::vector<std::variant<BAs...>> nv;
	for (const auto& n: v | std::ranges::views::transform(e)) if (n.has_value()) nv.push_back(n.value());
	return nv;
}

template <typename... BAs>
std::optional<char> operator|(const std::optional<sp_tau_node<BAs...>>& o, const terminal_extractor_t<BAs...> e) {
	// IDEA use o.transform(e) from C++23 when implemented in the future by gcc/clang
	return o.has_value() ? e(o.value()) : std::optional<char>();
}

// returns an optional containing the non terminal of the node if possible
template<typename... BAs>
static const auto non_terminal_extractor = [](const sp_tau_node<BAs...>& n) -> std::optional<size_t> {
	if (std::holds_alternative<tau_source_sym>(n->value)
			&& get<tau_source_sym>(n->value).nt())
		return std::optional<size_t>(get<tau_source_sym>(n->value).n());
	return std::optional<size_t>();
};

template<typename... BAs>
using non_terminal_extractor_t = decltype(non_terminal_extractor<BAs...>);

template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator||(const std::vector<sp_tau_node<BAs...>>& v, const non_terminal_extractor_t<BAs...> e) {
	return v | std::ranges::views::transform(e);
}

template <typename... BAs>
std::optional<size_t> operator|(const std::optional<sp_tau_node<BAs...>>& o, const non_terminal_extractor_t<BAs...> e) {
	// IDEA use o.transform(e) from C++23 when implemented in the future by gcc/clang
	return o.has_value() ? e(o.value()) : std::optional<size_t>();
}

template <typename... BAs>
std::optional<size_t> operator|(const sp_tau_node<BAs...>& o, const non_terminal_extractor_t<BAs...> e) {
	return e(o);
}

// returns an optional containing size_t of the node if possible
template <typename... BAs>
static const auto size_t_extractor = [](const sp_tau_node<BAs...>& n) -> std::optional<size_t> {
	if (std::holds_alternative<size_t>(n->value))
		return std::optional<size_t>(std::get<size_t>(n->value));
	return std::optional<size_t>();
};
template<typename... BAs>
using size_t_extractor_t = decltype(size_t_extractor<BAs...>);

template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator||(const std::vector<sp_tau_node<BAs...>>& v, const size_t_extractor_t<BAs...> e) {
	return v | std::ranges::views::transform(e);
}

template <typename... BAs>
std::optional<size_t> operator|(const std::optional<sp_tau_node<BAs...>>& o, const size_t_extractor_t<BAs...> e) {
	return o.has_value() ? e(o.value()) : std::optional<size_t>();
}

template <typename... BAs>
std::optional<size_t> operator|(const sp_tau_node<BAs...>& o, const size_t_extractor_t<BAs...> e) {
	return e(o);
}

// returns an optional containing the offset of the node if possible
template <typename...BAs>
static const auto& offset_extractor = size_t_extractor<BAs...>;
template <typename...BAs>
using offset_extractor_t = size_t_extractor_t<BAs...>;

// returns an optional containing the bas... of the node if possible
template<typename... BAs>
static const auto ba_extractor = [](const sp_tau_node<BAs...>& n) -> std::optional<std::variant<BAs...>> {
	if (std::holds_alternative<std::variant<BAs...>>(n->value))
		return std::optional<std::variant<BAs...>>(get<std::variant<BAs...>>(n->value));
	return std::optional<std::variant<BAs...>>();
};

template<typename... BAs>
using ba_extractor_t = decltype(ba_extractor<BAs...>);

template <typename... BAs>
std::vector<std::variant<BAs...>> operator||(const std::vector<sp_tau_node<BAs...>>& v, const ba_extractor_t<BAs...> e) {
	std::vector<std::variant<BAs...>> nv;
	for (const auto& n: v | std::ranges::views::transform(e)) if (n.has_value()) nv.push_back(n.value());
	return nv;
}

template <typename... BAs>
std::optional<std::variant<BAs...>> operator|(const std::optional<sp_tau_node<BAs...>>& o, const ba_extractor_t<BAs...> e) {
	// IDEA use o.transform(e) from C++23 when implemented in the future by gcc/clang
	return o.has_value() ? e(o.value()) : std::optional<std::variant<BAs...>>();
}

template <typename... BAs>
std::optional<std::variant<BAs...>> operator|(const sp_tau_node<BAs...>& o, const ba_extractor_t<BAs...> e) {
	// IDEA use o.transform(e) from C++23 when implemented in the future by gcc/clang
	return e(o);
}

// returns the only child of a node
template <typename... BAs>
static const auto only_child_extractor = [](const sp_tau_node<BAs...>& n) -> std::optional<sp_tau_node<BAs...>> {
	if (n->child.size() != 1) return std::optional<sp_tau_node<BAs...>>();
	return std::optional<sp_tau_node<BAs...>>(n->child[0]);
};

template<typename... BAs>
using only_child_extractor_t = decltype(only_child_extractor<BAs...>);

template <typename... BAs>
std::vector<sp_tau_node<BAs...>> operator||(const std::vector<sp_tau_node<BAs...>>& v, const only_child_extractor_t<BAs...> e) {
	std::vector<sp_tau_node<BAs...>> nv;
	for (const auto& n: v | std::ranges::views::transform(e)) if (n.has_value()) nv.push_back(n.value());
	return nv;
}

template <typename... BAs>
std::optional<sp_tau_node<BAs...>> operator|(const std::optional<sp_tau_node<BAs...>>& o, const only_child_extractor_t<BAs...> e) {
	// IDEA use o.transform(e) from C++23 when implemented in the future by gcc/clang
	return o.has_value() ? e(o.value()) : std::optional<sp_tau_node<BAs...>>();
}

// IDEA maybe unify all the implementations dealing with operator| and operator|| for extractors
template <typename... BAs>
std::optional<sp_tau_node<BAs...>> operator|(const sp_tau_node<BAs...>& o, const only_child_extractor_t<BAs...> e) {
	// IDEA use o.transform(e) from C++23 when implemented in the future by gcc/clang
	return e(o);
}

template <typename T>
static const auto optional_value_extractor = [](const std::optional<T>& o) -> T {return o.value();} ;

template <typename T>
using optional_value_extractor_t = decltype(optional_value_extractor<T>);

template <typename T>
T operator|(const std::optional<T>& o, const optional_value_extractor_t<T> e) {
	return e(o);
}

//
// functions traversing the nodes according to the specified non terminals and
// extracting the required information from them
//
//

// returns the extracted component/information of the specified node if possible,
// the component/information is extracted using the given extractor.
template <size_t... nt, typename extractor_t, typename... BAs>
auto get_optional(const extractor_t& extractor, const sp_tau_node<BAs...>& n) {
	return get_node<nt..., BAs...>(n).and_then(extractor);
}

// returns the extracted components/informations of the specified nodes, the
// component/information is extracted using the given extractor.
template <size_t... nt, typename extractor_t, typename... BAs>
auto get_optionals(const extractor_t& extractor, const sp_tau_node<BAs...>& n) {
	return get_nodes<nt..., BAs...>(n)
		| std::ranges::views::transform(extractor)
		| std::ranges::views::filter(&std::optional<tau_sym<BAs...>>::has_value);
}


// converts a tau_source_sym to a tau_sym, it is used to convert from
// tau_source to tau_node...
template <typename... BAs>
struct tauify {

	tau_sym<BAs...> operator()(const tau_source_sym& n) const {
		tau_sym<BAs...> nn(n);
		return nn;
	}
};

// extracts terminal from sp_tau_node
template <typename... BAs>
auto tau_node_terminal_extractor = [](const sp_tau_node<BAs...>& n) -> std::optional<char> {
	if (n->value.index() == 0
			&& !get<0>(n->value).nt()
			&& !get<0>(n->value).is_null())
		return std::optional<char>(get<0>(n->value).t());
	return std::optional<char>();
};

template <typename... BAs>
using tau_node_terminal_extractor_t = decltype(tau_node_terminal_extractor<BAs...>);

// extracts terminal from sp_tau_source_node
extern std::function<std::optional<char>(const sp_tau_source_node& n)>
	tau_source_terminal_extractor;

template <typename... BAs>
using tau_source_terminal_extractor_t = decltype(tau_source_terminal_extractor);

// adds terminal symbols to a given stream, used in conjuction with usual
// traversals (see make_string_* methods).
template <typename extractor_t, typename node_t>
struct stringify {

	stringify(const extractor_t& extractor, std::basic_stringstream<char>& stream)
		: stream(stream), extractor(extractor) {}

	node_t operator()(const node_t& n) {
		if (auto str = extractor(n); str)
			stream << str.value();
		return n;
	}

	std::basic_stringstream<char>& stream;
	const extractor_t& extractor;
};

// converts a sp_tau_node<...> to a string skipping the nodes that satisfy the
// given predicate.
template <typename extractor_t, typename predicate_t, typename node_t>
std::string make_string_with_skip(const extractor_t& extractor, predicate_t& skip, const node_t& n) {
	std::basic_stringstream<char> ss;
	stringify<extractor_t, node_t> sy(extractor, ss);
	post_order_tree_traverser<stringify<extractor_t, node_t>, predicate_t, node_t>(sy, skip)(n);
	return ss.str();
}

// converts a sp_tau_node<...> to a string.
template <typename extractor_t, typename node_t>
std::string make_string(const extractor_t& extractor, const node_t& n) {
	return make_string_with_skip<extractor_t, all_t<node_t>, node_t>(extractor, all<node_t>, n);
}

// bind the given, using a binder, the constants of the a given sp_tau_node<...>.
template<typename binder_t, typename... BAs>
struct bind_transformer {

	bind_transformer(binder_t& binder) : binder(binder) {}

	sp_tau_node<BAs...> operator()(const sp_tau_node<BAs...>& n) {
		if (auto it = changes.find(n); it != changes.end()) return it->second;
		if (is_non_terminal<tau_parser::binding, BAs...>(n))
			return changes.emplace(n, binder.bind(n)).first->second;
		// IDEA maybe we could use the replace transform instead of having the following code
		bool changed = false;
		std::vector<sp_tau_node<BAs...>> child;
		for (auto& c : n->child)
			if (changes.contains(c)) changed = true, child.push_back(changes[c]);
			else child.push_back(c);
		auto nn = make_node<tau_sym<BAs...>>(n->value, child);
		if (changed)
			changes[n] = nn;
		return nn;
	}

	std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> changes;
	binder_t binder;
};

// is not a whitespace predicate
template<typename... BAs>
static const auto not_whitespace_predicate = [](const sp_tau_node<BAs...>& n) {
	return n->value.index() != 0
		|| !get<0>(n->value).nt()
		|| (get<0>(n->value).n() != tau_parser::_ &&
			get<0>(n->value).n() != tau_parser::__);
};

template<typename... BAs>
using not_whitespace_predicate_t = decltype(not_whitespace_predicate<BAs...>);

// binds the constants of a given binding using the label specified
// in the code and according to a map from labels to constants in the BAs...
template<typename... BAs>
struct name_binder {

	name_binder(const bindings<BAs...>& bs) : bs(bs) {}

	sp_tau_node<BAs...> bind(const sp_tau_node<BAs...>& n) const {
		// FIXME (LOW) check if the node is a named binding one
		auto bn = make_string_with_skip<
				tau_node_terminal_extractor_t<BAs...>,
				not_whitespace_predicate_t<BAs...>,
				sp_tau_node<BAs...>>(
			tau_node_terminal_extractor<BAs...>,
			not_whitespace_predicate<BAs...>, n);
		auto s = bs.find(bn);
		if (s != bs.end()) {
			tau_sym<BAs...> ts = s->second;
			return make_node<tau_sym<BAs...>>(ts, {});
		}
		return n;
	}

	const bindings<BAs...>& bs;
};

// binds the constants of a given binding using the multi-factory for the types
// supported.
template<typename factory_t, typename... BAs>
struct factory_binder {

	factory_binder(factory_t& factory) : factory(factory) {}

	sp_tau_node<BAs...> bind(const sp_tau_node<BAs...>& n) const {
		// FIXME (LOW) check that the node is a factory binding one
		if(auto type = find_top(n, is_non_terminal<tau_parser::type, BAs...>); type) {
			// the factory take two arguments, the first is the type and the
			// second is the node representing the constant.
			auto type_name = make_string_with_skip<
					tau_node_terminal_extractor_t<BAs...>,
					not_whitespace_predicate_t<BAs...>,
					sp_tau_node<BAs...>>(
				tau_node_terminal_extractor<BAs...>,
				not_whitespace_predicate<BAs...>, type.value());
			return factory.build(type_name, n);
		}
		return n;
	}

	factory_t& factory;
};

// TODO (HIGH) improve type resolution adding types to quantifiers.
//
// Type resolution is used to ensure that all the Boolean Algebra operations are
// consistente. Types could be specified at two levels:
//
// - At the boolean algebra expression level: in this case, the type of the
//   expression is the type of one of the constants of the boolean expression,
//   there is no need to provide the type in each of them.
//
// - At the quantifier expression level: in this case, the type of a variable is
//   specified and all the underlying boolean algebra expressions are typed
//   accordingly.
//
// In both cases, if there is a missmatch between the types, an error is raised
// during the type resolution phase, i.e. before executing the program.


// check if the type is unresolved.
template<typename... BAs>
static const auto is_unresolved_predicate = [](const sp_tau_node<BAs...>& n) {
	if (!is_non_terminal<tau_parser::type, BAs...>(n)) return false;
	auto type_name = make_string_with_skip<
			tau_node_terminal_extractor_t<BAs...>,
			not_whitespace_predicate_t<BAs...>,
			sp_tau_node<BAs...>>(
			tau_node_terminal_extractor<BAs...>,
		not_whitespace_predicate<BAs...>, n);
	return type_name.empty();
};

template<typename... BAs>
using is_unresolved_predicate_t = decltype(is_unresolved_predicate<BAs...>);

// check if the type is resolved.
template<typename... BAs>
static const auto is_resolved_predicate = [](const sp_tau_node<BAs...>& n) {
	if (!is_non_terminal<tau_parser::type, BAs...>(n)) return false;
	auto type_name = make_string_with_skip<
			tau_node_terminal_extractor_t<BAs...>,
			not_whitespace_predicate_t<BAs...>,
			sp_tau_node<BAs...>>(
			tau_node_terminal_extractor<BAs...>,
		not_whitespace_predicate<BAs...>, n);
	return !type_name.empty();
};

template<typename... BAs>
using is_resolved_predicate_t = decltype(is_resolved_predicate<BAs...>);

// check if the given expression is unresolved, i.e. contains a type which
// is not resolved.
template<typename... BAs>
std::optional<sp_tau_node<BAs...>> is_unresolved(const sp_tau_node<BAs...>& n) {
	return find_top(n, is_unresolved_predicate<BAs...>);
}

// resolve all the unresolved types in the given expression. the types are
// resolved to the first bottom type resolved in the expression.
template<typename... BAs>
sp_tau_node<BAs...> resolve_type(const sp_tau_node<BAs...>& n) {
	if (auto unresolved = is_unresolved(n); unresolved) {
		// always we have type information or it is not needed at all
		if (auto type = find_bottom(n, is_resolved_predicate<BAs...>); type) {
			std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> change {{unresolved.value(), type.value()}};
			return replace<sp_tau_node<BAs...>>(n, change);
		}
	}
	return n;
}

// resolve all the unresolved types in the given statement.
template<typename binder_t, typename... BAs>
sp_tau_node<BAs...> resolve_types(const sp_tau_node<BAs...> source) {
	std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> changes;
	for (const auto& bf: select_top(source, is_non_terminal<BAs...>(tau_parser::bf))) {
		if (auto rbf = resolve_type(bf); rbf != bf) changes[bf] = rbf;
	}
	return replace<sp_tau_node<BAs...>>(source, changes);
}

// resolve all the unresolved types in the given rules.
template<typename... BAs>
rules<nso<BAs...>> resolve_types(const rules<nso<BAs...>> rs) {
	rules<nso<BAs...>> nrs;
	for (const auto& r : rs) {
		auto nr = resolve_types(r);
		nrs.push_back(nr);
	}
	return nrs;
}

// resolve all the unresolved types in the given library.
template<typename binder_t, typename... BAs>
library<nso<BAs...>> resolve_types(const library<nso<BAs...>> lib) {
	return { resolve_types(lib.system) };
}

// resolve all the unresolved types in the given nso_rr.
template<typename binder_t, typename... BAs>
rr<nso<BAs...>> resolve_types(const rr<nso<BAs...>> f) {
	return { resolve_types(f.rec_relations), resolve_type(f.main) };
}

// creates a specific rule from a generic rule
// TODO (LOW) should depend in node_t instead of BAs...
template<typename... BAs>
rule<nso<BAs...>> make_rule(tau_parser::nonterminal rule_t, tau_parser::nonterminal matcher_t, tau_parser::nonterminal body_t, sp_tau_node<BAs...>& rule) {
	auto matcher = rule | rule_t | matcher_t| only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
	auto body = rule | rule_t | body_t | only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
	return { matcher, body };
}

// creates a specific rule from a generic rule
// TODO (LOW) should depend in node_t instead of BAs...
template<typename... BAs>
rule<nso<BAs...>> make_rule(sp_tau_node<BAs...>& rule) {
	auto type = only_child_extractor<BAs...>(rule) | non_terminal_extractor<BAs...> | optional_value_extractor<size_t>;
	switch (type) {
	case tau_parser::bf_rule: return make_rule<BAs...>(tau_parser::bf_rule, tau_parser::bf_matcher, tau_parser::bf_body, rule);
	case tau_parser::wff_rule: return make_rule<BAs...>(tau_parser::wff_rule, tau_parser::wff_matcher, tau_parser::wff_body, rule);
	case tau_parser::tau_rule: return make_rule<BAs...>(tau_parser::tau_rule, tau_parser::tau_matcher, tau_parser::tau_body, rule);
	default: assert(false); return {};
	};
}

// creates a specific rule from a generic rule.
// TODO (LOW) should depend in node_t instead of BAs...
template<typename... BAs>
rec_relation<nso<BAs...>> make_rec_relation(tau_parser::nonterminal rule_t, tau_parser::nonterminal type_t, sp_tau_node<BAs...>& rule) {
	auto elements = rule | rule_t || type_t;
	return { elements[0], elements[1] };
}

// creates a specific rule from a generic rule.
template<typename... BAs>
rec_relation<nso<BAs...>> make_rec_relation(sp_tau_node<BAs...>& rule) {
	auto type = only_child_extractor<BAs...>(rule) | non_terminal_extractor<BAs...> | optional_value_extractor<size_t>;
	switch (type) {
	case tau_parser::bf_rec_relation: return make_rec_relation<BAs...>(tau_parser::bf_rec_relation, tau_parser::bf, rule);
	case tau_parser::wff_rec_relation: return make_rec_relation<BAs...>(tau_parser::wff_rec_relation, tau_parser::wff, rule);
	default: assert(false); return {};
	};
}

// create a set of rules from a given tau source.
// TODO (LOW) should depend in node_t instead of BAs...
template<typename... BAs>
rules<nso<BAs...>> make_rules(sp_tau_node<BAs...>& tau_source) {
	rules<nso<BAs...>> rs;
	// TODO (LOW) change call to select by operator|| and operator|
	for (auto& r: select_top(tau_source, is_non_terminal<tau_parser::rule, BAs...>))
		rs.push_back(make_rule<BAs...>(r));
	return rs;
}

// create a set of relations from a given tau source.
// TODO (LOW) should depend in node_t instead of BAs...
template<typename... BAs>
rec_relations<nso<BAs...>> make_rec_relations(sp_tau_node<BAs...>& tau_source) {
	rec_relations<nso<BAs...>> rs;
	// TODO (LOW) change call to select by operator|| and operator|
	for (auto& r: select_top(tau_source, is_non_terminal<tau_parser::nso_rec_relation, BAs...>))
		rs.push_back(make_rec_relation<BAs...>(r));
	return rs;
}

sp_tau_source_node clean_tau_source(const sp_tau_source_node& tau_source);

// make a tau source from the given source code string.
sp_tau_source_node make_tau_source(const std::string& source, idni::parser<>::parse_options options = {});

// make a tau source from the given source code stream.
sp_tau_source_node make_tau_source(std::istream& is);

// make a tau source from the given source code stream.
sp_tau_source_node make_tau_source_from_file(const std::string& filename);

template<typename...BAs>
sp_tau_node<BAs...> process_digits(sp_tau_node<BAs...>& tau_source){
	std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> changes;
	for(auto& n: select_top(tau_source, is_non_terminal<tau_parser::digits, BAs...>)){
		auto offset = make_string_with_skip<
				tau_node_terminal_extractor_t<BAs...>,
				not_whitespace_predicate_t<BAs...>,
				sp_tau_node<BAs...>>(
			tau_node_terminal_extractor<BAs...>,
			not_whitespace_predicate<BAs...>, n);
		auto num = std::stoul(offset);
		auto nn = make_node<tau_sym<BAs...>>(tau_sym<BAs...>(num), {});
		changes[n] = nn;
	}
	return replace<sp_tau_node<BAs...>>(tau_source, changes);
}

// create tau code from tau source
template<typename... BAs>
// TODO (LOW) should depend on node_t instead of BAs...
sp_tau_node<BAs...> make_tau_code(sp_tau_source_node& tau_source) {
	tauify<BAs...> tf;
	map_transformer<tauify<BAs...>, sp_tau_source_node, sp_tau_node<BAs...>> transform(tf);
	auto tau_code = post_order_traverser<
			map_transformer<tauify<BAs...>, sp_tau_source_node, sp_tau_node<BAs...>>,
			all_t<sp_tau_source_node>,
			sp_node<tau_source_sym>,
			sp_tau_node<BAs...>>(
		transform, all<sp_tau_source_node>)(tau_source);
	return process_digits(tau_code);
}

// make a library from the given tau source.
// TODO (LOW) should depend on node_t instead of BAs...
template<typename... BAs>
library<nso<BAs...>> make_library(sp_tau_source_node& tau_source) {
	auto lib = make_tau_code<BAs...>(tau_source);
	return make_rules(lib);
}

// make a library from the given tau source string.
// TODO (LOW) should depend on node_t instead of BAs...
template<typename... BAs>
library<nso<BAs...>> make_library(const std::string& source) {
	auto tau_source = make_tau_source(source);
	return make_library<BAs...>(tau_source);
}

// make a nso_rr from the given tau source and binder.
template<typename binder_t, typename... BAs>
sp_tau_node<BAs...> bind_tau_code_using_binder(const sp_tau_node<BAs...>& tau_source, binder_t& binder) {
	auto binded = post_order_traverser<
			binder_t,
			all_t<sp_tau_node<BAs...>>,
			sp_tau_node<BAs...>>(
		binder, all<sp_tau_node<BAs...>>)(tau_source);
	return binded;
}

// make a nso_rr from the given tau source and bindings.
template<typename... BAs>
sp_tau_node<BAs...> bind_tau_code_using_bindings(sp_tau_node<BAs...>& tau_code, const bindings<BAs...>& bindings) {
	name_binder<BAs...> nb(bindings);
	bind_transformer<name_binder<BAs...>, BAs...> bs(nb);
	return bind_tau_code_using_binder<bind_transformer<name_binder<BAs...>, BAs...>, BAs...>(tau_code, bs);
}

// make a nso_rr from the given tau source and bindings.
template<typename factory_t, typename... BAs>
sp_tau_node<BAs...> bind_tau_code_using_factory(const sp_tau_node<BAs...>& tau_code, factory_t& factory) {
	factory_binder<factory_t, BAs...> fb(factory);
	bind_transformer<factory_binder<factory_t, BAs...>, BAs...> bs(fb);
	return bind_tau_code_using_binder<bind_transformer<factory_binder<factory_t, BAs...>, BAs...>, BAs...>(tau_code, bs);
}

// make a nso_rr from the given tau source and binder.
template<typename binder_t, typename... BAs>
rr<nso<BAs...>> make_nso_rr_using_binder(const sp_tau_node<BAs...>& tau_code, binder_t& binder) {
	auto binded = bind_tau_code_using_binder<binder_t, BAs...>(tau_code, binder);
	auto main = binded | tau_parser::nso_rr | tau_parser::nso_main | tau_parser::wff | optional_value_extractor<sp_tau_node<BAs...>>;
	auto rules = make_rec_relations<BAs...>(binded);
	return { rules, main };
}

// make a nso_rr from the given tau source and binder.
template<typename binder_t, typename... BAs>
rr<nso<BAs...>> make_nso_rr_using_binder(sp_tau_source_node& tau_source, binder_t& binder) {
	auto tau_code = make_tau_code<BAs...>(tau_source);
	return make_nso_rr_using_binder<binder_t, BAs...>(tau_code, binder);
}

// make a nso_rr from the given tau source and bindings.
template<typename... BAs>
rr<nso<BAs...>> make_nso_rr_using_bindings(sp_tau_source_node& tau_source, const bindings<BAs...>& bindings) {
	name_binder<BAs...> nb(bindings);
	bind_transformer<name_binder<BAs...>, BAs...> bs(nb);
	return make_nso_rr_using_binder<bind_transformer<name_binder<BAs...>, BAs...>, BAs...>(tau_source, bs);
}


// make a nso_rr from the given tau source and bindings.
template<typename factory_t, typename... BAs>
rr<nso<BAs...>> make_nso_rr_using_factory(sp_tau_source_node& tau_source, factory_t& factory) {
	bind_transformer<factory_t, BAs...> bs(factory);
	return make_nso_rr_using_binder<bind_transformer<factory_t, BAs...>, BAs...>(tau_source, bs);
}

template<typename factory_t, typename... BAs>
rr<nso<BAs...>> make_nso_rr_using_factory(const sp_tau_node<BAs...>& tau_code, factory_t& factory) {
	bind_transformer<factory_t, BAs...> bs(factory);
	return make_nso_rr_using_binder<bind_transformer<factory_t, BAs...>, BAs...>(tau_code, bs);
}

// make a nso_rr from the given tau source and bindings.
template<typename factory_t, typename... BAs>
rr<nso<BAs...>> make_nso_rr_using_factory(const std::string& source, factory_t& factory) {
	auto tau_source = make_tau_source(source);
	return make_nso_rr_using_factory<factory_t, BAs...>(tau_source, factory);
}

// make a nso_rr from the given tau source and bindings.
template<typename... BAs>
rr<nso<BAs...>> make_nso_rr_using_bindings(const std::string& source, const bindings<BAs...>& bindings) {
	auto tau_source = make_tau_source(source);
	name_binder<BAs...> nb(bindings);
	bind_transformer<name_binder<BAs...>, BAs...> bs(nb);
	return make_nso_rr_using_bindings<
			bind_transformer<name_binder<BAs...>, BAs...>,
			BAs...>(
		tau_source, bs);
}

// creates a specific builder from a sp_tau_node.
template<typename... BAs>
builder<BAs...> make_builder(const sp_tau_node<BAs...>& builder) {
	auto head = builder | tau_parser::builder | tau_parser::builder_head | optional_value_extractor<sp_tau_node<BAs...>>;
	auto type_node = builder | tau_parser::builder | tau_parser::builder_body | only_child_extractor<BAs...>;
	auto type = type_node | non_terminal_extractor<BAs...> | optional_value_extractor<size_t>;
	switch (type) {
	case tau_parser::bf_builder_body: return { head, type_node | tau_parser::bf | optional_value_extractor<sp_tau_node<BAs...>>};
	case tau_parser::wff_builder_body: return { head, type_node | tau_parser::wff | optional_value_extractor<sp_tau_node<BAs...>>};
	case tau_parser::tau_builder_body: return { head, type_node | tau_parser::tau | optional_value_extractor<sp_tau_node<BAs...>>};
	default: throw std::runtime_error("unknown builder type");
	};
}

// create a builder from a given tau source.
template<typename... BAs>
builder<BAs...> make_builder(sp_tau_source_node& tau_source) {
	auto builder = make_tau_code<BAs...>(tau_source);
	return make_builder(builder);
}

// make a builder from the given tau source string.
template<typename... BAs>
builder<BAs...> make_builder(const std::string& source) {
	auto tau_source = make_tau_source(source);
	return make_builder<BAs...>(tau_source);
}

template<typename... BAs>
sp_tau_node<BAs...> tau_apply_builder(const builder<BAs...>& b, std::vector<sp_tau_node<BAs...>>& n) {
	std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> changes;
	std::vector<sp_tau_node<BAs...>> vars = b.first || tau_parser::capture;
	for (size_t i = 0; i < vars.size(); ++i) changes[vars[i]] = n[i];
	return replace<sp_tau_node<BAs...>>(b.second, changes);
}

template<typename... BAs>
sp_tau_node<BAs...> trim(const sp_tau_node<BAs...>& n) {
	return n->child[0];
}

template<typename... BAs>
sp_tau_node<BAs...> wrap(tau_parser::nonterminal t, const sp_tau_node<BAs...>& n) {
	auto nts = std::get<tau_source_sym>(n->value).nts;
	return make_node<tau_sym<BAs...>>(tau_sym<BAs...>(tau_source_sym(t, nts)), {n});
}

// definitions of basic bf and wff
const std::string BLDR_BF_0 = "( $X ) := 0.";
const std::string BLDR_BF_1 = "( $X ) := 1.";
const std::string BLDR_WFF_F = "( $X ) ::= F.";
const std::string BLDR_WFF_T = "( $X ) ::= T.";

// definitions of wff builder rules
const std::string BLDR_WFF_EQ = "( $X ) ::= ($X = 0).";
const std::string BLDR_WFF_NEQ = "( $X ) ::= ($X != 0).";
const std::string BLDR_BF_NOT_LESS_EQUAL = "( $X $Y ) ::= ($X !<= $Y).";
const std::string BLDR_WFF_AND = "( $X $Y ) ::= ($X && $Y).";
const std::string BLDR_WFF_OR = "( $X $Y ) ::= ($X || $Y).";
const std::string BLDR_WFF_NEG = "( $X ) ::= ! $X.";
const std::string BLDR_WFF_ALL = "( $X $Y ) ::= all $X $Y.";
const std::string BLDR_WFF_EX = "( $X $Y ) ::= ex $X $Y.";
const std::string BLDR_WFF_BALL = "( $X $Y ) ::= ball $X $Y.";
const std::string BLDR_WFF_BEX = "( $X $Y ) ::= bex $X $Y.";

// definitions of bf builder rules
const std::string BLDR_BF_AND = "( $X $Y ) := ($X & $Y).";
const std::string BLDR_BF_OR = "( $X $Y ) := ($X | $Y).";
const std::string BLDR_BF_NEG = "( $X ) := $X'.";
const std::string BLDR_BF_SPLITTER = "( $X ) := S($X).";
const std::string BLDR_BF_ALL = "( $X $Y ) := fall $X $Y.";
const std::string BLDR_BF_EX = "( $X $Y ) := fex $X $Y.";
const std::string BLDR_BF_CONSTANT = "( $X ) := { $X }.";

// definitions of tau builder rules
const std::string BLDR_TAU_AND = "( $X $Y ) :::= ($X &&& $Y).";
const std::string BLDR_TAU_OR = "( $X $Y ) :::= ($X ||| $Y).";
const std::string BLDR_TAU_NEG = "( $X ) :::= !!! $X.";

// basic bf and wff builders
template<typename... BAs>
static auto bldr_bf_0 = make_builder<BAs...>(BLDR_BF_0);
template<typename... BAs>
static auto bldr_bf_1 = make_builder<BAs...>(BLDR_BF_1);
template<typename... BAs>
static auto bldr_wff_F = make_builder<BAs...>(BLDR_WFF_F);
template<typename... BAs>
static auto bldr_wff_T = make_builder<BAs...>(BLDR_WFF_F);

// wff builder
template<typename... BAs>
static auto bldr_wff_eq = make_builder<BAs...>(BLDR_WFF_EQ);
template<typename... BAs>
static auto bldr_wff_neq = make_builder<BAs...>(BLDR_WFF_NEQ);
template<typename... BAs>
static auto bldr_wff_and = make_builder<BAs...>(BLDR_WFF_AND);
template<typename... BAs>
static auto bldr_wff_or = make_builder<BAs...>(BLDR_WFF_OR);
template<typename... BAs>
static auto bldr_wff_neg = make_builder<BAs...>(BLDR_WFF_NEG);
template<typename... BAs>
static auto bldr_wff_all = make_builder<BAs...>(BLDR_WFF_ALL);
template<typename... BAs>
static auto bldr_wff_ex = make_builder<BAs...>(BLDR_WFF_EX);
template<typename... BAs>
static auto bldr_wff_ball = make_builder<BAs...>(BLDR_WFF_BALL);
template<typename... BAs>
static auto bldr_wff_bex = make_builder<BAs...>(BLDR_WFF_BEX);

// bf builder
template<typename... BAs>
static auto bldr_bf_and = make_builder<BAs...>(BLDR_BF_AND);
template<typename... BAs>
static auto bldr_bf_or = make_builder<BAs...>(BLDR_BF_OR);
template<typename... BAs>
static auto bldr_bf_neg = make_builder<BAs...>(BLDR_BF_NEG);
template<typename... BAs>
static auto bldr_bf_splitter = make_builder<BAs...>(BLDR_BF_SPLITTER);
template<typename... BAs>
static auto bldr_bf_not_less_equal = make_builder<BAs...>(BLDR_BF_NOT_LESS_EQUAL);
template<typename... BAs>
static auto bldr_bf_all = make_builder<BAs...>(BLDR_BF_ALL);
template<typename... BAs>
static auto bldr_bf_ex = make_builder<BAs...>(BLDR_BF_EX);
template<typename... BAs>
static auto bldr_bf_constant = make_builder<BAs...>(BLDR_BF_CONSTANT);

// tau builder
template<typename... BAs>
static auto bldr_tau_and = make_builder<BAs...>(BLDR_TAU_AND);
template<typename... BAs>
static auto bldr_tau_or = make_builder<BAs...>(BLDR_TAU_OR);
template<typename... BAs>
static auto bldr_tau_neg = make_builder<BAs...>(BLDR_TAU_NEG);

// basic bf and wff constants
template<typename... BAs>
static const sp_tau_node<BAs...> _0 = bldr_bf_0<BAs...>.second;

template<typename... BAs>
static const sp_tau_node<BAs...> _1 = bldr_bf_1<BAs...>.second;

template<typename... BAs>
static const sp_tau_node<BAs...> _F = bldr_wff_F<BAs...>.second;

template<typename... BAs>
static const sp_tau_node<BAs...> _T = bldr_wff_T<BAs...>.second;

// wff factory method for building wff formulas
template<typename... BAs>
sp_tau_node<BAs...> build_wff_eq(const sp_tau_node<BAs...>& l) {
	auto tl = trim<BAs...>(l);
	std::vector<sp_tau_node<BAs...>> args {trim(l)} ;
	return tau_apply_builder<BAs...>(bldr_wff_eq<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_neq(const sp_tau_node<BAs...>& l) {
	std::vector<sp_tau_node<BAs...>> args {trim(l)} ;
	return tau_apply_builder<BAs...>(bldr_wff_neq<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_and(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {trim(l), trim(r)} ;
	return tau_apply_builder<BAs...>(bldr_wff_and<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_or(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {trim(l), trim(r)} ;
	return tau_apply_builder<BAs...>(bldr_wff_or<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_xor(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_wff_or<BAs...>(build_wff_and(build_wff_neg(l), r),
		build_wff_and(build_wff_neg(r), l));
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_neg(const sp_tau_node<BAs...>& l) {
	std::vector<sp_tau_node<BAs...>> args {trim(l)} ;
	return tau_apply_builder<BAs...>(bldr_wff_neg<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_imply(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_wff_or<BAs...>(build_wff_neg<BAs...>(l), r);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_conditional(const sp_tau_node<BAs...>& x, const sp_tau_node<BAs...>& y, const sp_tau_node<BAs...>& z) {
	return build_wff_and<BAs...>(build_wff_imply<BAs...>(x, y), build_wff_imply<BAs...>(build_wff_neg<BAs...>(x), z));
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_equiv(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_wff_and<BAs...>(build_wff_imply<BAs...>(l, r), build_wff_imply<BAs...>(r, l));
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_all(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {l, trim(r)} ;
	return tau_apply_builder<BAs...>(bldr_wff_all<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_ex(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {l, trim(r)} ;
	return tau_apply_builder<BAs...>(bldr_wff_ex<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_ball(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {l, trim(r)} ;
	return tau_apply_builder<BAs...>(bldr_wff_ball<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_wff_bex(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {l, trim(r)} ;
	return tau_apply_builder<BAs...>(bldr_wff_bex<BAs...>, args);
}

// bf factory method for building bf formulas
template<typename... BAs>
sp_tau_node<BAs...> build_bf_and(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {trim(l), trim(r)};
	return tau_apply_builder<BAs...>(bldr_bf_and<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_or(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {trim(l), trim(r)};
	return tau_apply_builder<BAs...>(bldr_bf_or<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_neg(const sp_tau_node<BAs...>& l) {
	std::vector<sp_tau_node<BAs...>> args {trim(l)};
	return tau_apply_builder<BAs...>(bldr_bf_neg<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_xor(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_bf_or<BAs...>(build_bf_and(build_bf_neg(l), r),
		build_bf_and(build_bf_neg(r), l));
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_less(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_bf_or<BAs...>(
		build_wff_eq<BAs...>(build_bf_and<BAs...>(l, build_bf_neg<BAs...>(r))),
		build_wff_neq(build_bf_xor<BAs...>(l, build_bf_neg<BAs...>(r))));
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_less_equal(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_wff_eq<BAs...>(build_bf_and<BAs...>(l, build_bf_neg<BAs...>(r)));
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_greater(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_bf_or<BAs...>(
		build_bf_or<BAs...>(
			build_wff_neq<BAs...>(l, build_bf_neg<BAs...>(r)),
			build_wff_eq<BAs...>(l, r)),
			build_bf_xor<BAs...>(l, build_bf_eq<BAs...>(l, r)));
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_all(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {l, trim(r)};
	return tau_apply_builder<BAs...>(bldr_bf_all<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_bf_ex(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args {l, trim(r)};
	return tau_apply_builder<BAs...>(bldr_bf_ex<BAs...>, args);
}

// tau factory method for building tau formulas
template<typename... BAs>
sp_tau_node<BAs...> build_tau_and(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args{trim(l), trim(r)};
	return tau_apply_builder<BAs...>(bldr_tau_and<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_tau_or(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	std::vector<sp_tau_node<BAs...>> args{trim(l), trim(r)};
	return tau_apply_builder<BAs...>(bldr_tau_or<BAs...>, args);
}

template<typename... BAs>
sp_tau_node<BAs...> build_tau_equiv(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_tau_and<BAs...>(build_tau_or(build_tau_neg(l), r), build_tau_or(build_tau_neg(r), l));
}

template<typename... BAs>
sp_tau_node<BAs...> build_tau_xor(const sp_tau_node<BAs...>& l, const sp_tau_node<BAs...>& r) {
	return build_tau_or<BAs...>(build_tau_and(build_tau_neg(l), r), build_tau_and(build_tau_neg(r), l));
}

template<typename... BAs>
sp_tau_node<BAs...> build_tau_neg(const sp_tau_node<BAs...>& l) {
	std::vector<sp_tau_node<BAs...>> args{trim(l)};
	return tau_apply_builder<BAs...>(bldr_tau_neg<BAs...>, args);
}


// IDEA convert to a const static applier and change all the code accordingly
//
// however, the logic is quite complez a lot of private functions doesn't make
// sense outside the actual structure.
template <typename... BAs>
struct callback_applier {

	sp_tau_node<BAs...> operator()(const sp_tau_node<BAs...>& n) {
		if (!is_callback<BAs...>(n)) return n;
		auto nt = get<tau_source_sym>(n->value).n();
		switch (nt) {
			case tau_parser::bf_neg_cb: return apply_unary_operation(_neg, n);
			case tau_parser::bf_and_cb: return apply_binary_operation(_and, n);
			case tau_parser::bf_or_cb: return apply_binary_operation(_or, n);
			case tau_parser::bf_xor_cb: return apply_binary_operation(_xor, n);
			case tau_parser::bf_eq_cb: return apply_equality_relation(_eq, n);
			case tau_parser::bf_neq_cb: return apply_equality_relation(_neq, n);
			case tau_parser::bf_is_one_cb: return apply_constant_check(_is_one, n);
			case tau_parser::bf_is_zero_cb: return apply_constant_check(_is_zero, n);
			case tau_parser::bf_has_subformula_cb: return apply_has_subformula_check(n, tau_parser::bf_cb_arg);
			case tau_parser::wff_has_clashing_subformulas_cb: return apply_wff_clashing_subformulas_check(n);
			case tau_parser::wff_has_subformula_cb: return apply_has_subformula_check(n, tau_parser::wff_cb_arg);
			case tau_parser::wff_remove_existential_cb: return apply_wff_remove_existential(n);
			case tau_parser::wff_remove_bexistential_cb: return apply_wff_remove_bexistential(n);
			case tau_parser::wff_remove_buniversal_cb: return apply_wff_remove_buniversal(n);
			case tau_parser::bf_remove_funiversal_cb: return apply_bf_remove_funiversal(n);
			case tau_parser::bf_remove_fexistential_cb: return apply_bf_remove_fexistential(n);
			case tau_parser::tau_collapse_positives_cb: return apply_tau_collapse_positives(n);
			case tau_parser::tau_positives_upwards_cb: return apply_tau_positives_upwards(n);
			default: return n;
		}
	}

private:
	// unary operation
	static constexpr auto _neg = [](const auto& l) -> sp_tau_node<BAs...> {
		auto res = ~l;
		std::variant<BAs...> v(res);
		return make_node<tau_sym<BAs...>>(tau_sym<BAs...>(v), {});
	};
	// binary operations
	static constexpr auto _and = overloaded([]<typename T>(const T& l, const T& r) -> sp_tau_node<BAs...> {
			auto res = l & r;
			std::variant<BAs...> v(res);
			return make_node<tau_sym<BAs...>>(tau_sym<BAs...>(v), {});
		}, [](const auto&, const auto&) -> sp_tau_node<BAs...> { throw std::logic_error("wrong types"); });
	static constexpr auto _or = overloaded([]<typename T>(const T& l, const T& r) -> sp_tau_node<BAs...> {
			auto res = l | r;
			std::variant<BAs...> v(res);
			return make_node<tau_sym<BAs...>>(tau_sym<BAs...>(v), {});
		}, [](const auto&, const auto&) -> sp_tau_node<BAs...> { throw std::logic_error("wrong types"); });
	static constexpr auto _xor = overloaded([]<typename T>(const T& l, const T& r) -> sp_tau_node<BAs...> {
			auto res = l | r;
			std::variant<BAs...> v(res);
			return make_node<tau_sym<BAs...>>(tau_sym<BAs...>(v), {});
		}, [](const auto&, const auto&) -> sp_tau_node<BAs...> { throw std::logic_error("wrong types"); });
	static constexpr auto _imply = overloaded([]<typename T>(const T& l, const T& r) -> sp_tau_node<BAs...> {
			auto res = ~l | r;
			std::variant<BAs...> v(res);
			return make_node<tau_sym<BAs...>>(tau_sym<BAs...>(v), {});
		}, [](const auto&, const auto&) -> sp_tau_node<BAs...> { throw std::logic_error("wrong types"); });
	static constexpr auto _equiv = overloaded([]<typename T>(const T& l, const T& r) -> sp_tau_node<BAs...> {
			auto res = (~l | r) & (~r | l);
			std::variant<BAs...> v(res);
			return make_node<tau_sym<BAs...>>(tau_sym<BAs...>(v), {});
		}, [](const auto&, const auto&) -> sp_tau_node<BAs...> { throw std::logic_error("wrong types"); });
	// ternary operators
	static constexpr auto _eq = [](const auto& l) -> bool { return l == false; };
	static constexpr auto _neq = [](const auto& l) -> bool { return !(l == false); };
	static constexpr auto _is_one = [](const auto& l) -> bool { return l == true; };
	static constexpr auto _is_zero = [](const auto& l) -> bool { return l == false; };

	// TODO (MEDIUM) unify this code with get_gssotc_clause and get_gssotc_literals
	void get_leaves(const sp_tau_node<BAs...>& n, tau_parser::nonterminal branch, tau_parser::nonterminal skip, std::vector<sp_tau_node<BAs...>>& leaves) {
		if (auto check = n | branch; check.has_value()) {
			for (auto& c: check || skip) get_leaves(c, branch, skip, leaves);
		} else {
			leaves.push_back(n);
			BOOST_LOG_TRIVIAL(trace) << "(I) found get_gssotc_clause: " << n;
		}
	}

	std::vector<sp_tau_node<BAs...>> get_leaves(const sp_tau_node<BAs...>& n, tau_parser::nonterminal branch, tau_parser::nonterminal skip) {
		std::vector<sp_tau_node<BAs...>> leaves;
		get_leaves(n, branch, skip, leaves);
		return leaves;
	}

	std::pair<sp_tau_node<BAs...>, sp_tau_node<BAs...>> get_quantifier_remove_constituents(const tau_parser::nonterminal type, const sp_tau_node<BAs...>& n) {
		auto args = n || type || only_child_extractor<BAs...>;
		std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> left_changes;
		left_changes[args[0] /* var */] = args[2] /* T */;
		auto left = replace<sp_tau_node<BAs...>>(args[1] /* formula */, left_changes);
		std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> right_changes;
		right_changes[args[0] /* var */] = args[3] /* F */;
		auto right = replace<sp_tau_node<BAs...>>(args[1] /* formula */, right_changes);
		return {left, right};
	}

	sp_tau_node<BAs...> apply_wff_remove_buniversal(const sp_tau_node<BAs...>& n) {
		auto [left, right] = get_quantifier_remove_constituents(tau_parser::wff_cb_arg, n);
		return build_wff_and<BAs...>(left, right);
	}

	sp_tau_node<BAs...> apply_wff_remove_bexistential(const sp_tau_node<BAs...>& n) {
		auto [left, right] = get_quantifier_remove_constituents(tau_parser::wff_cb_arg, n);
		return build_wff_or<BAs...>(left, right);
	}

	sp_tau_node<BAs...> apply_bf_remove_funiversal(const sp_tau_node<BAs...>& n) {
		auto [left, right] = get_quantifier_remove_constituents(tau_parser::bf_cb_arg, n);
		return build_bf_and<BAs...>(left, right);
	}

	sp_tau_node<BAs...> apply_bf_remove_fexistential(const sp_tau_node<BAs...>& n) {
		auto [left, right] = get_quantifier_remove_constituents(tau_parser::bf_cb_arg, n);
		return build_bf_or<BAs...>(left, right);
	}

	sp_tau_node<BAs...> apply_wff_remove_existential(const sp_tau_node<BAs...>& n) {
		auto args = n || tau_parser::wff_cb_arg || only_child_extractor<BAs...>;
		auto var = args[0] | only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
		auto wff = args[1];
		std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> wff_changes;
		for (auto& l: get_leaves(wff, tau_parser::wff_or, tau_parser::wff)) {
			std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> n_changes;
			auto check_eq = find_top(l, is_non_terminal<tau_parser::bf_eq, BAs...>);
			if (check_eq.has_value()) {
				auto f = check_eq
					| tau_parser::bf
					| optional_value_extractor<sp_tau_node<BAs...>>;
				auto fall = build_bf_all<BAs...>(var, f);
				wff_changes[check_eq.value()] = build_wff_eq<BAs...>(fall) | tau_parser::bf_eq | optional_value_extractor<sp_tau_node<BAs...>>;
				auto x_plus_fx = build_bf_xor<BAs...>(wrap(tau_parser::bf, var), f) | only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
				for (auto& neq: select_all(n, is_non_terminal<tau_parser::bf_neq, BAs...>)) {
					auto g_i = neq | tau_parser::bf	| optional_value_extractor<sp_tau_node<BAs...>>;
					std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> gi_changes;
					gi_changes[var] = x_plus_fx;
					auto ngi = replace<sp_tau_node<BAs...>>(g_i, gi_changes);
					auto fex = build_bf_ex<BAs...>(var, ngi);
					auto wff_neq = build_wff_neq<BAs...>(fex)| tau_parser::bf_neq | optional_value_extractor<sp_tau_node<BAs...>>;
					wff_changes[neq] = wff_neq;
				}
			} else {
				for (auto& neq: select_all(n, is_non_terminal<tau_parser::bf_neq, BAs...>)) {
					auto g_i = neq | tau_parser::bf	| optional_value_extractor<sp_tau_node<BAs...>>;
					auto fex = build_bf_ex<BAs...>(var, g_i);
					auto wff_neq = build_wff_neq<BAs...>(fex)| tau_parser::bf_neq | optional_value_extractor<sp_tau_node<BAs...>>;
					wff_changes[neq] = wff_neq;
					// wff_changes[neq] = build_wff_neq<BAs...>(fex)| tau_parser::bf_neq | optional_value_extractor<sp_tau_node<BAs...>>;
				}
			}
		}
		return replace<sp_tau_node<BAs...>>(wff, wff_changes);
	}

	// TODO (LOW) this should be promoted to a predicate if needed elsewhere
	bool are_equal(sp_tau_node<BAs...>& p, sp_tau_node<BAs...>& n) {
		if (p->value == n->value) {
			auto p_it = p->child.begin();
			auto n_it = n->child.begin();
			while (p_it != p->child.end() && n_it != n->child.end()) {
				if (is_non_essential<BAs...>(*p_it)) { ++p_it; continue; }
				if (is_non_essential<BAs...>(*n_it)) { ++n_it; continue; }
				if (*p_it == *n_it) { ++p_it; ++n_it; continue; }
				if (are_equal(*p_it, *n_it)) { ++p_it; ++n_it; continue; }
				return false;
			}
			return true;
		}
		return false;
	}

	sp_tau_node<BAs...> apply_has_subformula_check(sp_tau_node<BAs...> n, const tau_parser::nonterminal& cb_arg_t) {
		auto args = n || cb_arg_t || only_child_extractor<BAs...>;
		for (auto& subformula: select_all(args[0], all<sp_tau_node<BAs...>>))
			if (subformula == args[1]) return args[2];
		return args[0];
	}

	// TODO (LOW) make a cleaner implementation
	// we should merge the two following methods and split the logic in
	// meaningful methods
	sp_tau_node<BAs...> apply_wff_clashing_subformulas_check(const sp_tau_node<BAs...>& n) {
		auto args = n || tau_parser::wff_cb_arg || only_child_extractor<BAs...>;
		std::vector<sp_tau_node<BAs...>> positives, negatives;
		for (auto& op: select_all(args[0], all<sp_tau_node<BAs...>>))
			if (is_non_terminal<tau_parser::wff_and>(op))
				for (auto& c: op->child)
					if (auto check = c | tau_parser::wff_and; !check.has_value() && is_non_terminal<tau_parser::wff>(c)) {
						auto cc = c | only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
						(is_non_terminal<tau_parser::wff_neg>(cc) ? negatives : positives).push_back(cc);
					}
		for (auto& negation: negatives) {
			auto negated = negation | tau_parser::wff | only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
			for (auto& positive: positives)
				if (positive == negated)
					return args[1];
		}
		return args[0];
	}

	sp_tau_node<BAs...> apply_binary_operation(const auto& op, const sp_tau_node<BAs...>& n) {
		auto ba_elements = n || tau_parser::bf_cb_arg || tau_parser::bf || only_child_extractor<BAs...> || ba_extractor<BAs...>;
		sp_tau_node<BAs...> nn(std::visit(op, ba_elements[0], ba_elements[1]));
		std::vector<sp_tau_node<BAs...>> arg { nn };
		return tau_apply_builder(bldr_bf_constant<BAs...>, arg);
	}

	sp_tau_node<BAs...> apply_unary_operation(const auto& op, const sp_tau_node<BAs...>& n) {
		auto ba_elements = n || tau_parser::bf_cb_arg || tau_parser::bf || only_child_extractor<BAs...> || ba_extractor<BAs...>;
		sp_tau_node<BAs...> nn(std::visit(op, ba_elements[0]));
		std::vector<sp_tau_node<BAs...>> arg { nn };
		return tau_apply_builder(bldr_bf_constant<BAs...>, arg);
	}

	sp_tau_node<BAs...> apply_equality_relation(const auto& op, const sp_tau_node<BAs...>& n) {
		auto bf_arg = n | tau_parser::bf_cb_arg | tau_parser::bf | only_child_extractor<BAs...>;
		auto wff_args = n || tau_parser::wff_cb_arg || only_child_extractor<BAs...>;
		auto ba_element = bf_arg | ba_extractor<BAs...> | optional_value_extractor<std::variant<BAs...>>;
		return std::visit(op, ba_element) ? wff_args[0] : wff_args[1];
	}

	sp_tau_node<BAs...> apply_constant_check(const auto& op, const sp_tau_node<BAs...>& n) {
		auto args = n || tau_parser::bf_cb_arg || only_child_extractor<BAs...>;
		auto ba_element = args[0] | tau_parser::bf_constant | tau_parser::constant
			| only_child_extractor<BAs...> |  ba_extractor<BAs...> | optional_value_extractor<std::variant<BAs...>>;
		return std::visit(op, ba_element) ? args[1] : args[0];
	}

	sp_tau_node<BAs...> apply_subs(const sp_tau_node<BAs...>& n) {
		auto args = n || tau_parser::bf_cb_arg || only_child_extractor<BAs...>;
		std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> m;
		m[args[0]] = args[1];
		auto tmp = replace<sp_tau_node<BAs...>>(args[2], m)| only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
		return tmp;
	}

	sp_tau_node<BAs...> apply_tau_positives_upwards(const sp_tau_node<BAs...>& n) {
		auto args = n || tau_parser::tau_cb_arg || only_child_extractor<BAs...>;
		if (auto check = args[0] | tau_parser::wff; check) return args[1];
		return n;
	}

	sp_tau_node<BAs...> apply_tau_collapse_positives(const sp_tau_node<BAs...>& n) {
		auto args = n || tau_parser::tau_cb_arg || only_child_extractor<BAs...>;
		if (auto check_1eft = args[0] | tau_parser::wff; check_1eft)
			if (auto check_right = args[1] | tau_parser::wff; check_right) {
				auto wff = build_wff_and<BAs...>(check_1eft.value(), check_right.value());
				auto tau = make_node<tau_sym<BAs...>>(tau_parser::tau, { wff });
				if (args.size() == 2) return tau;
				auto arg3 = args[2] | only_child_extractor<BAs...> | optional_value_extractor<sp_tau_node<BAs...>>;
				return build_tau_and<BAs...>(tau, arg3);
			}
		return n;
	}
};

// apply one tau rule to the given expression
// IDEA maybe this could be operator|
template<typename predicate_t, typename... BAs>
sp_tau_node<BAs...> nso_rr_apply_if(const rule<nso<BAs...>>& r, const sp_tau_node<BAs...>& n, predicate_t& predicate) {
	// IDEA maybe we could traverse only once
	auto nn = apply_with_skip_if<
			sp_tau_node<BAs...>,
			none_t<sp_tau_node<BAs...>>,
			is_capture_t<BAs...>,
			is_non_essential_t<BAs...>,
			predicate_t>(
		r, n , none<sp_tau_node<BAs...>>, is_capture<BAs...>, is_non_essential<BAs...>, predicate);
	if (auto cbs = select_all(nn, is_callback<BAs...>); !cbs.empty()) {
		callback_applier<BAs...> cb_applier;
		std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> changes;
		for (auto& cb : cbs) {
			auto nnn = cb_applier(cb);
			changes[cb] = nnn;
		}
		auto cnn = replace<sp_tau_node<BAs...>>(nn, changes);

		BOOST_LOG_TRIVIAL(debug) << "(C) " << cnn;

		return cnn;
	}
	return nn;
}

// apply the given rules to the given expression
// IDEA maybe this could be operator|
template<typename predicate_t, typename... BAs>
sp_tau_node<BAs...> nso_rr_apply_if(const rules<nso<BAs...>>& rs, const sp_tau_node<BAs...>& n, predicate_t& predicate) {
	if (rs.empty()) return n;
	sp_tau_node<BAs...> nn = n;
	for (auto& r : rs) {
		auto nnn = nso_rr_apply_if<predicate_t, BAs...>(r, nn, predicate);
		while (nnn != nn) {
			nn = nnn;
			nnn = nso_rr_apply_if<predicate_t, BAs...>(r, nn, predicate);
		}
	}
	return nn;
}

// apply one tau rule to the given expression
// IDEA maybe this could be operator|
template<typename... BAs>
sp_tau_node<BAs...> nso_rr_apply(const rule<nso<BAs...>>& r, const sp_tau_node<BAs...>& n) {
	// IDEA maybe we could traverse only once

	// apply the rule
	auto nn = apply_with_skip<
			sp_tau_node<BAs...>,
			none_t<sp_tau_node<BAs...>>,
			is_capture_t<BAs...>,
			is_non_essential_t<BAs...>>(
		r, n , none<sp_tau_node<BAs...>>, is_capture<BAs...>, is_non_essential<BAs...>);

	std::map<sp_tau_node<BAs...>, sp_tau_node<BAs...>> changes;

	// compute changes from callbacks
	if (auto cbs = select_all(nn, is_callback<BAs...>); !cbs.empty()) {
		callback_applier<BAs...> cb_applier;
		for (auto& cb : cbs) {
			auto nnn = cb_applier(cb);
			changes[cb] = nnn;
		}
	}

	// apply numerical simplifications
	auto pred = is_non_terminal<BAs...>(tau_parser::shift);
	if (auto shifts = select_all(nn, pred); !shifts.empty()) {
		for (auto& shift : shifts) {
			auto args = shift || tau_parser::num;
			if (args.size() == 2) {
				auto left = args[0] | only_child_extractor<BAs...> | offset_extractor<BAs...> | optional_value_extractor<size_t>;
				auto right = args[1] | only_child_extractor<BAs...> | offset_extractor<BAs...> | optional_value_extractor<size_t>;
				if (left < right) {
					BOOST_LOG_TRIVIAL(debug) << "(C) " << n;
					return n;
				}
				auto nts = std::get<tau_source_sym>(nn->value).nts;
				auto digits = make_node<tau_sym<BAs...>>(tau_sym<BAs...>(left-right), {});
				auto new_num = make_node<tau_sym<BAs...>>( tau_sym<BAs...>(tau_source_sym(tau_parser::num, nts)), {digits});
				changes[shift] = new_num;
			}
		}
	}

	// apply the changes and print info
	if (!changes.empty()) {
		auto cnn = replace<sp_tau_node<BAs...>>(nn, changes);
		BOOST_LOG_TRIVIAL(debug) << "(C) " << cnn;
		return cnn;
	}

	return nn;
}

// apply the given rules to the given expression
// IDEA maybe this could be operator|
template<typename... BAs>
sp_tau_node<BAs...> nso_rr_apply(const rules<nso<BAs...>>& rs, const sp_tau_node<BAs...>& n) {
	if (rs.empty()) return n;
	sp_tau_node<BAs...> nn = n;
	for (auto& r : rs) nn = nso_rr_apply<BAs...>(r, nn);
	return nn;
}

} // namespace idni::tau

//
// operators << to pretty print the tau language related types
//

// << for rules
template <typename... BAs>
std::ostream& operator<<(std::ostream& stream, const idni::tau::rules<idni::tau::nso<BAs...>>& rs) {
	for (const auto& r : rs) stream << r << "\n";
	return stream;
}

// << for tau_source_sym
std::ostream& operator<<(std::ostream& stream, const idni::tau::tau_source_sym& rs);

// << for BAs... variant
template <typename... BAs>
std::ostream& operator<<(std::ostream& os, const std::variant<BAs...>& rs) {
	std::visit(idni::tau::overloaded {
		[&os](const auto& a) { os << a; }
	}, rs);
	return os;
}

// << for tau_sym
template <typename... BAs>
std::ostream& operator<<(std::ostream& stream, const idni::tau::tau_sym<BAs...>& rs) {
	// using tau_sym = std::variant<tau_source_sym, std::variant<BAs...>, size_t>;
	std::visit(idni::tau::overloaded {
		[&stream](const idni::tau::tau_source_sym& t) {
			if (!t.nt() && !t.is_null()) stream << t.t();
		},
		[&stream](const auto& a) { stream << a; }
	}, rs);
	return stream;
}

// << for formulas
template <typename... BAs>
std::ostream& operator<<(std::ostream& stream, const idni::tau::rr<idni::tau::nso<BAs...>>& f) {
	return stream << f.rec_relations << f.main << "\n";
}

// << for bindings
// TODO (HIGH) << for bindings depends on << for variant<BAs...>
// TODO (HIGH) << for bindings needs to follow tau lang syntax
template <typename... BAs>
std::ostream& operator<<(std::ostream& stream, const idni::tau::bindings<BAs...>& bs) {
	for (const auto& b : bs) stream << b.first << ": " << b.second << "\n";
	return stream;
}

// outputs a sp_tau_node<...> to a stream, using the stringify transformer
// and assumes that the constants also override operator<<.
//
// IDEA maybe it should be move to out.h
template <typename... BAs>
std::ostream& operator<<(std::ostream& stream, const idni::tau::sp_tau_node<BAs...>& n){
	stream << n->value;
	for (const auto& c : n->child) stream << c;
	return stream;
}

// outputs a sp_tau_source_node to a stream, using the stringify transformer
// and assumes that the constants also override operator<<.
//
// IDEA maybe it should be move to out.h
std::ostream& operator<<(std::ostream& stream, const idni::tau::sp_tau_source_node& n);

// << tau_source_node (make it shared to make use of the previous operator)
std::ostream& operator<<(std::ostream& stream, const idni::tau::tau_source_node& n);

#endif // __NSO_RR_H__