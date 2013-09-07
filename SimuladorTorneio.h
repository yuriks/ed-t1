// SimuladorTorneio.h
// Simulador de torneios. Usa numeros aleatorios para determinar resultado das partidas.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_SIMULADORTORNEIO_H
#define YURIKS_T1_SIMULADORTORNEIO_H

#include "Torneio.h"

#include <iostream>

namespace yuriks
{
	namespace simulador
	{
		class FonteCompetidor
		{
		public:
			virtual const Equipe& vencedora() const = 0;
			virtual ~FonteCompetidor() {}
		};

		class FonteEquipe : public FonteCompetidor
		{
		public:
			explicit FonteEquipe(const Equipe& equipe);
			virtual const Equipe& vencedora() const;

		private:
			const Equipe* equipe;
		};

		class Partida : public FonteCompetidor
		{
		public:
			Partida(FonteCompetidor* a, FonteCompetidor* b);
			virtual ~Partida();

			virtual const Equipe& vencedora() const;
			void simular();

			const FonteCompetidor& equipe_a() const;
			const FonteCompetidor& equipe_b() const;
			int pontos_equipe_a() const;
			int pontos_equipe_b() const;

		private:
			FonteCompetidor* vencedora_;
			FonteCompetidor* compet_a;
			FonteCompetidor* compet_b;
			int pontos_a;
			int pontos_b;
		};

		inline const FonteCompetidor& Partida::equipe_a() const
		{
			return *compet_a;
		}

		inline const FonteCompetidor& Partida::equipe_b() const
		{
			return *compet_b;
		}

		inline int Partida::pontos_equipe_a() const
		{
			return pontos_a;
		}

		inline int Partida::pontos_equipe_b() const
		{
			return pontos_b;
		}
	} // namespace simulador
} // namespace yuriks

#include "PilhaFonteCompetidor.h"

namespace yuriks
{
	namespace simulador
	{
		class SimuladorTorneio
		{
		public:
			explicit SimuladorTorneio(const Torneio& torneio);
			~SimuladorTorneio();
			void simular();
			void imprimir(std::ostream& f) const;

		private:
			typedef ::yuriks::PilhaFonteCompetidor PilhaFC;

			const Torneio& torneio;
			FonteCompetidor* arvore;

			FonteCompetidor* criar_arvore();
			void criar_nivel(PilhaFC& from, PilhaFC& to);

			// Nao assignable
			SimuladorTorneio& operator=(const SimuladorTorneio&);
		};
	} // namespace simulador
} // namespace yuriks

#endif // YURIKS_T1_SIMULADORTORNEIO_H
