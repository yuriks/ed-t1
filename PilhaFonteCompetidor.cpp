// PilhaFonteCompetidor.cpp
// Estrutura de dados de pilha.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "PilhaFonteCompetidor.h"

#include <cassert>
#include <stdexcept>

// Nao poder fazer templates = :P
#define Pilha PilhaFonteCompetidor

namespace yuriks
{
	Pilha::Pilha(unsigned int tam)
		: items(new T[tam]),
		size_(0), // Tem 0 items, apenas pre-aloca memoria
		capacity_(tam)
	{
	}

	Pilha::~Pilha()
	{
		delete[] items;
	}


	Pilha::Pilha(const Pilha& p)
		: items(new T[p.capacity()]),
		size_(p.size()),
		capacity_(p.capacity())
	{
		// Copia elementos
		*this = p;
	}

	Pilha& Pilha::operator=(const Pilha& p)
	{
		// Tenha certeza que temos espaco para os items
		if (p.size() > capacity())
			throw std::overflow_error("Pilha de origem eh muito pequena.");

		// Faz copias de cada elemento de p
		for (unsigned int i = 0; i < p.size(); ++i)
		{
			items[i] = p.items[i];
		}

		return *this;
	}

	Pilha::T& Pilha::top()
	{
		if (size() == 0)
			throw std::underflow_error("Pilha vazia.");

		return items[size()-1];
	}

	const Pilha::T& Pilha::top() const
	{
		if (size() == 0)
			throw std::underflow_error("Pilha vazia.");

		return items[size()-1];
	}

	void Pilha::push(const T& e)
	{
		// Tenha certeza que temos espaco para o novo item
		if (size() == capacity())
			throw std::overflow_error("Pilha cheia.");

		items[size_] = e;
		++size_;
	}

	void Pilha::pop()
	{
		if (size() == 0)
			throw std::underflow_error("Pilha vazia.");

		--size_;
	}
} // namespace yuriks

#undef Pilha
