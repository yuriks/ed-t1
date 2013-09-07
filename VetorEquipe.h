// VetorEquipe.h
// Vetor com redimensionamento automatico.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_VETOREQUIPE_H
#define YURIKS_T1_VETOREQUIPE_H

#include "Equipe.h"

// Nao poder fazer templates = :P entao eu fiz isso
// caso queira reutilizar o codigo no futuro.
#define T Equipe
#define Vetor VetorEquipe

namespace yuriks
{
	/** Vetor de tamanho dinamico. Lida automaticamente com o tamanho do vetor
	 * a medida que entradas sao inseridas.
	 
	 * Implementado usando um vetor de ponteiros, portanto, nao recomendado
	 * para tipos basicos e/ou muito pequenos.
	 */
	class Vetor
	{
	public:
		typedef T *const * Iterator;
		typedef const T *const * ConstIterator;

		Vetor();
		// Reserva tamanho para tam elementos.
		explicit Vetor(unsigned int tam);
		~Vetor();

		Vetor(const Vetor& v); // Copy constructor
		Vetor& operator=(const Vetor& v);

		// Insere novo elemento no fim do vetor.
		// Retorna iterador para o elemento adicionado.
		Iterator push_back(const T& e);
		// Remove elemento apontado pelo iterador. Se e == end(), nao faz nada.
		void erase(const Iterator& e);

		// Retorna numero de items no vetor.
		unsigned int size() const;
		// Retorna tamanho pre-alocado.
		unsigned int capacity() const;
		// Pre-aloca espaco para ate n items.
		void reserve(unsigned int n);

		// TODO Criar iterador para nao vazar a implementacao
		Iterator begin();
		Iterator end();
		ConstIterator begin() const;
		ConstIterator end() const;

		// Acesso aleatorio por indice.
		T& operator[](unsigned int i);
		const T& operator[](unsigned int i) const;

	private:
		T** items;

		unsigned int size_;
		unsigned int capacity_;
	};


	inline unsigned int Vetor::size() const
	{
		return size_;
	}

	inline unsigned int Vetor::capacity() const
	{
		return capacity_;
	}
} // namespace yuriks

#undef T
#undef Vetor

#endif // YURIKS_T1_VETOREQUIPE_H
