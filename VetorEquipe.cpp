// VetorEquipe.cpp
// Vetor com redimensionamento automatico.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "VetorEquipe.h"

#include <cassert>

// Nao poder fazer templates = :P
#define T Equipe
#define Vetor VetorEquipe

namespace yuriks
{
	Vetor::Vetor()
		: items(new T*[0]), // Arrays com tamanho 0 sao OKs
		size_(0), capacity_(0)
	{
	}

	Vetor::Vetor(unsigned int tam)
		: items(new T*[tam]),
		size_(0), // Tem 0 items, apenas pre-aloca memoria
		capacity_(tam)
	{
	}

	Vetor::~Vetor()
	{
		// Deleta cada elemento do vetor.
		for (T** i = items; i != items+size(); ++i)
		{
			delete *i;
		}

		delete[] items;
	}


	Vetor::Vetor(const Vetor& v)
		: items(new T*[v.size()]),
		size_(v.size()),
		capacity_(v.size())
	{
		// Faz copias de cada elemento de v
		ConstIterator j = v.begin();
		for (T** i = items; j != v.end(); ++i, ++j)
		{
			*i = new T(**j);
		}
	}

	Vetor& Vetor::operator=(const Vetor& v)
	{
		// Tenha certeza que temos espaco para os items
		reserve(v.size());

		// Apaga elementos antigos
		for (T** i = items; i != items+size(); ++i)
		{
			delete *i;
		}

		// Faz copias de cada elemento de v
		ConstIterator j = v.begin();
		for (T** i = items; j != v.end(); ++i, ++j)
		{
			*i = new T(**j);
		}

		return *this;
	}


	Vetor::Iterator Vetor::push_back(const T& e)
	{
		// Tenha certeza que temos espaco para o novo item
		reserve(size()+1);

		T* novo_el = new T(e);

		items[size_] = novo_el;
		++size_;

		return end();
	}

	void Vetor::erase(const Iterator& e)
	{
		if (e == end())
			return;

		delete *e;

		// Perde const's de e
		T** i = items + (e - items);

		// Copia elementos apos o removido um espaco para tras.
		for (++i; i != items+size_; ++i)
		{
			*(i-1) = *i;
		}

		--size_;
	}

	void Vetor::reserve(unsigned int n)
	{
		if (capacity_ >= n)
			return;

		// Aloca novo array...
		T** novo_arr = new T*[n];

		// ...e copia os items velhos.
		//std::copy(items, items+capacity_, novo_arr);
		for (unsigned int i = 0; i < capacity_; ++i)
		{
			novo_arr[i] = items[i];
		}

		// Apaga array antigo e atualiza variaveis.
		delete[] items;
		items = novo_arr;
		capacity_ = n;
	}


	// Nota: T *const *: eh um ponteiro para vetor contante de ponteiros.
	// Evita que usuarios modifiquem `items', permite modificar apenas
	// seus valores.
	Vetor::Iterator Vetor::begin()
	{
		return items;
	}

	Vetor::Iterator Vetor::end()
	{
		return items+size();
	}

	Vetor::ConstIterator Vetor::begin() const
	{
		return items;
	}

	Vetor::ConstIterator Vetor::end() const
	{
		return items+size();
	}


	T& Vetor::operator[](unsigned int i)
	{
		assert(i < size());
		return *items[i];
	}

	const T& Vetor::operator[](unsigned int i) const
	{
		assert(i < size());
		return *items[i];
	}
} // namespace yuriks

#undef T
#undef Vetor
