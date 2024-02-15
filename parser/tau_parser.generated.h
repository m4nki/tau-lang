// This file is generated from a file parser/tau.tgf by
//       https://github.com/IDNI/parser/tools/tgf
//
#ifndef __TAU_PARSER_H__
#define __TAU_PARSER_H__
#include <string.h>
#include "parser.h"
struct tau_parser {
	using char_type     = char;
	using terminal_type = char;
	using traits_type   = std::char_traits<char_type>;
	using int_type      = typename traits_type::int_type;
	using symbol_type   = idni::lit<char_type, terminal_type>;
	using location_type = std::array<size_t, 2>;
	using node_type     = std::pair<symbol_type, location_type>;
	using parser_type   = idni::parser<char_type, terminal_type>;
	using options       = parser_type::options;
	using parse_options = parser_type::parse_options;
	using forest_type   = parser_type::pforest;
	using input_type    = parser_type::input;
	using decoder_type  = parser_type::input::decoder_type;
	using encoder_type  = std::function<std::basic_string<char_type>(
			const std::vector<terminal_type>&)>;
	tau_parser() :
		nts(load_nonterminals()), cc(load_cc()),
		g(nts, load_prods(), nt(244), cc), p(g, load_opts()) {}
	std::unique_ptr<forest_type> parse(const char_type* data, size_t size=0,
		parse_options po = {}) { return p.parse(data, size, po); }
	std::unique_ptr<forest_type> parse(std::basic_istream<char_type>& is,
		parse_options po = {}) { return p.parse(is, po); }
	std::unique_ptr<forest_type> parse(std::string fn, mmap_mode m,
		parse_options po = {}) { return p.parse(fn, m, po); }
#ifndef WIN32
	std::unique_ptr<forest_type> parse(int fd, parse_options po = {})
		{ return p.parse(fd, po); }
#endif //WIN32
	bool found(int start = -1) { return p.found(start); }
	typename parser_type::error get_error() { return p.get_error(); }
	enum nonterminal {
		nul, eof, space, digit, xdigit, alpha, alnum, punct, printable, eol, 
		ws_comment, _Rws_comment_0, _Rws_comment_1, ws_required, ws, hex_escape, unicode_escape, char_escape_encode, esc, q_char, 
		q_str, q_bqstr, char_punct, _Rchar_punct_2, _Rchar_punct_3, _Rchar_punct_4, char0, char_, string_char, bqstring_char, 
		chars, _Rchars_5, _Rchars_6, digits, _Rdigits_7, bf_def, wff_def, tau_def, dot, open_parenthesis, 
		close_parenthesis, open_bracket, close_bracket, open_brace, close_brace, minus, colon, semicolon, less, comma, 
		sym, offsets, offset, _Roffsets_8, _Roffsets_9, num, capture, shift, variable, var, 
		io_var, pos_var, in, out, in_var_name, out_var_name, bool_variable, capture_var, tau_rule, tau_matcher, 
		tau_body, tau, tau_collapse_positives_cb, tau_positives_upwards_cb, tau_rec_relation, tau_and, tau_neg, tau_or, tau_wff, _Rtau_rec_relation_10, 
		tau_ref, tau_ref_args, _Rtau_ref_args_11, _Rtau_ref_args_12, _Rtau_ref_args_13, tau_and_sym, tau_or_sym, tau_neg_sym, wff, wff_rule, 
		wff_matcher, wff_body, bf_eq_cb, bf_neq_cb, wff_has_clashing_subformulas_cb, wff_has_subformula_cb, wff_remove_existential_cb, wff_remove_bexistential_cb, wff_remove_buniversal_cb, wff_rec_relation, 
		wff_and, wff_neg, wff_xor, wff_conditional, wff_or, wff_all, wff_ex, wff_imply, wff_equiv, wff_t, 
		wff_f, wff_ball, wff_bex, bf_eq, bf_neq, bf_less, bf_less_equal, bf_greater, _Rwff_rec_relation_14, wff_ref, 
		wff_ref_args, _Rwff_ref_args_15, _Rwff_ref_args_16, _Rwff_ref_args_17, wff_and_sym, wff_or_sym, wff_xor_sym, wff_conditional_sym, wff_neg_sym, wff_imply_sym, 
		wff_equiv_sym, wff_all_sym, _Rwff_all_18, wff_ex_sym, _Rwff_ex_19, wff_ball_sym, _Rwff_ball_20, wff_bex_sym, _Rwff_bex_21, bf, 
		bf_equality_sym, bf_nequality_sym, bf_less_sym, bf_less_equal_sym, bf_greater_sym, bf_rule, bf_matcher, bf_body, bf_is_zero_cb, bf_is_one_cb, 
		bf_has_clashing_subformulas_cb, bf_has_subformula_cb, bf_remove_funiversal_cb, bf_remove_fexistential_cb, bf_and_cb, bf_or_cb, bf_xor_cb, bf_neg_cb, bf_ref, bf_constant, 
		bf_and, bf_neg, bf_xor, bf_or, bf_all, bf_ex, bf_t, bf_f, bf_rec_relation, _Rbf_rec_relation_22, 
		bf_ref_args, _Rbf_ref_args_23, _Rbf_ref_args_24, _Rbf_ref_args_25, bf_and_sym, bf_or_sym, bf_xor_sym, bf_neg_sym, bf_all_sym, _Rbf_all_26, 
		bf_ex_sym, _Rbf_ex_27, constant, binding, source_binding, named_binding, type, source, source0, _Rsource_28, 
		_Rsource_29, bf_and_cb_sym, bf_cb_arg, bf_or_cb_sym, bf_xor_cb_sym, bf_neg_cb_sym, bf_eq_cb_sym, wff_cb_arg, bf_neq_cb_sym, bf_is_zero_cb_sym, 
		bf_is_one_cb_sym, bf_remove_funiversal_cb_sym, bf_remove_fexistential_cb_sym, wff_remove_existential_cb_sym, wff_remove_bexistential_cb_sym, wff_remove_buniversal_cb_sym, bf_has_clashing_subformulas_cb_sym, wff_has_clashing_subformulas_cb_sym, bf_has_subformula_cb_sym, wff_has_subformula_cb_sym, 
		tau_collapse_positives_cb_sym, tau_cb_arg, _Rtau_collapse_positives_cb_30, _Rtau_collapse_positives_cb_31, tau_positives_upwards_cb_sym, inputs, input, _Rinputs_32, _Rinputs_33, builder, 
		builder_head, builder_body, _Rbuilder_head_34, _Rbuilder_head_35, bf_builder_body, wff_builder_body, tau_builder_body, library, rules, rule, 
		_Rrules_36, _Rrules_37, nso_rr, nso_rec_relations, nso_main, nso_rec_relation, _Rnso_rec_relations_38, _Rnso_rec_relations_39, gssotc_rr, gssotc_rec_relations, 
		gssotc_main, gssotc_rec_relation, _Rgssotc_rec_relations_40, _Rgssotc_rec_relations_41, start, _Rstart_42, __neg_0, __neg_1, __neg_2, __neg_3, 
		__neg_4, __neg_5, __neg_6, __neg_7, __neg_8, __neg_9, __neg_10, __neg_11, __neg_12, __neg_13, 
		__neg_14, __neg_15, __neg_16, __neg_17, __neg_18, __neg_19, __neg_20, __neg_21, __neg_22, __neg_23, 
		__neg_24, __neg_25, __neg_26, __neg_27, __neg_28, __neg_29, __neg_30, __neg_31, __neg_32, __neg_33, 
		__neg_34, __neg_35, __neg_36, __neg_37, __neg_38, __neg_39, __neg_40, __neg_41, __neg_42, __neg_43, 
	};
	size_t id(const std::basic_string<char_type>& name) {
		return nts.get(name);
	}
private:
	std::vector<terminal_type> ts{
		'\0', '\n', '\r', '\t', '#', '\\', 'x', 'u', '\'', 
		'"', '`', ':', '=', '.', '(', ')', '[', ']', '{', 
		'}', '-', ';', '<', ',', '?', '$', 'i', '_', 'o', 
		'&', '|', '!', '^', '>', 'a', 'l', 'e', 'b', 'T', 
		'F', '+', '~', 'f', '1', '0', 'h', 's', 'c', 'n', 
		'g', 'r', 'm', 'w', 'v', 't', 'd', 'q', 'z', 'p', 
	};
	idni::nonterminals<char_type, terminal_type> nts{};
	idni::char_class_fns<terminal_type> cc;
	idni::grammar<char_type, terminal_type> g;
	parser_type p;
	idni::prods<char_type, terminal_type> t(size_t tid) {
		return idni::prods<char_type, terminal_type>(ts[tid]);
	}
	idni::prods<char_type, terminal_type> nt(size_t ntid) {
		return idni::prods<char_type, terminal_type>(
			symbol_type(ntid, &nts));
	}
	idni::nonterminals<char_type, terminal_type> load_nonterminals() const {
		idni::nonterminals<char_type, terminal_type> nts{};
		for (const auto& nt : {
			"", "eof", "space", "digit", "xdigit", "alpha", "alnum", "punct", "printable", "eol", 
			"ws_comment", "_Rws_comment_0", "_Rws_comment_1", "ws_required", "ws", "hex_escape", "unicode_escape", "char_escape_encode", "esc", "q_char", 
			"q_str", "q_bqstr", "char_punct", "_Rchar_punct_2", "_Rchar_punct_3", "_Rchar_punct_4", "char0", "char_", "string_char", "bqstring_char", 
			"chars", "_Rchars_5", "_Rchars_6", "digits", "_Rdigits_7", "bf_def", "wff_def", "tau_def", "dot", "open_parenthesis", 
			"close_parenthesis", "open_bracket", "close_bracket", "open_brace", "close_brace", "minus", "colon", "semicolon", "less", "comma", 
			"sym", "offsets", "offset", "_Roffsets_8", "_Roffsets_9", "num", "capture", "shift", "variable", "var", 
			"io_var", "pos_var", "in", "out", "in_var_name", "out_var_name", "bool_variable", "capture_var", "tau_rule", "tau_matcher", 
			"tau_body", "tau", "tau_collapse_positives_cb", "tau_positives_upwards_cb", "tau_rec_relation", "tau_and", "tau_neg", "tau_or", "tau_wff", "_Rtau_rec_relation_10", 
			"tau_ref", "tau_ref_args", "_Rtau_ref_args_11", "_Rtau_ref_args_12", "_Rtau_ref_args_13", "tau_and_sym", "tau_or_sym", "tau_neg_sym", "wff", "wff_rule", 
			"wff_matcher", "wff_body", "bf_eq_cb", "bf_neq_cb", "wff_has_clashing_subformulas_cb", "wff_has_subformula_cb", "wff_remove_existential_cb", "wff_remove_bexistential_cb", "wff_remove_buniversal_cb", "wff_rec_relation", 
			"wff_and", "wff_neg", "wff_xor", "wff_conditional", "wff_or", "wff_all", "wff_ex", "wff_imply", "wff_equiv", "wff_t", 
			"wff_f", "wff_ball", "wff_bex", "bf_eq", "bf_neq", "bf_less", "bf_less_equal", "bf_greater", "_Rwff_rec_relation_14", "wff_ref", 
			"wff_ref_args", "_Rwff_ref_args_15", "_Rwff_ref_args_16", "_Rwff_ref_args_17", "wff_and_sym", "wff_or_sym", "wff_xor_sym", "wff_conditional_sym", "wff_neg_sym", "wff_imply_sym", 
			"wff_equiv_sym", "wff_all_sym", "_Rwff_all_18", "wff_ex_sym", "_Rwff_ex_19", "wff_ball_sym", "_Rwff_ball_20", "wff_bex_sym", "_Rwff_bex_21", "bf", 
			"bf_equality_sym", "bf_nequality_sym", "bf_less_sym", "bf_less_equal_sym", "bf_greater_sym", "bf_rule", "bf_matcher", "bf_body", "bf_is_zero_cb", "bf_is_one_cb", 
			"bf_has_clashing_subformulas_cb", "bf_has_subformula_cb", "bf_remove_funiversal_cb", "bf_remove_fexistential_cb", "bf_and_cb", "bf_or_cb", "bf_xor_cb", "bf_neg_cb", "bf_ref", "bf_constant", 
			"bf_and", "bf_neg", "bf_xor", "bf_or", "bf_all", "bf_ex", "bf_t", "bf_f", "bf_rec_relation", "_Rbf_rec_relation_22", 
			"bf_ref_args", "_Rbf_ref_args_23", "_Rbf_ref_args_24", "_Rbf_ref_args_25", "bf_and_sym", "bf_or_sym", "bf_xor_sym", "bf_neg_sym", "bf_all_sym", "_Rbf_all_26", 
			"bf_ex_sym", "_Rbf_ex_27", "constant", "binding", "source_binding", "named_binding", "type", "source", "source0", "_Rsource_28", 
			"_Rsource_29", "bf_and_cb_sym", "bf_cb_arg", "bf_or_cb_sym", "bf_xor_cb_sym", "bf_neg_cb_sym", "bf_eq_cb_sym", "wff_cb_arg", "bf_neq_cb_sym", "bf_is_zero_cb_sym", 
			"bf_is_one_cb_sym", "bf_remove_funiversal_cb_sym", "bf_remove_fexistential_cb_sym", "wff_remove_existential_cb_sym", "wff_remove_bexistential_cb_sym", "wff_remove_buniversal_cb_sym", "bf_has_clashing_subformulas_cb_sym", "wff_has_clashing_subformulas_cb_sym", "bf_has_subformula_cb_sym", "wff_has_subformula_cb_sym", 
			"tau_collapse_positives_cb_sym", "tau_cb_arg", "_Rtau_collapse_positives_cb_30", "_Rtau_collapse_positives_cb_31", "tau_positives_upwards_cb_sym", "inputs", "input", "_Rinputs_32", "_Rinputs_33", "builder", 
			"builder_head", "builder_body", "_Rbuilder_head_34", "_Rbuilder_head_35", "bf_builder_body", "wff_builder_body", "tau_builder_body", "library", "rules", "rule", 
			"_Rrules_36", "_Rrules_37", "nso_rr", "nso_rec_relations", "nso_main", "nso_rec_relation", "_Rnso_rec_relations_38", "_Rnso_rec_relations_39", "gssotc_rr", "gssotc_rec_relations", 
			"gssotc_main", "gssotc_rec_relation", "_Rgssotc_rec_relations_40", "_Rgssotc_rec_relations_41", "start", "_Rstart_42", "__neg_0", "__neg_1", "__neg_2", "__neg_3", 
			"__neg_4", "__neg_5", "__neg_6", "__neg_7", "__neg_8", "__neg_9", "__neg_10", "__neg_11", "__neg_12", "__neg_13", 
			"__neg_14", "__neg_15", "__neg_16", "__neg_17", "__neg_18", "__neg_19", "__neg_20", "__neg_21", "__neg_22", "__neg_23", 
			"__neg_24", "__neg_25", "__neg_26", "__neg_27", "__neg_28", "__neg_29", "__neg_30", "__neg_31", "__neg_32", "__neg_33", 
			"__neg_34", "__neg_35", "__neg_36", "__neg_37", "__neg_38", "__neg_39", "__neg_40", "__neg_41", "__neg_42", "__neg_43", 
		}) nts.get(nt);
		return nts;
	}
	idni::char_class_fns<terminal_type> load_cc() {
		return idni::predefined_char_classes<char_type, terminal_type>({
			"eof",
			"space",
			"digit",
			"xdigit",
			"alpha",
			"alnum",
			"punct",
			"printable",
		}, nts);
	}
	options load_opts() {
		options o;
		return o;
	}
	idni::prods<char_type, terminal_type> load_prods() {
		idni::prods<char_type, terminal_type> q,
			nul(symbol_type{});
		// eol => '\n'.
		q(nt(9), (t(1)));
		// eol => '\r'.
		q(nt(9), (t(2)));
		// eol => eof.
		q(nt(9), (nt(1)));
		// _Rws_comment_0 => '\t'.
		q(nt(11), (t(3)));
		// _Rws_comment_0 => printable.
		q(nt(11), (nt(8)));
		// _Rws_comment_1 => null.
		q(nt(12), (nul));
		// _Rws_comment_1 => _Rws_comment_0 _Rws_comment_1.
		q(nt(12), (nt(11)+nt(12)));
		// ws_comment => '#' _Rws_comment_1 eol.
		q(nt(10), (t(4)+nt(12)+nt(9)));
		// ws_required => space ws.
		q(nt(13), (nt(2)+nt(14)));
		// ws_required => ws_comment ws.
		q(nt(13), (nt(10)+nt(14)));
		// ws => null.
		q(nt(14), (nul));
		// ws => ws_required.
		q(nt(14), (nt(13)));
		// hex_escape => '\\' '\\' 'x' xdigit xdigit.
		q(nt(15), (t(5)+t(5)+t(6)+nt(4)+nt(4)));
		// unicode_escape => '\\' '\\' 'u' xdigit xdigit xdigit xdigit.
		q(nt(16), (t(5)+t(5)+t(7)+nt(4)+nt(4)+nt(4)+nt(4)));
		// char_escape_encode => hex_escape.
		q(nt(17), (nt(15)));
		// char_escape_encode => unicode_escape.
		q(nt(17), (nt(16)));
		// esc => '\\' '\\' '\\' '\\'.
		q(nt(18), (t(5)+t(5)+t(5)+t(5)));
		// q_char => '\''.
		q(nt(19), (t(8)));
		// q_str => '"'.
		q(nt(20), (t(9)));
		// q_bqstr => '`'.
		q(nt(21), (t(10)));
		// _Rchar_punct_2 => esc q_char.
		q(nt(23), (nt(18)+nt(19)));
		// _Rchar_punct_3 => esc q_str.
		q(nt(24), (nt(18)+nt(20)));
		// _Rchar_punct_4 => esc q_bqstr.
		q(nt(25), (nt(18)+nt(21)));
		// __neg_0 => q_char.
		q(nt(246), (nt(19)));
		// __neg_1 => q_str.
		q(nt(247), (nt(20)));
		// __neg_2 => q_bqstr.
		q(nt(248), (nt(21)));
		// __neg_3 => _Rchar_punct_2.
		q(nt(249), (nt(23)));
		// __neg_4 => _Rchar_punct_3.
		q(nt(250), (nt(24)));
		// __neg_5 => _Rchar_punct_4.
		q(nt(251), (nt(25)));
		// char_punct => punct & ~( __neg_0 ) & ~( __neg_1 ) & ~( __neg_2 ) & ~( __neg_3 ) & ~( __neg_4 ) & ~( __neg_5 ).
		q(nt(22), (nt(7)) & ~(nt(246)) & ~(nt(247)) & ~(nt(248)) & ~(nt(249)) & ~(nt(250)) & ~(nt(251)));
		// char0 => space.
		q(nt(26), (nt(2)));
		// char0 => alnum.
		q(nt(26), (nt(6)));
		// char0 => char_escape_encode.
		q(nt(26), (nt(17)));
		// char0 => char_punct.
		q(nt(26), (nt(22)));
		// char_ => esc q_char.
		q(nt(27), (nt(18)+nt(19)));
		// char_ => q_str.
		q(nt(27), (nt(20)));
		// char_ => q_bqstr.
		q(nt(27), (nt(21)));
		// char_ => char0.
		q(nt(27), (nt(26)));
		// string_char => esc q_str.
		q(nt(28), (nt(18)+nt(20)));
		// string_char => q_char.
		q(nt(28), (nt(19)));
		// string_char => q_bqstr.
		q(nt(28), (nt(21)));
		// string_char => char0.
		q(nt(28), (nt(26)));
		// bqstring_char => esc q_bqstr.
		q(nt(29), (nt(18)+nt(21)));
		// bqstring_char => q_char.
		q(nt(29), (nt(19)));
		// bqstring_char => q_str.
		q(nt(29), (nt(20)));
		// bqstring_char => char0.
		q(nt(29), (nt(26)));
		// _Rchars_5 => alnum.
		q(nt(31), (nt(6)));
		// _Rchars_6 => null.
		q(nt(32), (nul));
		// _Rchars_6 => _Rchars_5 _Rchars_6.
		q(nt(32), (nt(31)+nt(32)));
		// chars => alpha _Rchars_6.
		q(nt(30), (nt(5)+nt(32)));
		// _Rdigits_7 => digit.
		q(nt(34), (nt(3)));
		// _Rdigits_7 => digit _Rdigits_7.
		q(nt(34), (nt(3)+nt(34)));
		// digits => _Rdigits_7.
		q(nt(33), (nt(34)));
		// bf_def => ':' '='.
		q(nt(35), (t(11)+t(12)));
		// wff_def => ':' ':' '='.
		q(nt(36), (t(11)+t(11)+t(12)));
		// tau_def => ':' ':' ':' '='.
		q(nt(37), (t(11)+t(11)+t(11)+t(12)));
		// dot => '.'.
		q(nt(38), (t(13)));
		// open_parenthesis => '('.
		q(nt(39), (t(14)));
		// close_parenthesis => ')'.
		q(nt(40), (t(15)));
		// open_bracket => '['.
		q(nt(41), (t(16)));
		// close_bracket => ']'.
		q(nt(42), (t(17)));
		// open_brace => '{'.
		q(nt(43), (t(18)));
		// close_brace => '}'.
		q(nt(44), (t(19)));
		// minus => '-'.
		q(nt(45), (t(20)));
		// colon => ':'.
		q(nt(46), (t(11)));
		// semicolon => ';'.
		q(nt(47), (t(21)));
		// less => '<'.
		q(nt(48), (t(22)));
		// comma => ','.
		q(nt(49), (t(23)));
		// sym => chars.
		q(nt(50), (nt(30)));
		// _Roffsets_8 => ws comma ws offset.
		q(nt(53), (nt(14)+nt(49)+nt(14)+nt(52)));
		// _Roffsets_9 => null.
		q(nt(54), (nul));
		// _Roffsets_9 => _Roffsets_8 _Roffsets_9.
		q(nt(54), (nt(53)+nt(54)));
		// offsets => open_bracket ws offset _Roffsets_9 ws close_bracket.
		q(nt(51), (nt(41)+nt(14)+nt(52)+nt(54)+nt(14)+nt(42)));
		// offset => num.
		q(nt(52), (nt(55)));
		// offset => capture.
		q(nt(52), (nt(56)));
		// offset => shift.
		q(nt(52), (nt(57)));
		// shift => capture ws minus ws num.
		q(nt(57), (nt(56)+nt(14)+nt(45)+nt(14)+nt(55)));
		// num => digits.
		q(nt(55), (nt(33)));
		// variable => var.
		q(nt(58), (nt(59)));
		// variable => io_var.
		q(nt(58), (nt(60)));
		// variable => pos_var.
		q(nt(58), (nt(61)));
		// io_var => in.
		q(nt(60), (nt(62)));
		// io_var => out.
		q(nt(60), (nt(63)));
		// in => in_var_name open_bracket offset close_bracket.
		q(nt(62), (nt(64)+nt(41)+nt(52)+nt(42)));
		// out => out_var_name open_bracket offset close_bracket.
		q(nt(63), (nt(65)+nt(41)+nt(52)+nt(42)));
		// bool_variable => '?' chars.
		q(nt(66), (t(24)+nt(30)));
		// capture => capture_var.
		q(nt(56), (nt(67)));
		// capture_var => '$' chars.
		q(nt(67), (t(25)+nt(30)));
		// var => chars.
		q(nt(59), (nt(30)));
		// in_var_name => 'i' '_' chars.
		q(nt(64), (t(26)+t(27)+nt(30)));
		// out_var_name => 'o' '_' chars.
		q(nt(65), (t(28)+t(27)+nt(30)));
		// tau_rule => tau_matcher ws tau_def ws tau_body ws dot.
		q(nt(68), (nt(69)+nt(14)+nt(37)+nt(14)+nt(70)+nt(14)+nt(38)));
		// tau_matcher => tau.
		q(nt(69), (nt(71)));
		// tau_body => tau.
		q(nt(70), (nt(71)));
		// tau_body => tau_collapse_positives_cb.
		q(nt(70), (nt(72)));
		// tau_body => tau_positives_upwards_cb.
		q(nt(70), (nt(73)));
		// __neg_6 => capture.
		q(nt(252), (nt(56)));
		// __neg_7 => tau_and.
		q(nt(253), (nt(75)));
		// __neg_8 => tau_neg.
		q(nt(254), (nt(76)));
		// __neg_9 => tau_or.
		q(nt(255), (nt(77)));
		// __neg_10 => tau_wff.
		q(nt(256), (nt(78)));
		// _Rtau_rec_relation_10 => ~( __neg_6 ) & tau & ~( __neg_7 ) & ~( __neg_8 ) & ~( __neg_9 ) & ~( __neg_10 ).
		q(nt(79), ~(nt(252)) & (nt(71)) & ~(nt(253)) & ~(nt(254)) & ~(nt(255)) & ~(nt(256)));
		// tau_rec_relation => _Rtau_rec_relation_10 ws tau_def ws tau ws dot.
		q(nt(74), (nt(79)+nt(14)+nt(37)+nt(14)+nt(71)+nt(14)+nt(38)));
		// tau_ref => sym ws offsets ws tau_ref_args.
		q(nt(80), (nt(50)+nt(14)+nt(51)+nt(14)+nt(81)));
		// _Rtau_ref_args_11 => capture.
		q(nt(82), (nt(56)));
		// _Rtau_ref_args_11 => variable.
		q(nt(82), (nt(58)));
		// _Rtau_ref_args_12 => _Rtau_ref_args_11.
		q(nt(83), (nt(82)));
		// _Rtau_ref_args_13 => null.
		q(nt(84), (nul));
		// _Rtau_ref_args_13 => _Rtau_ref_args_12 _Rtau_ref_args_13.
		q(nt(84), (nt(83)+nt(84)));
		// tau_ref_args => open_parenthesis ws _Rtau_ref_args_13 ws close_parenthesis.
		q(nt(81), (nt(39)+nt(14)+nt(84)+nt(14)+nt(40)));
		// tau => capture.
		q(nt(71), (nt(56)));
		// tau => tau_and.
		q(nt(71), (nt(75)));
		// tau => tau_neg.
		q(nt(71), (nt(76)));
		// tau => tau_or.
		q(nt(71), (nt(77)));
		// tau => tau_wff.
		q(nt(71), (nt(78)));
		// tau => tau_ref.
		q(nt(71), (nt(80)));
		// tau_and => open_parenthesis ws tau ws tau_and_sym ws tau ws close_parenthesis.
		q(nt(75), (nt(39)+nt(14)+nt(71)+nt(14)+nt(85)+nt(14)+nt(71)+nt(14)+nt(40)));
		// tau_or => open_parenthesis ws tau ws tau_or_sym ws tau ws close_parenthesis.
		q(nt(77), (nt(39)+nt(14)+nt(71)+nt(14)+nt(86)+nt(14)+nt(71)+nt(14)+nt(40)));
		// tau_neg => tau_neg_sym ws tau.
		q(nt(76), (nt(87)+nt(14)+nt(71)));
		// tau_wff => open_brace ws wff ws close_brace.
		q(nt(78), (nt(43)+nt(14)+nt(88)+nt(14)+nt(44)));
		// tau_and_sym => '&' '&' '&'.
		q(nt(85), (t(29)+t(29)+t(29)));
		// tau_or_sym => '|' '|' '|'.
		q(nt(86), (t(30)+t(30)+t(30)));
		// tau_neg_sym => '!' '!' '!'.
		q(nt(87), (t(31)+t(31)+t(31)));
		// wff_rule => wff_matcher ws wff_def ws wff_body ws dot.
		q(nt(89), (nt(90)+nt(14)+nt(36)+nt(14)+nt(91)+nt(14)+nt(38)));
		// wff_matcher => wff.
		q(nt(90), (nt(88)));
		// wff_body => wff.
		q(nt(91), (nt(88)));
		// wff_body => bf_eq_cb.
		q(nt(91), (nt(92)));
		// wff_body => bf_neq_cb.
		q(nt(91), (nt(93)));
		// wff_body => wff_has_clashing_subformulas_cb.
		q(nt(91), (nt(94)));
		// wff_body => wff_has_subformula_cb.
		q(nt(91), (nt(95)));
		// wff_body => wff_remove_existential_cb.
		q(nt(91), (nt(96)));
		// wff_body => wff_remove_bexistential_cb.
		q(nt(91), (nt(97)));
		// wff_body => wff_remove_buniversal_cb.
		q(nt(91), (nt(98)));
		// __neg_11 => capture.
		q(nt(257), (nt(56)));
		// __neg_12 => bool_variable.
		q(nt(258), (nt(66)));
		// __neg_13 => wff_and.
		q(nt(259), (nt(100)));
		// __neg_14 => wff_neg.
		q(nt(260), (nt(101)));
		// __neg_15 => wff_xor.
		q(nt(261), (nt(102)));
		// __neg_16 => wff_conditional.
		q(nt(262), (nt(103)));
		// __neg_17 => wff_or.
		q(nt(263), (nt(104)));
		// __neg_18 => wff_all.
		q(nt(264), (nt(105)));
		// __neg_19 => wff_ex.
		q(nt(265), (nt(106)));
		// __neg_20 => wff_imply.
		q(nt(266), (nt(107)));
		// __neg_21 => wff_equiv.
		q(nt(267), (nt(108)));
		// __neg_22 => wff_t.
		q(nt(268), (nt(109)));
		// __neg_23 => wff_f.
		q(nt(269), (nt(110)));
		// __neg_24 => wff_ball.
		q(nt(270), (nt(111)));
		// __neg_25 => wff_bex.
		q(nt(271), (nt(112)));
		// __neg_26 => bf_eq.
		q(nt(272), (nt(113)));
		// __neg_27 => bf_neq.
		q(nt(273), (nt(114)));
		// __neg_28 => bf_less.
		q(nt(274), (nt(115)));
		// __neg_29 => bf_less_equal.
		q(nt(275), (nt(116)));
		// __neg_30 => bf_greater.
		q(nt(276), (nt(117)));
		// _Rwff_rec_relation_14 => ~( __neg_11 ) & ~( __neg_12 ) & wff & ~( __neg_13 ) & ~( __neg_14 ) & ~( __neg_15 ) & ~( __neg_16 ) & ~( __neg_17 ) & ~( __neg_18 ) & ~( __neg_19 ) & ~( __neg_20 ) & ~( __neg_21 ) & ~( __neg_22 ) & ~( __neg_23 ) & ~( __neg_24 ) & ~( __neg_25 ) & ~( __neg_26 ) & ~( __neg_27 ) & ~( __neg_28 ) & ~( __neg_29 ) & ~( __neg_30 ).
		q(nt(118), ~(nt(257)) & ~(nt(258)) & (nt(88)) & ~(nt(259)) & ~(nt(260)) & ~(nt(261)) & ~(nt(262)) & ~(nt(263)) & ~(nt(264)) & ~(nt(265)) & ~(nt(266)) & ~(nt(267)) & ~(nt(268)) & ~(nt(269)) & ~(nt(270)) & ~(nt(271)) & ~(nt(272)) & ~(nt(273)) & ~(nt(274)) & ~(nt(275)) & ~(nt(276)));
		// wff_rec_relation => _Rwff_rec_relation_14 ws wff_def ws wff ws dot.
		q(nt(99), (nt(118)+nt(14)+nt(36)+nt(14)+nt(88)+nt(14)+nt(38)));
		// wff => capture.
		q(nt(88), (nt(56)));
		// wff => bool_variable.
		q(nt(88), (nt(66)));
		// wff => wff_and.
		q(nt(88), (nt(100)));
		// wff => wff_neg.
		q(nt(88), (nt(101)));
		// wff => wff_xor.
		q(nt(88), (nt(102)));
		// wff => wff_conditional.
		q(nt(88), (nt(103)));
		// wff => wff_or.
		q(nt(88), (nt(104)));
		// wff => wff_all.
		q(nt(88), (nt(105)));
		// wff => wff_ex.
		q(nt(88), (nt(106)));
		// wff => wff_imply.
		q(nt(88), (nt(107)));
		// wff => wff_equiv.
		q(nt(88), (nt(108)));
		// wff => wff_t.
		q(nt(88), (nt(109)));
		// wff => wff_f.
		q(nt(88), (nt(110)));
		// wff => wff_ball.
		q(nt(88), (nt(111)));
		// wff => wff_bex.
		q(nt(88), (nt(112)));
		// wff => bf_eq.
		q(nt(88), (nt(113)));
		// wff => bf_neq.
		q(nt(88), (nt(114)));
		// wff => bf_less.
		q(nt(88), (nt(115)));
		// wff => bf_less_equal.
		q(nt(88), (nt(116)));
		// wff => bf_greater.
		q(nt(88), (nt(117)));
		// wff => wff_ref.
		q(nt(88), (nt(119)));
		// wff_ref => sym ws offsets ws wff_ref_args.
		q(nt(119), (nt(50)+nt(14)+nt(51)+nt(14)+nt(120)));
		// _Rwff_ref_args_15 => capture.
		q(nt(121), (nt(56)));
		// _Rwff_ref_args_15 => variable.
		q(nt(121), (nt(58)));
		// _Rwff_ref_args_16 => _Rwff_ref_args_15.
		q(nt(122), (nt(121)));
		// _Rwff_ref_args_17 => null.
		q(nt(123), (nul));
		// _Rwff_ref_args_17 => _Rwff_ref_args_16 _Rwff_ref_args_17.
		q(nt(123), (nt(122)+nt(123)));
		// wff_ref_args => open_parenthesis ws _Rwff_ref_args_17 ws close_parenthesis.
		q(nt(120), (nt(39)+nt(14)+nt(123)+nt(14)+nt(40)));
		// wff_and => open_parenthesis ws wff ws wff_and_sym ws wff ws close_parenthesis.
		q(nt(100), (nt(39)+nt(14)+nt(88)+nt(14)+nt(124)+nt(14)+nt(88)+nt(14)+nt(40)));
		// wff_or => open_parenthesis ws wff ws wff_or_sym ws wff ws close_parenthesis.
		q(nt(104), (nt(39)+nt(14)+nt(88)+nt(14)+nt(125)+nt(14)+nt(88)+nt(14)+nt(40)));
		// wff_xor => open_parenthesis ws wff ws wff_xor_sym ws wff ws close_parenthesis.
		q(nt(102), (nt(39)+nt(14)+nt(88)+nt(14)+nt(126)+nt(14)+nt(88)+nt(14)+nt(40)));
		// wff_conditional => open_parenthesis ws wff ws wff_conditional_sym ws wff ws colon ws wff ws close_parenthesis.
		q(nt(103), (nt(39)+nt(14)+nt(88)+nt(14)+nt(127)+nt(14)+nt(88)+nt(14)+nt(46)+nt(14)+nt(88)+nt(14)+nt(40)));
		// wff_neg => wff_neg_sym ws wff.
		q(nt(101), (nt(128)+nt(14)+nt(88)));
		// wff_imply => open_parenthesis ws wff ws wff_imply_sym ws wff ws close_parenthesis.
		q(nt(107), (nt(39)+nt(14)+nt(88)+nt(14)+nt(129)+nt(14)+nt(88)+nt(14)+nt(40)));
		// wff_equiv => open_parenthesis ws wff ws wff_equiv_sym ws wff ws close_parenthesis.
		q(nt(108), (nt(39)+nt(14)+nt(88)+nt(14)+nt(130)+nt(14)+nt(88)+nt(14)+nt(40)));
		// _Rwff_all_18 => capture.
		q(nt(132), (nt(56)));
		// _Rwff_all_18 => variable.
		q(nt(132), (nt(58)));
		// wff_all => wff_all_sym ws_required _Rwff_all_18 ws_required wff.
		q(nt(105), (nt(131)+nt(13)+nt(132)+nt(13)+nt(88)));
		// _Rwff_ex_19 => capture.
		q(nt(134), (nt(56)));
		// _Rwff_ex_19 => variable.
		q(nt(134), (nt(58)));
		// wff_ex => wff_ex_sym ws_required _Rwff_ex_19 ws_required wff.
		q(nt(106), (nt(133)+nt(13)+nt(134)+nt(13)+nt(88)));
		// _Rwff_ball_20 => capture.
		q(nt(136), (nt(56)));
		// _Rwff_ball_20 => bool_variable.
		q(nt(136), (nt(66)));
		// wff_ball => wff_ball_sym ws_required _Rwff_ball_20 ws_required wff.
		q(nt(111), (nt(135)+nt(13)+nt(136)+nt(13)+nt(88)));
		// _Rwff_bex_21 => capture.
		q(nt(138), (nt(56)));
		// _Rwff_bex_21 => bool_variable.
		q(nt(138), (nt(66)));
		// wff_bex => wff_bex_sym ws_required _Rwff_bex_21 ws_required wff.
		q(nt(112), (nt(137)+nt(13)+nt(138)+nt(13)+nt(88)));
		// bf_eq => open_parenthesis ws bf ws bf_equality_sym ws bf ws close_parenthesis.
		q(nt(113), (nt(39)+nt(14)+nt(139)+nt(14)+nt(140)+nt(14)+nt(139)+nt(14)+nt(40)));
		// bf_neq => open_parenthesis ws bf ws bf_nequality_sym ws bf ws close_parenthesis.
		q(nt(114), (nt(39)+nt(14)+nt(139)+nt(14)+nt(141)+nt(14)+nt(139)+nt(14)+nt(40)));
		// bf_less => open_parenthesis ws bf ws bf_less_sym ws bf ws close_parenthesis.
		q(nt(115), (nt(39)+nt(14)+nt(139)+nt(14)+nt(142)+nt(14)+nt(139)+nt(14)+nt(40)));
		// bf_less_equal => open_parenthesis ws bf ws bf_less_equal_sym ws bf ws close_parenthesis.
		q(nt(116), (nt(39)+nt(14)+nt(139)+nt(14)+nt(143)+nt(14)+nt(139)+nt(14)+nt(40)));
		// bf_greater => open_parenthesis ws bf ws bf_greater_sym ws bf ws close_parenthesis.
		q(nt(117), (nt(39)+nt(14)+nt(139)+nt(14)+nt(144)+nt(14)+nt(139)+nt(14)+nt(40)));
		// wff_and_sym => '&' '&'.
		q(nt(124), (t(29)+t(29)));
		// wff_or_sym => '|' '|'.
		q(nt(125), (t(30)+t(30)));
		// wff_xor_sym => '^'.
		q(nt(126), (t(32)));
		// wff_conditional_sym => '?'.
		q(nt(127), (t(24)));
		// wff_neg_sym => '!'.
		q(nt(128), (t(31)));
		// wff_imply_sym => '-' '>'.
		q(nt(129), (t(20)+t(33)));
		// wff_equiv_sym => '<' '-' '>'.
		q(nt(130), (t(22)+t(20)+t(33)));
		// wff_all_sym => 'a' 'l' 'l'.
		q(nt(131), (t(34)+t(35)+t(35)));
		// wff_ex_sym => 'e' 'x'.
		q(nt(133), (t(36)+t(6)));
		// wff_ball_sym => 'b' 'a' 'l' 'l'.
		q(nt(135), (t(37)+t(34)+t(35)+t(35)));
		// wff_bex_sym => 'b' 'e' 'x'.
		q(nt(137), (t(37)+t(36)+t(6)));
		// wff_t => 'T'.
		q(nt(109), (t(38)));
		// wff_f => 'F'.
		q(nt(110), (t(39)));
		// bf_rule => bf_matcher ws bf_def ws bf_body ws dot.
		q(nt(145), (nt(146)+nt(14)+nt(35)+nt(14)+nt(147)+nt(14)+nt(38)));
		// bf_matcher => bf.
		q(nt(146), (nt(139)));
		// bf_body => bf.
		q(nt(147), (nt(139)));
		// bf_body => bf_is_zero_cb.
		q(nt(147), (nt(148)));
		// bf_body => bf_is_one_cb.
		q(nt(147), (nt(149)));
		// bf_body => bf_has_clashing_subformulas_cb.
		q(nt(147), (nt(150)));
		// bf_body => bf_has_subformula_cb.
		q(nt(147), (nt(151)));
		// bf_body => bf_remove_funiversal_cb.
		q(nt(147), (nt(152)));
		// bf_body => bf_remove_fexistential_cb.
		q(nt(147), (nt(153)));
		// bf_body => bf_and_cb.
		q(nt(147), (nt(154)));
		// bf_body => bf_or_cb.
		q(nt(147), (nt(155)));
		// bf_body => bf_xor_cb.
		q(nt(147), (nt(156)));
		// bf_body => bf_neg_cb.
		q(nt(147), (nt(157)));
		// bf => capture.
		q(nt(139), (nt(56)));
		// bf => variable.
		q(nt(139), (nt(58)));
		// bf => bf_ref.
		q(nt(139), (nt(158)));
		// bf => bf_constant.
		q(nt(139), (nt(159)));
		// bf => bf_and.
		q(nt(139), (nt(160)));
		// bf => bf_neg.
		q(nt(139), (nt(161)));
		// bf => bf_xor.
		q(nt(139), (nt(162)));
		// bf => bf_or.
		q(nt(139), (nt(163)));
		// bf => bf_all.
		q(nt(139), (nt(164)));
		// bf => bf_ex.
		q(nt(139), (nt(165)));
		// bf => bf_t.
		q(nt(139), (nt(166)));
		// bf => bf_f.
		q(nt(139), (nt(167)));
		// __neg_31 => capture.
		q(nt(277), (nt(56)));
		// __neg_32 => variable.
		q(nt(278), (nt(58)));
		// __neg_33 => bf_eq.
		q(nt(279), (nt(113)));
		// __neg_34 => bf_neq.
		q(nt(280), (nt(114)));
		// __neg_35 => bf_constant.
		q(nt(281), (nt(159)));
		// __neg_36 => bf_and.
		q(nt(282), (nt(160)));
		// __neg_37 => bf_neg.
		q(nt(283), (nt(161)));
		// __neg_38 => bf_xor.
		q(nt(284), (nt(162)));
		// __neg_39 => bf_or.
		q(nt(285), (nt(163)));
		// __neg_40 => bf_all.
		q(nt(286), (nt(164)));
		// __neg_41 => bf_ex.
		q(nt(287), (nt(165)));
		// __neg_42 => bf_t.
		q(nt(288), (nt(166)));
		// __neg_43 => bf_f.
		q(nt(289), (nt(167)));
		// _Rbf_rec_relation_22 => ~( __neg_31 ) & ~( __neg_32 ) & ~( __neg_33 ) & ~( __neg_34 ) & bf & ~( __neg_35 ) & ~( __neg_36 ) & ~( __neg_37 ) & ~( __neg_38 ) & ~( __neg_39 ) & ~( __neg_40 ) & ~( __neg_41 ) & ~( __neg_42 ) & ~( __neg_43 ).
		q(nt(169), ~(nt(277)) & ~(nt(278)) & ~(nt(279)) & ~(nt(280)) & (nt(139)) & ~(nt(281)) & ~(nt(282)) & ~(nt(283)) & ~(nt(284)) & ~(nt(285)) & ~(nt(286)) & ~(nt(287)) & ~(nt(288)) & ~(nt(289)));
		// bf_rec_relation => _Rbf_rec_relation_22 ws bf_def ws bf ws dot.
		q(nt(168), (nt(169)+nt(14)+nt(35)+nt(14)+nt(139)+nt(14)+nt(38)));
		// bf_ref => sym offsets bf_ref_args.
		q(nt(158), (nt(50)+nt(51)+nt(170)));
		// _Rbf_ref_args_23 => capture.
		q(nt(171), (nt(56)));
		// _Rbf_ref_args_23 => variable.
		q(nt(171), (nt(58)));
		// _Rbf_ref_args_24 => _Rbf_ref_args_23.
		q(nt(172), (nt(171)));
		// _Rbf_ref_args_25 => null.
		q(nt(173), (nul));
		// _Rbf_ref_args_25 => _Rbf_ref_args_24 _Rbf_ref_args_25.
		q(nt(173), (nt(172)+nt(173)));
		// bf_ref_args => open_parenthesis ws _Rbf_ref_args_25 ws close_parenthesis.
		q(nt(170), (nt(39)+nt(14)+nt(173)+nt(14)+nt(40)));
		// bf_and => open_parenthesis ws bf ws bf_and_sym ws bf ws close_parenthesis.
		q(nt(160), (nt(39)+nt(14)+nt(139)+nt(14)+nt(174)+nt(14)+nt(139)+nt(14)+nt(40)));
		// bf_or => open_parenthesis ws bf ws bf_or_sym ws bf ws close_parenthesis.
		q(nt(163), (nt(39)+nt(14)+nt(139)+nt(14)+nt(175)+nt(14)+nt(139)+nt(14)+nt(40)));
		// bf_xor => open_parenthesis ws bf ws bf_xor_sym ws bf ws close_parenthesis.
		q(nt(162), (nt(39)+nt(14)+nt(139)+nt(14)+nt(176)+nt(14)+nt(139)+nt(14)+nt(40)));
		// bf_neg => bf_neg_sym ws bf.
		q(nt(161), (nt(177)+nt(14)+nt(139)));
		// _Rbf_all_26 => capture.
		q(nt(179), (nt(56)));
		// _Rbf_all_26 => variable.
		q(nt(179), (nt(58)));
		// bf_all => bf_all_sym ws_required _Rbf_all_26 ws_required bf.
		q(nt(164), (nt(178)+nt(13)+nt(179)+nt(13)+nt(139)));
		// _Rbf_ex_27 => capture.
		q(nt(181), (nt(56)));
		// _Rbf_ex_27 => variable.
		q(nt(181), (nt(58)));
		// bf_ex => bf_ex_sym ws_required _Rbf_ex_27 ws_required bf.
		q(nt(165), (nt(180)+nt(13)+nt(181)+nt(13)+nt(139)));
		// bf_and_sym => '&'.
		q(nt(174), (t(29)));
		// bf_or_sym => '|'.
		q(nt(175), (t(30)));
		// bf_xor_sym => '+'.
		q(nt(176), (t(40)));
		// bf_neg_sym => '~'.
		q(nt(177), (t(41)));
		// bf_equality_sym => '='.
		q(nt(140), (t(12)));
		// bf_nequality_sym => '!' '='.
		q(nt(141), (t(31)+t(12)));
		// bf_less_sym => '<'.
		q(nt(142), (t(22)));
		// bf_less_equal_sym => '<' '='.
		q(nt(143), (t(22)+t(12)));
		// bf_greater_sym => '>'.
		q(nt(144), (t(33)));
		// bf_all_sym => 'f' 'a' 'l' 'l'.
		q(nt(178), (t(42)+t(34)+t(35)+t(35)));
		// bf_ex_sym => 'f' 'e' 'x'.
		q(nt(180), (t(42)+t(36)+t(6)));
		// bf_t => '1'.
		q(nt(166), (t(43)));
		// bf_f => '0'.
		q(nt(167), (t(44)));
		// bf_constant => open_brace ws constant ws close_brace.
		q(nt(159), (nt(43)+nt(14)+nt(182)+nt(14)+nt(44)));
		// constant => capture.
		q(nt(182), (nt(56)));
		// constant => binding.
		q(nt(182), (nt(183)));
		// binding => source_binding.
		q(nt(183), (nt(184)));
		// binding => named_binding.
		q(nt(183), (nt(185)));
		// named_binding => chars.
		q(nt(185), (nt(30)));
		// source_binding => type ws colon ws source.
		q(nt(184), (nt(186)+nt(14)+nt(46)+nt(14)+nt(187)));
		// type => null.
		q(nt(186), (nul));
		// type => chars.
		q(nt(186), (nt(30)));
		// source0 => space.
		q(nt(188), (nt(2)));
		// source0 => alnum.
		q(nt(188), (nt(6)));
		// source0 => punct.
		q(nt(188), (nt(7)));
		// _Rsource_28 => source0.
		q(nt(189), (nt(188)));
		// _Rsource_29 => _Rsource_28.
		q(nt(190), (nt(189)));
		// _Rsource_29 => _Rsource_28 _Rsource_29.
		q(nt(190), (nt(189)+nt(190)));
		// source => _Rsource_29.
		q(nt(187), (nt(190)));
		// bf_and_cb => bf_and_cb_sym ws_required bf_cb_arg ws_required bf_cb_arg.
		q(nt(154), (nt(191)+nt(13)+nt(192)+nt(13)+nt(192)));
		// bf_or_cb => bf_or_cb_sym ws_required bf_cb_arg ws_required bf_cb_arg.
		q(nt(155), (nt(193)+nt(13)+nt(192)+nt(13)+nt(192)));
		// bf_xor_cb => bf_xor_cb_sym ws_required bf_cb_arg ws_required bf_cb_arg.
		q(nt(156), (nt(194)+nt(13)+nt(192)+nt(13)+nt(192)));
		// bf_neg_cb => bf_neg_cb_sym ws_required bf_cb_arg.
		q(nt(157), (nt(195)+nt(13)+nt(192)));
		// bf_eq_cb => bf_eq_cb_sym ws bf_cb_arg ws_required wff_cb_arg ws_required wff_cb_arg.
		q(nt(92), (nt(196)+nt(14)+nt(192)+nt(13)+nt(197)+nt(13)+nt(197)));
		// bf_neq_cb => bf_neq_cb_sym ws bf_cb_arg ws_required wff_cb_arg ws_required wff_cb_arg.
		q(nt(93), (nt(198)+nt(14)+nt(192)+nt(13)+nt(197)+nt(13)+nt(197)));
		// bf_is_zero_cb => bf_is_zero_cb_sym ws bf_cb_arg ws_required bf_cb_arg.
		q(nt(148), (nt(199)+nt(14)+nt(192)+nt(13)+nt(192)));
		// bf_is_one_cb => bf_is_one_cb_sym ws bf_cb_arg ws_required bf_cb_arg.
		q(nt(149), (nt(200)+nt(14)+nt(192)+nt(13)+nt(192)));
		// bf_remove_funiversal_cb => bf_remove_funiversal_cb_sym ws bf_cb_arg ws_required bf_cb_arg ws_required bf_cb_arg ws_required bf_cb_arg.
		q(nt(152), (nt(201)+nt(14)+nt(192)+nt(13)+nt(192)+nt(13)+nt(192)+nt(13)+nt(192)));
		// bf_remove_fexistential_cb => bf_remove_fexistential_cb_sym ws bf_cb_arg ws_required bf_cb_arg ws_required bf_cb_arg ws_required bf_cb_arg.
		q(nt(153), (nt(202)+nt(14)+nt(192)+nt(13)+nt(192)+nt(13)+nt(192)+nt(13)+nt(192)));
		// wff_remove_existential_cb => wff_remove_existential_cb_sym ws wff_cb_arg ws_required wff_cb_arg.
		q(nt(96), (nt(203)+nt(14)+nt(197)+nt(13)+nt(197)));
		// wff_remove_bexistential_cb => wff_remove_bexistential_cb_sym ws wff_cb_arg ws_required wff_cb_arg ws_required wff_cb_arg ws_required wff_cb_arg.
		q(nt(97), (nt(204)+nt(14)+nt(197)+nt(13)+nt(197)+nt(13)+nt(197)+nt(13)+nt(197)));
		// wff_remove_buniversal_cb => wff_remove_buniversal_cb_sym ws wff_cb_arg ws_required wff_cb_arg ws_required wff_cb_arg ws_required wff_cb_arg.
		q(nt(98), (nt(205)+nt(14)+nt(197)+nt(13)+nt(197)+nt(13)+nt(197)+nt(13)+nt(197)));
		// bf_has_clashing_subformulas_cb => bf_has_clashing_subformulas_cb_sym ws bf_cb_arg ws_required bf_cb_arg.
		q(nt(150), (nt(206)+nt(14)+nt(192)+nt(13)+nt(192)));
		// wff_has_clashing_subformulas_cb => wff_has_clashing_subformulas_cb_sym ws wff_cb_arg ws_required wff_cb_arg.
		q(nt(94), (nt(207)+nt(14)+nt(197)+nt(13)+nt(197)));
		// bf_has_subformula_cb => bf_has_subformula_cb_sym ws bf_cb_arg ws_required bf_cb_arg ws_required bf_cb_arg.
		q(nt(151), (nt(208)+nt(14)+nt(192)+nt(13)+nt(192)+nt(13)+nt(192)));
		// wff_has_subformula_cb => wff_has_subformula_cb_sym ws wff_cb_arg ws_required wff_cb_arg ws_required wff_cb_arg.
		q(nt(95), (nt(209)+nt(14)+nt(197)+nt(13)+nt(197)+nt(13)+nt(197)));
		// bf_has_clashing_subformulas_cb_sym => 'b' 'f' '_' 'h' 'a' 's' '_' 'c' 'l' 'a' 's' 'h' 'i' 'n' 'g' '_' 's' 'u' 'b' 'f' 'o' 'r' 'm' 'u' 'l' 'a' 's' '_' 'c' 'b'.
		q(nt(206), (t(37)+t(42)+t(27)+t(45)+t(34)+t(46)+t(27)+t(47)+t(35)+t(34)+t(46)+t(45)+t(26)+t(48)+t(49)+t(27)+t(46)+t(7)+t(37)+t(42)+t(28)+t(50)+t(51)+t(7)+t(35)+t(34)+t(46)+t(27)+t(47)+t(37)));
		// bf_has_subformula_cb_sym => 'b' 'f' '_' 'h' 'a' 's' '_' 's' 'u' 'b' 'f' 'o' 'r' 'm' 'u' 'l' 'a' '_' 'c' 'b'.
		q(nt(208), (t(37)+t(42)+t(27)+t(45)+t(34)+t(46)+t(27)+t(46)+t(7)+t(37)+t(42)+t(28)+t(50)+t(51)+t(7)+t(35)+t(34)+t(27)+t(47)+t(37)));
		// wff_has_clashing_subformulas_cb_sym => 'w' 'f' 'f' '_' 'h' 'a' 's' '_' 'c' 'l' 'a' 's' 'h' 'i' 'n' 'g' '_' 's' 'u' 'b' 'f' 'o' 'r' 'm' 'u' 'l' 'a' 's' '_' 'c' 'b'.
		q(nt(207), (t(52)+t(42)+t(42)+t(27)+t(45)+t(34)+t(46)+t(27)+t(47)+t(35)+t(34)+t(46)+t(45)+t(26)+t(48)+t(49)+t(27)+t(46)+t(7)+t(37)+t(42)+t(28)+t(50)+t(51)+t(7)+t(35)+t(34)+t(46)+t(27)+t(47)+t(37)));
		// wff_has_subformula_cb_sym => 'w' 'f' 'f' '_' 'h' 'a' 's' '_' 's' 'u' 'b' 'f' 'o' 'r' 'm' 'u' 'l' 'a' '_' 'c' 'b'.
		q(nt(209), (t(52)+t(42)+t(42)+t(27)+t(45)+t(34)+t(46)+t(27)+t(46)+t(7)+t(37)+t(42)+t(28)+t(50)+t(51)+t(7)+t(35)+t(34)+t(27)+t(47)+t(37)));
		// wff_remove_existential_cb_sym => 'w' 'f' 'f' '_' 'r' 'e' 'm' 'o' 'v' 'e' '_' 'e' 'x' 'i' 's' 't' 'e' 'n' 't' 'i' 'a' 'l' '_' 'c' 'b'.
		q(nt(203), (t(52)+t(42)+t(42)+t(27)+t(50)+t(36)+t(51)+t(28)+t(53)+t(36)+t(27)+t(36)+t(6)+t(26)+t(46)+t(54)+t(36)+t(48)+t(54)+t(26)+t(34)+t(35)+t(27)+t(47)+t(37)));
		// wff_remove_bexistential_cb_sym => 'w' 'f' 'f' '_' 'r' 'e' 'm' 'o' 'v' 'e' '_' 'b' 'e' 'x' 'i' 's' 't' 'e' 'n' 't' 'i' 'a' 'l' '_' 'c' 'b'.
		q(nt(204), (t(52)+t(42)+t(42)+t(27)+t(50)+t(36)+t(51)+t(28)+t(53)+t(36)+t(27)+t(37)+t(36)+t(6)+t(26)+t(46)+t(54)+t(36)+t(48)+t(54)+t(26)+t(34)+t(35)+t(27)+t(47)+t(37)));
		// wff_remove_buniversal_cb_sym => 'w' 'f' 'f' '_' 'r' 'e' 'm' 'o' 'v' 'e' '_' 'b' 'u' 'n' 'i' 'v' 'e' 'r' 's' 'a' 'l' '_' 'c' 'b'.
		q(nt(205), (t(52)+t(42)+t(42)+t(27)+t(50)+t(36)+t(51)+t(28)+t(53)+t(36)+t(27)+t(37)+t(7)+t(48)+t(26)+t(53)+t(36)+t(50)+t(46)+t(34)+t(35)+t(27)+t(47)+t(37)));
		// bf_remove_fexistential_cb_sym => 'b' 'f' '_' 'r' 'e' 'm' 'o' 'v' 'e' '_' 'f' 'e' 'x' 'i' 's' 't' 'e' 'n' 't' 'i' 'a' 'l' '_' 'c' 'b'.
		q(nt(202), (t(37)+t(42)+t(27)+t(50)+t(36)+t(51)+t(28)+t(53)+t(36)+t(27)+t(42)+t(36)+t(6)+t(26)+t(46)+t(54)+t(36)+t(48)+t(54)+t(26)+t(34)+t(35)+t(27)+t(47)+t(37)));
		// bf_remove_funiversal_cb_sym => 'b' 'f' '_' 'r' 'e' 'm' 'o' 'v' 'e' '_' 'f' 'u' 'n' 'i' 'v' 'e' 'r' 's' 'a' 'l' '_' 'c' 'b'.
		q(nt(201), (t(37)+t(42)+t(27)+t(50)+t(36)+t(51)+t(28)+t(53)+t(36)+t(27)+t(42)+t(7)+t(48)+t(26)+t(53)+t(36)+t(50)+t(46)+t(34)+t(35)+t(27)+t(47)+t(37)));
		// bf_cb_arg => bf.
		q(nt(192), (nt(139)));
		// wff_cb_arg => wff.
		q(nt(197), (nt(88)));
		// bf_and_cb_sym => 'b' 'f' '_' 'a' 'n' 'd' '_' 'c' 'b'.
		q(nt(191), (t(37)+t(42)+t(27)+t(34)+t(48)+t(55)+t(27)+t(47)+t(37)));
		// bf_or_cb_sym => 'b' 'f' '_' 'o' 'r' '_' 'c' 'b'.
		q(nt(193), (t(37)+t(42)+t(27)+t(28)+t(50)+t(27)+t(47)+t(37)));
		// bf_xor_cb_sym => 'b' 'f' '_' 'x' 'o' 'r' '_' 'c' 'b'.
		q(nt(194), (t(37)+t(42)+t(27)+t(6)+t(28)+t(50)+t(27)+t(47)+t(37)));
		// bf_neg_cb_sym => 'b' 'f' '_' 'n' 'e' 'g' '_' 'c' 'b'.
		q(nt(195), (t(37)+t(42)+t(27)+t(48)+t(36)+t(49)+t(27)+t(47)+t(37)));
		// bf_eq_cb_sym => 'b' 'f' '_' 'e' 'q' '_' 'c' 'b'.
		q(nt(196), (t(37)+t(42)+t(27)+t(36)+t(56)+t(27)+t(47)+t(37)));
		// bf_neq_cb_sym => 'b' 'f' '_' 'n' 'e' 'q' '_' 'c' 'b'.
		q(nt(198), (t(37)+t(42)+t(27)+t(48)+t(36)+t(56)+t(27)+t(47)+t(37)));
		// bf_is_zero_cb_sym => 'b' 'f' '_' 'i' 's' '_' 'z' 'e' 'r' 'o' '_' 'c' 'b'.
		q(nt(199), (t(37)+t(42)+t(27)+t(26)+t(46)+t(27)+t(57)+t(36)+t(50)+t(28)+t(27)+t(47)+t(37)));
		// bf_is_one_cb_sym => 'b' 'f' '_' 'i' 's' '_' 'o' 'n' 'e' '_' 'c' 'b'.
		q(nt(200), (t(37)+t(42)+t(27)+t(26)+t(46)+t(27)+t(28)+t(48)+t(36)+t(27)+t(47)+t(37)));
		// _Rtau_collapse_positives_cb_30 => tau_collapse_positives_cb_sym ws_required tau_cb_arg ws_required tau_cb_arg ws_required tau_cb_arg.
		q(nt(212), (nt(210)+nt(13)+nt(211)+nt(13)+nt(211)+nt(13)+nt(211)));
		// _Rtau_collapse_positives_cb_31 => tau_collapse_positives_cb_sym ws_required tau_cb_arg ws_required tau_cb_arg.
		q(nt(213), (nt(210)+nt(13)+nt(211)+nt(13)+nt(211)));
		// tau_collapse_positives_cb => _Rtau_collapse_positives_cb_30.
		q(nt(72), (nt(212)));
		// tau_collapse_positives_cb => _Rtau_collapse_positives_cb_31.
		q(nt(72), (nt(213)));
		// tau_positives_upwards_cb => tau_positives_upwards_cb_sym ws_required tau_cb_arg ws_required tau_cb_arg.
		q(nt(73), (nt(214)+nt(13)+nt(211)+nt(13)+nt(211)));
		// tau_cb_arg => capture.
		q(nt(211), (nt(56)));
		// tau_cb_arg => tau.
		q(nt(211), (nt(71)));
		// tau_collapse_positives_cb_sym => 't' 'a' 'u' '_' 'c' 'o' 'l' 'l' 'a' 'p' 's' 'e' '_' 'p' 'o' 's' 'i' 't' 'i' 'v' 'e' 's' '_' 'c' 'b'.
		q(nt(210), (t(54)+t(34)+t(7)+t(27)+t(47)+t(28)+t(35)+t(35)+t(34)+t(58)+t(46)+t(36)+t(27)+t(58)+t(28)+t(46)+t(26)+t(54)+t(26)+t(53)+t(36)+t(46)+t(27)+t(47)+t(37)));
		// tau_positives_upwards_cb_sym => 't' 'a' 'u' '_' 'p' 'o' 's' 'i' 't' 'i' 'v' 'e' 's' '_' 'u' 'p' 'w' 'a' 'r' 'd' 's' '_' 'c' 'b'.
		q(nt(214), (t(54)+t(34)+t(7)+t(27)+t(58)+t(28)+t(46)+t(26)+t(54)+t(26)+t(53)+t(36)+t(46)+t(27)+t(7)+t(58)+t(52)+t(34)+t(50)+t(55)+t(46)+t(27)+t(47)+t(37)));
		// _Rinputs_32 => ws input.
		q(nt(217), (nt(14)+nt(216)));
		// _Rinputs_33 => null.
		q(nt(218), (nul));
		// _Rinputs_33 => _Rinputs_32 _Rinputs_33.
		q(nt(218), (nt(217)+nt(218)));
		// inputs => ws less ws input _Rinputs_33 ws dot.
		q(nt(215), (nt(14)+nt(48)+nt(14)+nt(216)+nt(218)+nt(14)+nt(38)));
		// input => in ws colon ws open_brace ws source_binding ws close_brace.
		q(nt(216), (nt(62)+nt(14)+nt(46)+nt(14)+nt(43)+nt(14)+nt(184)+nt(14)+nt(44)));
		// builder => ws builder_head ws builder_body ws dot.
		q(nt(219), (nt(14)+nt(220)+nt(14)+nt(221)+nt(14)+nt(38)));
		// _Rbuilder_head_34 => ws_required capture.
		q(nt(222), (nt(13)+nt(56)));
		// _Rbuilder_head_35 => null.
		q(nt(223), (nul));
		// _Rbuilder_head_35 => _Rbuilder_head_34 _Rbuilder_head_35.
		q(nt(223), (nt(222)+nt(223)));
		// builder_head => open_parenthesis ws capture _Rbuilder_head_35 ws close_parenthesis.
		q(nt(220), (nt(39)+nt(14)+nt(56)+nt(223)+nt(14)+nt(40)));
		// builder_body => bf_builder_body.
		q(nt(221), (nt(224)));
		// builder_body => wff_builder_body.
		q(nt(221), (nt(225)));
		// builder_body => tau_builder_body.
		q(nt(221), (nt(226)));
		// bf_builder_body => bf_def ws bf.
		q(nt(224), (nt(35)+nt(14)+nt(139)));
		// wff_builder_body => wff_def ws wff.
		q(nt(225), (nt(36)+nt(14)+nt(88)));
		// tau_builder_body => tau_def ws tau.
		q(nt(226), (nt(37)+nt(14)+nt(71)));
		// library => rules.
		q(nt(227), (nt(228)));
		// _Rrules_36 => ws rule.
		q(nt(230), (nt(14)+nt(229)));
		// _Rrules_37 => null.
		q(nt(231), (nul));
		// _Rrules_37 => _Rrules_36 _Rrules_37.
		q(nt(231), (nt(230)+nt(231)));
		// rules => _Rrules_37.
		q(nt(228), (nt(231)));
		// rule => tau_rule.
		q(nt(229), (nt(68)));
		// rule => wff_rule.
		q(nt(229), (nt(89)));
		// rule => bf_rule.
		q(nt(229), (nt(145)));
		// nso_rr => nso_rec_relations nso_main.
		q(nt(232), (nt(233)+nt(234)));
		// _Rnso_rec_relations_38 => ws nso_rec_relation.
		q(nt(236), (nt(14)+nt(235)));
		// _Rnso_rec_relations_39 => null.
		q(nt(237), (nul));
		// _Rnso_rec_relations_39 => _Rnso_rec_relations_38 _Rnso_rec_relations_39.
		q(nt(237), (nt(236)+nt(237)));
		// nso_rec_relations => _Rnso_rec_relations_39.
		q(nt(233), (nt(237)));
		// nso_rec_relation => wff_rec_relation.
		q(nt(235), (nt(99)));
		// nso_rec_relation => bf_rec_relation.
		q(nt(235), (nt(168)));
		// nso_main => ws wff ws dot.
		q(nt(234), (nt(14)+nt(88)+nt(14)+nt(38)));
		// gssotc_rr => gssotc_rec_relations gssotc_main.
		q(nt(238), (nt(239)+nt(240)));
		// _Rgssotc_rec_relations_40 => ws gssotc_rec_relation.
		q(nt(242), (nt(14)+nt(241)));
		// _Rgssotc_rec_relations_41 => null.
		q(nt(243), (nul));
		// _Rgssotc_rec_relations_41 => _Rgssotc_rec_relations_40 _Rgssotc_rec_relations_41.
		q(nt(243), (nt(242)+nt(243)));
		// gssotc_rec_relations => _Rgssotc_rec_relations_41.
		q(nt(239), (nt(243)));
		// gssotc_rec_relation => tau_rec_relation.
		q(nt(241), (nt(74)));
		// gssotc_rec_relation => wff_rec_relation.
		q(nt(241), (nt(99)));
		// gssotc_rec_relation => bf_rec_relation.
		q(nt(241), (nt(168)));
		// gssotc_main => ws tau ws semicolon.
		q(nt(240), (nt(14)+nt(71)+nt(14)+nt(47)));
		// _Rstart_42 => inputs.
		q(nt(245), (nt(215)));
		// _Rstart_42 => builder.
		q(nt(245), (nt(219)));
		// _Rstart_42 => library.
		q(nt(245), (nt(227)));
		// _Rstart_42 => nso_rr.
		q(nt(245), (nt(232)));
		// _Rstart_42 => gssotc_rr.
		q(nt(245), (nt(238)));
		// start => _Rstart_42 ws.
		q(nt(244), (nt(245)+nt(14)));
		return q;
	}
};
#endif // __TAU_PARSER_H__
