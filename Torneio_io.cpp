// Torneio_io.cpp
// Codigo de leitura/gravacao especifico para `Torneio`.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "Torneio_io.h"

#include "xml.h"
#include "Torneio.h"
#include "util.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

namespace yuriks
{
	namespace
	{
		/** Consumidor de eventos para leitura de um Torneio de um arquivo XML.
		*
		 * Esta classe eh implementada como uma maquina de estados que faz a verificacao
		 * de que todas as tags e atributos necessarios estao presentes.
		 */
		class TorneioConsumer : public xml::EventConsumer
		{
		public:
			TorneioConsumer(Torneio& torneio);

			// Eventos
			void tag_open(const char* tag_name);
			void tag_close(const char* tag_name);
			void tag_prop(const char* prop_name, const char* prop_val);
			void tag_text_node(const char* value);

		private:
			enum CurrentTag
			{
				CURTAG_NONE,
				CURTAG_ROOT,
				CURTAG_TRIO,
				CURTAG_JOGADOR
			};

			Torneio& torneio;

			CurrentTag cur_tag;
			bool prop_read[3];
			unsigned int num_jogadores;
			bool root_closed;
			Equipe cur_equipe;

			// Nao assignavel
			TorneioConsumer& operator=(const TorneioConsumer&);
		};

		TorneioConsumer::TorneioConsumer(Torneio& torneio)
			: torneio(torneio), cur_tag(CURTAG_NONE), root_closed(false)
		{
		}

		void TorneioConsumer::tag_open(const char* tag_name)
		{
			using xml::ReadingException;

			switch (cur_tag)
			{
			case CURTAG_NONE:
			{
				if (root_closed)
					throw ReadingException("Apenas uma tag raiz 'truco' permitida.");

				if (std::strcmp(tag_name, "truco") == 0)
				{
					cur_tag = CURTAG_ROOT;
					prop_read[0] = false;
				}
				else
					throw ReadingException("Tag 'truco' esperada.");
			} break;
			case CURTAG_ROOT:
			{
				if (std::strcmp(tag_name, "trio") == 0)
				{
					cur_tag = CURTAG_TRIO;
					prop_read[0] = prop_read[1] = false;
					num_jogadores = 0;
					cur_equipe = Equipe();
				}
				else
					throw ReadingException("Tag 'trio' esperada.");
			} break;
			case CURTAG_TRIO:
			{
				if (!prop_read[0])
					throw ReadingException("Tag 'trio' requer atributo 'apelido'.");
				if (!prop_read[1])
					throw ReadingException("Tag 'trio' requer atributo 'id'.");

				if (std::strcmp(tag_name, "jogador") == 0)
				{
					num_jogadores++;
					if (num_jogadores > 3)
						throw ReadingException("Apenas 3 jogadores por trio.");

					prop_read[2] = false;
					cur_tag = CURTAG_JOGADOR;
					
				}
				else
					throw ReadingException("Tag 'jogador' esperada.");
			} break;
			case CURTAG_JOGADOR:
			{
				throw ReadingException("Tag inesperada.");
			} break;
			}
		}

		void TorneioConsumer::tag_close(const char* /*tag_name*/)
		{
			using xml::ReadingException;

			switch (cur_tag)
			{
			case CURTAG_NONE:
			{
				// Nunca deve acontecer
				assert(false);
			} break;
			case CURTAG_ROOT:
			{
				cur_tag = CURTAG_NONE;
				root_closed = true;
			} break;
			case CURTAG_TRIO:
			{
				if (num_jogadores != 3)
					throw ReadingException("Cada trio requer 3 jogadores.");
				torneio.add_equipe(cur_equipe);

				cur_tag = CURTAG_ROOT;
			} break;
			case CURTAG_JOGADOR:
			{
				if (!prop_read[2])
					cur_equipe.integrantes[num_jogadores-1][0] = '\0';
					//throw ReadingException("Nome do jogador faltando.");
				cur_tag = CURTAG_TRIO;
			} break;
			}
		}

