// xml.h
// Leitor XML baseado em eventos. Semi-completo.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#ifndef YURIKS_T1_XML_H
#define YURIKS_T1_XML_H

#include <iostream>
#include <stdexcept>

namespace yuriks
{
	namespace xml
	{
		/** Interface a ser implementada por classes que desejarem receber eventos do parser. */
		class EventConsumer
		{
		public:
			// Abertura de tag
			virtual void tag_open(const char* tag_name) = 0;
			// Fechamento de tag
			virtual void tag_close(const char* tag_name) = 0;

			// Atributo de tag
			virtual void tag_prop(const char* prop_name, const char* prop_val) = 0;
			// Trecho textual
			virtual void tag_text_node(const char* value) = 0;
		};

		/** Excecao lancada quando forem encontrados erros na leitura do xml. */
		struct ReadingException : std::runtime_error
		{
			ReadingException(const std::string& str)
				: std::runtime_error(str)
			{
			}
		};

		/** Leitor XML. Acredito suportar um sub-conjunto do formato adequado a este trabalho.
		 *
		 * A medida que a leitura do arquivo for sendo realizada, o leitor ira
		 * chamar as funcoes do EventConsumer passado para read_xml.
		 */
		class Reader
		{
		public:
			Reader(std::istream& f);
			// Executa a leitura do arquivo, chamando os eventos em `consumer`.
			void read_xml(EventConsumer& consumer);
			// Numero da ultima linha lida. Util para mensagens de erro.
			unsigned int line_num() const;

		private:
			static const unsigned int max_tag_depth = 8;
			static const unsigned int max_tag_len = 16;
			static const unsigned int max_prop_name_len = 16;
			static const unsigned int max_prop_value_len = 64;
			static const unsigned int max_tag_content_len = 64;

			bool used;
			std::istream& f;
			int cur_line;

			std::istream& getch(char& c);

			// Le ate encontrar caractere nao-espaco.
			void skip_whitespace();
			bool read_next_tag(char* tag, unsigned int tag_len);
			bool read_next_prop(char* prop, unsigned int prop_len, char* val, unsigned int val_len);
			void read_to_tag_end();
			bool read_tag_content(char* content, unsigned int content_len);
			// Retorna true se c for [A-Za-z_]
			bool read_identifier(char* id, unsigned int id_len);
			// Le um valor entre aspas.
			void read_quoted(char* val, unsigned int val_len);
			// Le uma entity (&amp; etc) e retorna o caractere que representa.
			char read_entity();
			// Le ate encontrar determinado caractere.
			void read_to_char(char ch, char* str, unsigned int str_len);
			// Le ate caractere, decodificando entities.
			void read_to_char_entdec(char ch, char* str, unsigned int str_len);

			// Nao assignable
			Reader& operator=(const Reader&);
		};

		// Escreve uma string codificando entities XML
		std::ostream& write_string_entenc(std::ostream& s, const char* str);
	} // namespace xml
} // namespace yuriks

#endif // YURIKS_T1_XML_H
