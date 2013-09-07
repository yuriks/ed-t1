// xml.cpp
// Leitor XML baseado em eventos. Semi-completo.
// Estruturas de Dados - T1
// 2911524 - Yuri Kunde Schlesner
#include "xml.h"

#include "util.h"

#include <cctype>
#include <cassert>
#include <cstring>
// Usado apenas para inicializar a excecao (std::string eh usado por std::runtime_error)
#include <string>

namespace yuriks
{
	namespace xml
	{
		namespace
		{
			// Retorna se c for [A-Za-z_]
			bool isidc(char c)
			{
				return c == '_' || std::isalpha(c);
			}
		} // namespace

		Reader::Reader(std::istream& f)
			: used(false), f(f), cur_line(1)
		{
		}

		void Reader::read_xml(EventConsumer& consumer)
		{
			assert(used == false);

			const char* tag_stack[max_tag_depth];
			unsigned int tag_stack_top = 0;

			try
			{
				char tag[max_tag_len];
				while (read_next_tag(tag, max_tag_len))
				{
					if (tag[0] == '/')
					{
						if (tag_stack_top == 0 || std::strcmp(tag_stack[tag_stack_top-1], tag+1) != 0)
							throw ReadingException("Fechamento de tag invalido.");
						consumer.tag_close(tag+1);

						delete[] tag_stack[tag_stack_top-1];
						--tag_stack_top;
					}
					else
					{
						if (tag_stack_top >= max_tag_depth)
							throw ReadingException("Profundidade maxima de tags excedida.");
						tag_stack[tag_stack_top] = strdup(tag);
						++tag_stack_top;

						consumer.tag_open(tag);

						char prop[max_prop_name_len];
						char prop_val[max_prop_value_len];
						while (read_next_prop(prop, max_prop_name_len, prop_val, max_prop_value_len))
						{
							consumer.tag_prop(prop, prop_val);
						}
					}
					read_to_tag_end();

					char content[max_tag_content_len];
					if (read_tag_content(content, max_tag_content_len))
					{
						consumer.tag_text_node(content);
					}
				}
			}
			catch (const std::exception&)
			{
				while (tag_stack_top > 0)
				{
					delete[] tag_stack[tag_stack_top-1];
					--tag_stack_top;
				}
				throw;
			}
		}

		unsigned int Reader::line_num() const
		{
			return cur_line;
		}

		std::istream& Reader::getch(char& c)
		{
			if (!f.get(c) && !f.eof())
				throw ReadingException("Erro de leitura.");
			if (c == '\n')
				++cur_line;
			return f;
		}

		void Reader::skip_whitespace()
		{
			char c;
			while (getch(c))
			{
				if (!std::isspace(c))
				{
					f.putback(c);
					return;
				}
			}

			if (!f.eof())
				throw ReadingException("Erro de leitura.");
		}

		bool Reader::read_next_tag(char* tag, unsigned int tag_len)
		{
			skip_whitespace();

			char c;
			if (!getch(c))
			   return false;
			if (c != '<')
				throw ReadingException("Esperava '<'.");

			if (f.peek() == '/')
			{
				getch(tag[0]);
				++tag;
				--tag_len;
			}

			read_identifier(tag, tag_len);
			return true;
		}

		bool Reader::read_next_prop(char* prop, unsigned int prop_len, char* val, unsigned int val_len)
		{
			skip_whitespace();

			if (read_identifier(prop, prop_len))
			{
				char c;
				if (!getch(c) || c != '=')
					throw ReadingException("Esperava '='.");
				read_quoted(val, val_len);
				return true;
			}
			else
			{
				return false;
			}
		}

		void Reader::read_to_tag_end()
		{
			skip_whitespace();
			char c;
			if (!getch(c) || (c != '/' && c != '>'))
			{
				if (f && c == '/')
					throw ReadingException("Tags auto-fechantes nao sao suportadas.");
				else
					throw ReadingException("Esperava '>'.");
			}
		}

		bool Reader::read_tag_content(char* content, unsigned int content_len)
		{
			skip_whitespace();
			if (f.peek() == '<' || f.eof())
				return false;
			else
			{
				read_to_char_entdec('<', content, content_len);
				f.putback('<');
				return true;
			}
		}

		// Le um identifier em id, tamanho maximo id_len
		bool Reader::read_identifier(char* id, unsigned int id_len)
		{
			unsigned int i = 0;
			char c;
			while (getch(c) && isidc(c))
			{
				if (i < id_len-1)
					id[i++] = c;
			}
			id[i] = '\0';

			// Repoe ultimo caractere lido
			if (f)
				f.putback(c);

			// Retorna se leu alguma coisa
			return i != 0;
		}

		void Reader::read_quoted(char* val, unsigned int val_len)
		{
			char c;
			if (!getch(c) || c != '"')
				throw ReadingException("Esperava '\"'.");
			read_to_char_entdec('"', val, val_len);

		}

		char Reader::read_entity()
		{
			static const struct { const char* ent; char ch; }
			entity_table[] =
			{
				{"amp",  '&'},
				{"lt",   '<'},
				{"gt",   '>'},
				{"quot", '"'},
				{"apos", '\''},
				{0,      '\0'}
			};

			char buf[5];
			read_to_char(';', buf, 5);

			unsigned int i = 0;
			while (entity_table[i].ent != 0)
			{
				if (std::strcmp(buf, entity_table[i].ent) == 0)
					return entity_table[i].ch;
				++i;
			}

			throw ReadingException("Unknown entity.");
		}

		void Reader::read_to_char(char ch, char* str, unsigned int str_len)
		{
			unsigned int i = 0;
			char c;
			while (getch(c) && c != ch)
			{
				if (i < str_len-1)
					str[i++] = c;
			}
			if (!f)
				throw ReadingException(std::string("Esperava '") + ch + "'.");

			str[i] = '\0';
		}

		void Reader::read_to_char_entdec(char ch, char* str, unsigned int str_len)
		{
			unsigned int i = 0;
			char c;
			while (getch(c) && c != ch)
			{
				if (c == '&')
					c = read_entity();
				if (i < str_len-1)
					str[i++] = c;
			}
			if (!f)
				throw ReadingException(std::string("Esperava '") + ch + "'.");

			str[i] = '\0';
		}

		std::ostream& write_string_entenc(std::ostream& s, const char* str)
		{
			for (; *str != '\0'; ++str)
			{
				switch (*str)
				{
				case '&':
					s << "&amp;"; break;
				case '<':
					s << "&lt;"; break;
				case '>':
					s << "&gt;"; break;
				case '"':
					s << "&quot;"; break;
				case '\'':
					s << "&apos;"; break;
				default:
					s << *str; break;
				}
			}

			return s;
		}
	} // namespace xml
} // namespace yuriks
