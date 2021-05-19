#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>
#include<stdexcept>
#include<initializer_list>

using std::ostream;


template<typename T>
class Vector {

public:
	class Iterator;
	class ConstIterator;
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;

private:

	size_type sz;
	size_type max_sz;
	pointer values;
	static constexpr size_type min_sz{ 5 };

public:


	Vector(size_type n = min_sz) : sz{ 0 }, max_sz{ n < min_sz ? min_sz : n }, values{ new value_type[max_sz] }{}

	Vector(std::initializer_list<value_type> list) : Vector(list.size()) {

		for (const auto& elem : list)
			values[sz++] = elem;
	}

	Vector(const Vector& vect) : Vector(vect.max_sz) {

		for (const auto& elem : vect)
			values[sz++] = elem;
	}

	~Vector() { delete[] values; }

	Vector& operator=(Vector cpy) {

		std::swap(sz, cpy.sz);
		std::swap(max_sz, cpy.max_sz);
		std::swap(values, cpy.values);
		return *this;
	}


	size_type size() const {

		return sz;
	}

	bool empty() const {

		if (sz == 0)
			return true;
		return false;
	}

	void clear() {

		sz = 0;
	}


	size_type capacity() const {

		return max_sz;
	}

	reference operator[](size_type index) {

		if (index > sz - 1 || sz == 0 || index < 0)
			throw std::runtime_error("Invalid index!");

		return values[index];
	}

	const_reference operator[](size_type index) const {

		if (index > sz - 1 || sz == 0 || index < 0)
			throw std::runtime_error("Invalid index!");

		return values[index];
	}

	void reserve(size_type n) {

		if (n > max_sz)
		{
			pointer v2 = new value_type[n];
			for (size_type i{ 0 }; i < sz; ++i)
				v2[i] = values[i];

			delete[] values;
			values = v2;
			max_sz = n;
		}
	}

	void shrink_to_fit() {

		if (sz > max_sz || sz < min_sz)
			throw std::runtime_error("Capacity too big!");

		else
		{
			pointer v2{ new value_type[sz] };
			for (size_type i{ 0 }; i < sz; ++i)
				v2[i] = values[i];

			delete[] values;
			values = v2;
			max_sz = sz;
		}
	}

	void push_back(value_type x) {


		if (sz == max_sz)
		{
			this->reserve(2 * max_sz);
		}

		values[sz++] = x;
	}

	void pop_back() {

		if (sz == 0)
			throw std::runtime_error("Vector is already empty!");

		--sz;
	}


	ostream& print(ostream& out) const {

		out << "[";

		for (size_type i{ 0 }; i < sz; i++)
		{
			out << values[i];
			if (i < sz - 1)
				std::cout << ", ";
		}

		out << "]";

		return out;
	}


	iterator begin() { return iterator(values); }
	const const_iterator begin() const { return const_iterator(values); }
	iterator end() { return iterator(values + sz); }
	const const_iterator end() const { return const_iterator(values + sz); }


	iterator insert(const_iterator pos, const_reference val) {

		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) > sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{ static_cast<size_type>(diff) };
		if (sz >= max_sz)
			reserve(max_sz * 2);
		for (auto i{ sz }; i-- > current;)
			values[i + 1] = values[i];
		values[current] = val;
		++sz;
		return iterator{ values + current };
	}

	iterator erase(const_iterator pos) {

		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) >= sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{ static_cast<size_type>(diff) };
		for (auto i{ current }; i < sz - 1; ++i)
			values[i] = values[i + 1];
		--sz;
		return iterator{ values + current };
	}


	class Iterator {

	public:
		using value_type = Vector::value_type;
		using reference = Vector::reference;
		using pointer = Vector::pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;

	private:

		pointer ptr;

	public:

		Iterator() : ptr{ nullptr } {}

		Iterator(pointer ptr) : ptr{ ptr } {}

		reference operator*() const {

			return *ptr;
		}

		pointer operator->() const {

			return ptr;
		}

		bool operator==(const const_iterator& it) const {

			return static_cast<const_iterator>(*this) == it;
		}

		bool operator!=(const const_iterator& it) const {

			return static_cast<const_iterator>(*this) != it;
		}

		iterator& operator++() {

			ptr++;
			return *this;
		}

		iterator operator++(int) {

			iterator it = ptr;
			++ptr;
			return it;
		}

		operator const_iterator() const {

			return const_iterator{ ptr };
		}
	};



	class ConstIterator {

	public:
		using value_type = Vector::value_type;
		using reference = Vector::const_reference;
		using pointer = Vector::const_pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;

	private:

		pointer ptr;

	public:

		ConstIterator() : ptr{ nullptr } {}

		ConstIterator(pointer ptr) : ptr{ ptr } {}

		reference operator*() const {

			return *ptr;
		}

		pointer operator->() const {

			return ptr;
		}

		bool operator==(const const_iterator& it) const {

			return ptr == it.ptr;
		}

		bool operator!=(const const_iterator& it) const {

			return ptr != it.ptr;
		}

		const_iterator& operator++() {

			ptr++;
			return *this;
		}

		const_iterator operator++(int) {

			const_iterator it = ptr;
			++ptr;
			return it;
		}


		friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
			return lop.ptr - rop.ptr;
		}

	};

};

template<typename T>
ostream& operator<<(ostream& out, const Vector<T>& v) {

	return v.print(out);
}

#endif
