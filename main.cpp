// main.cpp
// Funcao principal e interface de linha de comando (CLI).
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "Torneio.h"
#include "util.h"
#include "Torneio_io.h"
#include "SimuladorTorneio.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace yuriks;

namespace
{
	/** Par nome->ponteiro-para-funcao. Usado para despachar comandos da CLI. */
	struct Comando
	{
		const char* nome;
		void (*func)(Torneio&);

		static const Comando comandos[];
		// Procura comando com nome `nome` em `comandos`. Retorna null se nao encontrado.
		static const Comando* procura_comando(const char* nome);
	};

	const Comando* Comando::procura_comando(const char* nome)
	{
		for (const Comando* c = &comandos[0]; c->nome != 0; ++c)
		{
			if (istrcmp(nome, c->nome) == 0)
			{
				return c;
			}
		}

		return 0;
	}


	////
	// Inicio dos comandos
	///

	void adiciona_equipe(Torneio& torneio)
	{
		Equipe e;

		std::cout << "Nome da equipe: ";
		std::cin.getline(e.nome, 21);
		for (unsigned int i = 0; i < 3; ++i)
		{
			std::cout << "  Integrante " << i+1 << ": ";
			std::cin.getline(e.integrantes[i], 33);
		}

		torneio.add_equipe(e);
	}

	void remove_equipe(Torneio& torneio)
	{
		std::cout << "Id da equipe: ";
		char buf[128];
		std::cin.getline(buf, 128);

		std::istringstream sstr(buf);
		unsigned int id;
		if (sstr >> id)
		{
			VetorEquipe::Iterator i = torneio.encontrar_equipe(id);
			if (i == torneio.end_equipes())
				std::cout << "Id nao encontrado." << std::endl;
			else
				torneio.remover_equipe(i);
		}
		else
			std::cout << "Erro de entrada." << std::endl;
	}

	void lista_equipes(Torneio& torneio)
	{
		for (VetorEquipe::ConstIterator i = torneio.begin_equipes();
			i != torneio.end_equipes(); ++i)
		{
			imprime_equipe(std::cout, **i);
		}
		std::cout << torneio.num_equipes() << " equipe";
		if (torneio.num_equipes() != 1)
			std::cout << 's';
		std::cout << " no total." << std::endl;
	}

	void procura_equipes(Torneio& torneio)
	{
		std::cout << "Texto para procurar: ";
		char texto[33];
		std::cin.getline(texto, 33);

		VetorEquipe::Iterator i = torneio.encontrar_equipe(texto,
				torneio.begin_equipes());
		while (i != torneio.end_equipes())
		{
			imprime_equipe(std::cout, **i);
			i = torneio.encontrar_equipe(texto, i+1);
		}
		std::cout << std::flush;
	}


	void carregar_equipes(Torneio& torneio)
	{
		std::ifstream f("registro.xml");
		if (f)
		{
			if (le_torneio(f, torneio))
				std::cout << "Leu registro.xml" << std::endl;
			else
				std::cout << "Erro no formato de registro.xml" << std::endl;
		}
		else
			std::cout << "Nao conseguiu abrir registro.xml" << std::endl;
	}

	void gravar_equipes(Torneio& torneio)
	{
		std::ofstream f("registro.xml");
		if (f)
		{
			grava_torneio(f, torneio);
			std::cout << "registro.xml escrito com sucesso." << std::endl;
		}
		else
			std::cout << "Nao pode abrir registro.xml" << std::endl;
	}

	void simular_campeonato(Torneio& torneio)
	{
		if (torneio.num_equipes() > 0)
		{
			std::cout << "Arquivo para gravar (deixe em branco para saida padrao): ";
			char arquivo[256];
			std::cin.getline(arquivo, 256);

			std::ostream* f;
			std::ofstream arq;
			if (std::strlen(arquivo) == 0)
				f = &std::cout;
			else
			{
				arq.open(arquivo);
				if (!arq)
				{
					std::cout << "Nao pode abrir " << arquivo << std::endl;
					return;
				}
				f = &arq;
			}

			// Inicializa gerador de numero aleatorio com a hora atual.
			std::srand(static_cast<unsigned int>(std::time(0)));

			simulador::SimuladorTorneio simul(torneio);
			simul.simular();
			simul.imprimir(*f);
		}
		else
			std::cout << "Necessita de pelo menos uma equipe." << std::endl;
	}

	void lista_comandos(Torneio& /* nao usado */)
	{
		std::cout << "Comandos disponiveis: (Nao precisa digitar todo o comando.)\n  ";
		for (const Comando* c = Comando::comandos; c->nome != 0; ++c)
		{
			std::cout << c->nome << ' ';
		}
		std::cout << std::endl;
	}

	////
	// Fim dos comandos
	///


	// Tabela com os comandos da CLI.
	const Comando Comando::comandos[] =
	{
		{"adicionar", adiciona_equipe},
		{"remover",   remove_equipe},
		{"listar",    lista_equipes},
		{"procurar",  procura_equipes},
		{"carregar",  carregar_equipes},
		{"gravar",    gravar_equipes},
		{"simular",   simular_campeonato},
		{"ajuda",     lista_comandos},
		{"?",         lista_comandos},
		{"sair",      0},
		{0, 0}
	};
} // namespace

int main()
{
	std::cout << "Digite 'ajuda' ou '?' para ver os comandos disponiveis." << std::endl;

	Torneio torneio;

	bool sair = false;
	while (!sair)
	{
		std::cout << "> ";
		char buf[128];
		std::cin.getline(buf, 128);

		const Comando* c = Comando::procura_comando(buf);
		if (c)
		{
			if (c->func)
				c->func(torneio);
			else // c->func == 0 sinaliza comando de saida
				sair = true;
		}
		else
		{
			std::cout << "Comando desconhecido." << std::endl;
		}
	}

	// return 0 implicito em C++
}
