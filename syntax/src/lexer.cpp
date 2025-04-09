/* Copyright (C) 2024 Stan Vlad <vstan02@protonmail.com>
 *
 * This file is part of Thalia.
 *
 * Thalia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <cctype>
#include <cstddef>
#include <string_view>
#include <utility>
#include <algorithm>

#include "thalia-syntax/lexer.hpp"
#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  static const
    std::pair<std::string_view, token_type> keywords[14] = {
    { "void", token_type::Void },
    { "i8", token_type::I8 },
    { "i16", token_type::I16 },
    { "i32", token_type::I32 },
    { "i64", token_type::I64 },
    { "use", token_type::Use },
    { "global", token_type::Global },
    { "local", token_type::Local },
    { "return", token_type::Return },
    { "while", token_type::While },
    { "if", token_type::If },
    { "else", token_type::Else },
    { "mut", token_type::Mut },
    { "def", token_type::Def }
  };

  static const
    std::pair<std::string_view, token_type> symbols[41] = {
    { "$", token_type::Cast },
    { "-", token_type::Minus },
    { "+", token_type::Plus },
    { "*", token_type::Mul },
    { "/", token_type::Div },
    { "%", token_type::Mod },
    { "<", token_type::Less },
    { "<=", token_type::LessEqual },
    { ">", token_type::Grt },
    { ">=", token_type::GrtEqual },
    { "==", token_type::Equal },
    { "!=", token_type::NotEqual },
    { ">>", token_type::RShift },
    { "<<", token_type::LShift },
    { "!", token_type::LogNot },
    { "||", token_type::LogOr },
    { "&&", token_type::LogAnd },
    { "~", token_type::BitNot },
    { "&", token_type::BitAnd },
    { "|", token_type::BitOr },
    { "^", token_type::Xor },
    { "=", token_type::Assign },
    { "-=", token_type::MinusAssign },
    { "+=", token_type::PlusAssign },
    { "*=", token_type::MulAssign },
    { "/=", token_type::DivAssign },
    { "%=", token_type::ModAssign },
    { "|=", token_type::OrAssign },
    { "&=", token_type::AndAssign },
    { "^=", token_type::XorAssign },
    { ">>=", token_type::RshAssign },
    { "<<=", token_type::LshAssign },
    { "(", token_type::LParen },
    { ")", token_type::RParen },
    { "{", token_type::LBrace },
    { "}", token_type::RBrace },
    { "[", token_type::LBracket },
    { "]", token_type::RBracket },
    { ",", token_type::Comma },
    { ";", token_type::Semi },
    { ":", token_type::Colon }
  };

  template <typename Key, typename Value>
  static token_type find_in(
    std::pair<Key, Value> const* begin,
    std::pair<Key, Value> const* end,
    Key const& key,
    Value const& def_value
  ) {
    auto const* result = std::find_if(
			begin, end,
			[key](auto const& p) -> bool { return key == p.first; }
		);
		return result != end
      ? result->second : def_value;
  }

  extern token lexer::next() {
    skip_whitespace();
    if (_target.empty())
      return token(token_type::Eof, _target, _line, _col);

    if (std::isdigit(_target[0]))
      return next_number();

    if (_target[0] == '_' || std::isalpha(_target[0]))
      return next_kw_or_id();

    return next_symbol(3);
  }

  extern token lexer::next_number() {
    std::size_t pos = 0;
    std::size_t col = _col;
    while (pos < _target.size() && std::isdigit(_target[pos]))
      ++pos;

    auto value = advance(pos);
    _col += pos;
    return token(token_type::Int, value, _line, col);
  }

  extern token lexer::next_kw_or_id() {
    std::size_t pos = 0;
    std::size_t col = _col;
    while (pos < _target.size()
        && (_target[pos] == '_' || std::isalnum(_target[pos])))
      ++pos;

    auto value = advance(pos);
    auto type = find_in(
      std::begin(keywords), std::end(keywords),
      value, token_type::Id
    );

    _col += pos;
    return token(type, value, _line, col);
  }

  extern token lexer::next_symbol(std::size_t max_size) {
    if (max_size > _target.size())
      return next_symbol(max_size - 1);

    auto type = find_in(
      std::begin(symbols), std::end(symbols),
      _target.substr(0, max_size), token_type::Unknown
    );

    if (max_size > 1 && type == token_type::Unknown)
      return next_symbol(max_size - 1);

    std::size_t col = _col;
    _col += max_size;
    token token(type, advance(max_size), _line, col);

    if (type == token_type::Unknown)
      *_errors << error(error_type::UnknownCharacter, token);
    return token;
  }

  extern std::string_view lexer::advance(std::size_t npos) {
    auto value = _target.substr(0, npos);
    _target.remove_prefix(npos);
    return value;
  }

  extern void lexer::skip_whitespace() {
    std::size_t pos = 0;
    while (pos < _target.size() && std::isspace(_target[pos])) {
      if (_target[pos] == '\n') {
        _col = 1;
        ++_line;
      } else {
        ++_col;
      }
      ++pos;
    }
    _target.remove_prefix(pos);
  }
}

