// PilhaFonteCompetidor.h
// Estrutura de dados de pilha.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_PILHAFONTECOMPETIDOR_H
#define YURIKS_T1_PILHAFONTECOMPETIDOR_H

namespace yuriks
{
	class PilhaFonteCompetidor;
} // namespace yuriks

#include "SimuladorTorneio.h"

// Nao poder fazer templates = :P
#define Pilha PilhaFonteCompetidor

namespace yuriks
{
	class Pilha
	{
	public:
		// Nao poder fazer templates = :P
		typedef ::yuriks::simulador::FonteCompetidor* T;

		// Reserva tamanho para tam elementos.
		explicit Pilha(unsigned int tam);
		~Pilha();

		Pilha(const Pilha& v); // Copy constructor
		Pilha& operator=(const Pilha& v);

		// Acessa elemento no topo da pilha.
		T& top();
		const T& top() const;
		// Insere novo elemento no topo da pilha.
		void push(const T& e);
		// Remove elemento do topo da pilha.
		void pop();

		// Retorna numero de items na pilha.
		unsigned int size() const;
		// Retorna tamanho maximo da pilha.
		unsigned int capacity() const;

	private:
		T* items;

		unsigned int size_;
		unsigned int capacity_;
	};


	inline unsigned int Pilha::size() const
	{
		return size_;
	}

	inline unsigned int Pilha::capacity() const
	{
		return capacity_;
	}
} // namespace yuriks

#undef Pilha

#endif // YURIKS_T1_PILHAFONTECOMPETIDOR_H
