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

#ifndef __ITERATORS_H__
#define __ITERATORS_H__

#include <vector>
#include <set>
#include <ranges>
#include <memory>


/*!
* Iterator returning the deltas needed to generate grey codes of a certain
* size following Algorithm L (Loopless Gray binary generation) from Knuth
* Vol 4A (originally published by Bitner, Ehrlich and Reingold, CACM 19 (1976),
* pp. 517-521).
*/
class grey_code_const_iterator {
public:
	// iterator traits
	using difference_type = size_t;
	using value_type = size_t;
	using pointer = const size_t*;
	using reference = const size_t&;
	using iterator_category = std::input_iterator_tag;

	// sentinel class
	class sentinel {};
	static constexpr sentinel end{};

	grey_code_const_iterator(size_t size);
	grey_code_const_iterator();
	grey_code_const_iterator &operator++();
	grey_code_const_iterator operator++(int);
	bool operator==(const grey_code_const_iterator&) const;
	bool operator!=(const grey_code_const_iterator &that) const;
	bool operator==(const sentinel &that) const;
	bool operator!=(const sentinel &that) const;
	const size_t &operator*() const;

private:
	std::vector<size_t> focus_pointers_;
	size_t size_;
	size_t delta_;

	bool compute_next_delta_();
};

/*!
 * Iterator returning all the subsets of a given set. It uses the
 * grey_code_const_iterator under the hood.
 */
template<class T>
class power_set_const_iterator {
public:
	// iterator traits
	using difference_type = size_t;
	using value_type = std::vector<T>;
	using pointer = const std::vector<T>;
	using reference = const std::vector<T>&;
	using iterator_category = std::input_iterator_tag;

	// sentinel class
	class sentinel {
	public:
		bool operator==(const power_set_const_iterator<T> &that) const {
			return that.grey_code_ == grey_code_const_iterator::end;
		}

		bool operator!=(const power_set_const_iterator<T> &that) const {
			return !(*this == that);
		}
	};

	static constexpr sentinel end{};

//	power_set_const_iterator(): grey_code_(0) {}

	power_set_const_iterator(std::vector<T> &set): grey_code_(set.size()), set_(set) {}

	power_set_const_iterator &operator++() {
		auto delta = *(++grey_code_);
		auto it = subset_.find(delta);
		if (it != subset_.end())
			subset_.erase(it);
		else
			subset_.insert(delta);
		return *this;
	}

	power_set_const_iterator operator++(int) {
		auto current = *this;
		auto delta = *(++grey_code_);
		auto it = subset_.find(delta);
		if (it != subset_.end())
			subset_.erase(it);
		else
			subset_.insert(delta);
		return current;
	}

	bool operator==(power_set_const_iterator<T> &that) const {
		return set_ == that.set_ && subset_ == that.subset_;
	}

	bool operator!=(power_set_const_iterator<T> &that) const {
		return !(*this == that);
	}

	bool operator==(power_set_const_iterator<T>::sentinel that) const {
		return that == *this;
	}

	bool operator!=(power_set_const_iterator<T>::sentinel that) const {
		return !(that == *this);
	}

	const std::vector<T> operator*() const{
		std::vector<T> v;
		for(auto it = subset_.begin(); it != subset_.end(); ++it) {
			v.emplace_back(set_[*it]);
		}
		return v;
	}

private:
	grey_code_const_iterator grey_code_;
	std::vector<T> &set_;
	std::set<size_t> subset_;
};

/*!
 * Range returning iterators to the poserset of the provided set.
 */
template<class T>
class powerset_range {
public:

	explicit powerset_range(std::vector<T>& set) : set_(set) {}

	bool empty() {
		return set_.size() == 0;
	}

	power_set_const_iterator<T> begin() {
		power_set_const_iterator<T> begin(set_);
		return begin;
	}

	power_set_const_iterator<T>::sentinel end() {
		return power_set_const_iterator<T>::end;
	}

private:
	std::vector<T> &set_;
};

/*!
 * Iterator returning all the partitions of a given set. It uses the
 * grey_code_const_iterator under the hood.
 */
template<class T>
class partitions_const_iterator {
public:
	// iterator traits
	using difference_type = size_t;
	using value_type = std::vector<T>;
	using pointer = const std::vector<T>;
	using reference = const std::vector<T>&;
	using iterator_category = std::input_iterator_tag;

	// sentinel class
	class sentinel {
	public:
		bool operator==(const power_set_const_iterator<T> &that) const {
			return that.grey_code_ == grey_code_const_iterator::end;
		}

		bool operator!=(const power_set_const_iterator<T> &that) const {
			return !(*this == that);
		}
	};

	static constexpr sentinel end{};

//	power_set_const_iterator(): grey_code_(0) {}

	partitions_const_iterator(std::vector<T> &set): grey_code_(set.size()), set_(set) {}

	partitions_const_iterator &operator++() {
		auto delta = *(++grey_code_);
		auto it = first_.find(delta);
		if (it != first_.end())
			first_.erase(it), second_.insert(delta);
		else
			first_.insert(delta), second_.erase(delta);
		return *this;
	}

	// REVIEW: operations on second_...
	partitions_const_iterator operator++(int) {
		auto current = *this;
		auto delta = *(++grey_code_);
		auto it = first_.find(delta);
		if (it != first_.end())
			first_.erase(it);
		else
			first_.insert(delta);
		return current;
	}

	bool operator==(partitions_const_iterator<T> &that) const {
		return set_ == that.set_ && first_ == that.first_ && second_ == that.second_;
	}

	bool operator!=(partitions_const_iterator<T> &that) const {
		return !(*this == that);
	}

	const std::pair<std::vector<T>, std::vector<T>> operator*() const{
		std::vector<T> f(first_.begin(), first_.end());
		std::vector<T> s(first_.begin(), first_.end());
		return std::make_pair(f,s);
	}

private:
	grey_code_const_iterator grey_code_;
	std::vector<T> &set_;
	std::set<size_t> first_;
	std::set<size_t> second_;
};

/*!
 * Range returning iterators to the poserset of the provided set.
 */
template<class T>
class partition_range {
public:

	explicit partition_range(std::vector<T>& set) : set_(set) {}

	bool empty() {
		return set_.size() == 0;
	}

	partitions_const_iterator<T> begin() {
		partitions_const_iterator<T> begin(set_);
		return begin;
	}

	partitions_const_iterator<T>::sentinel end() {
		return partitions_const_iterator<T>::end;
	}

private:
	std::vector<T> &set_;
};

#endif // __ITERATORS_H__
