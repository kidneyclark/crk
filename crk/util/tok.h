#ifndef CRK_UTIL_TOK_H
#define CRK_UTIL_TOK_H

#include "../com/types.h"
#include "../util/tabula.h"

namespace crk::tok
{
	struct Token
	{
		u64 type;
		Tabula::pos_type str;
	};

	struct TokenizedData
	{
		crk::vector<Token> tokens;
		crk::Tabula strings;
		const char *original_data;
		size_t data_length;
	};

	class Lexer
	{
	public:
		TokenizedData Tokenize(const char *in_text, size_t in_length)
		{
			m_Data = in_text;
			m_Length = in_length;
			m_Tokens.clear();
			m_Strings.Clear();

			Token tok;
			while (NextToken(tok))
			{
				m_Tokens.push_back(tok);
			}

			TokenizedData out = {
				.tokens = m_Tokens,
				.strings = m_Strings,
				.original_data = m_Text,
				.original_data = m_Text,
			}
		}

	private:
		virtual bool NextToken(Token &tok) = 0;

		Token CreateToken(u64 type, const char *str, size_t str_len)
		{
			Token tok;
			tok.type = type;
			tok.str = m_Strings.AddStringSafe(str, str_len);
			return tok;
		}

		crk::vector<Token> m_Tokens;
		size_t m_Pos {0};
		size_t m_Length {0};
		const char *m_Data {nullptr};
		crk::Tabula m_Strings;
	};


}

#endif