		void TorneioConsumer::tag_prop(const char* prop_name, const char* prop_val)
		{
			using xml::ReadingException;

			switch (cur_tag)
			{
			case CURTAG_NONE:
			{
				// Nunca deve acontecer
				assert(false);
			} break;
			case CURTAG_ROOT:
			{
				if (std::strcmp(prop_name, "trios") == 0)
				{
					if (!prop_read[0])
					{
						std::istringstream sstr(prop_val);
						unsigned int num_trios;
						if (!(sstr >> num_trios))
							throw ReadingException("Erro lendo valor do atributo 'trios'.");
						torneio.reserva_equipes(num_trios);

						prop_read[0] = true;
					}
					else
						throw ReadingException("Atributo 'trios' duplicado.");
				}
				else
					throw ReadingException("Atributo inesperado.");
			} break;
			case CURTAG_TRIO:
			{
				if (std::strcmp(prop_name, "apelido") == 0)
				{
					if (!prop_read[0])
					{
						strnzcpy(cur_equipe.nome, prop_val, 21);
						prop_read[0] = true;
					}
					else
						throw ReadingException("Atributo 'trios' duplicado.");
				}
				else if (std::strcmp(prop_name, "id") == 0)
				{
					if (!prop_read[1])
					{
						std::istringstream sstr(prop_val);
						unsigned int id_trio;
						if (!(sstr >> id_trio))
							throw ReadingException("Erro lendo valor do atributo 'id'.");

						// Sera ignorado depois. (OK?)
						cur_equipe.id = id_trio;
						prop_read[1] = true;
					}
					else
						throw ReadingException("Atributo 'id' duplicado.");
				}
				else
					throw ReadingException("Atributo inesperado.");
			} break;
			case CURTAG_JOGADOR:
			{
				throw ReadingException("Atributo inesperado.");
			} break;
			}
		}

		void TorneioConsumer::tag_text_node(const char* value)
		{
			using xml::ReadingException;

			switch (cur_tag)
			{
			case CURTAG_NONE:
			{
				// Nunca deve acontecer
				assert(false);
			} break;
			case CURTAG_ROOT:
			{
				throw ReadingException("Texto inesperado.");
			} break;
			case CURTAG_TRIO:
			{
				throw ReadingException("Texto inesperado.");
			} break;
			case CURTAG_JOGADOR:
			{
				strnzcpy(cur_equipe.integrantes[num_jogadores-1], value, 33);
				prop_read[2] = true;
			} break;
			}
		}
	} // namespace

	// Essa funcao le algo quase, mas nao completamente, diferente de XML.
	// Nao suporta tags self-closing, CDATA, maioria das entities, header de versao XML, etc.
	bool le_torneio(std::istream& f, Torneio& torneio)
	{
		xml::Reader leitor(f);
		TorneioConsumer consumidor(torneio);
		try
		{
			leitor.read_xml(consumidor);
		}
		catch (const xml::ReadingException& e)
		{
			std::cout << "Erro no arquivo xml, linha " << leitor.line_num() << ": " << e.what() << std::endl;
			return false;
		}

		return true;
	}

	// Grava Torneio em arquivo. Apenas alguns prints triviais.
	void grava_torneio(std::ostream& f, const Torneio& torneio)
	{
		f << "<truco trios=\"" << torneio.num_equipes() << "\">\n";
		for (VetorEquipe::ConstIterator trio = torneio.begin_equipes();
			trio != torneio.end_equipes(); ++trio)
		{
			f << "    <trio apelido=\"";
			xml::write_string_entenc(f, (*trio)->nome);
			f << "\" id=\"" << (*trio)->id << "\">\n";
			for (unsigned int j = 0; j < 3; ++j)
			{
				f << "        <jogador>";
				xml::write_string_entenc(f, (*trio)->integrantes[j]);
				f << "</jogador>\n";
			}
			f << "    </trio>\n";
		}
		f << "</truco>\n";
	}
} // namespace yuriks
