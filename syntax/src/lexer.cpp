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
#include <initializer_list>
#include <string_view>
#include <utility>
#include <algorithm>
#include <vector>

#include "thalia-syntax/lexer.hpp"
#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  static auto const keywords
    = std::initializer_list<std::pair<std::string_view, token_type>> {
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

  static auto const symbols
    = std::initializer_list<std::pair<std::string_view, token_type>> {
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
  static auto find_in(
    std::pair<Key, Value> const* begin,
    std::pair<Key, Value> const* end,
    Key const& key,
    Value const& def_value
  ) -> Value {
    auto const* result = std::find_if(
			begin, end,
			[key](auto const& p) -> bool { return key == p.first; }
		);
		return result != end
      ? result->second : def_value;
  }

  extern auto lexer::scan_all()
    -> std::vector<token> {
    auto tokens = std::vector<token> {};
    auto token = syntax::token {};
    do {
      token = scan_next();
      if (!token.unknown())
        tokens.push_back(token);
    } while(!token.eof());
    return tokens;
  }

  extern auto lexer::scan_next()
    -> token {
    skip_whitespace();
    if (_target.empty())
      return token(token_type::Eof, _target, _line, _col);

    if (std::isdigit(_target[0]))
      return scan_number();

    if (_target[0] == '_' || std::isalpha(_target[0]))
      return scan_kw_or_id();

    return scan_symbol(3);
  }

  extern auto lexer::scan_number()
    -> token {
    auto pos = std::size_t { 0 };
    auto col = _col;
    while (pos < _target.size() && std::isdigit(_target[pos]))
      ++pos;

    auto value = advance(pos);
    _col += pos;
    return token { token_type::Int, value, _line, col };
  }

  extern auto lexer::scan_kw_or_id()
    -> token {
    auto pos = std::size_t { 0 };
    auto col = _col;
    while (pos < _target.size()
        && (_target[pos] == '_' || std::isalnum(_target[pos])))
      ++pos;

    auto value = advance(pos);
    auto type = find_in(
      std::begin(keywords), std::end(keywords),
      value, token_type::Id
    );

    _col += pos;
    return token { type, value, _line, col };
  }

  extern auto lexer::scan_symbol(std::size_t max_size)
    -> token {
    if (max_size > _target.size())
      return scan_symbol(max_size - 1);

    auto type = find_in(
      std::begin(symbols), std::end(symbols),
      _target.substr(0, max_size), token_type::Unknown
    );

    if (max_size > 1 && type == token_type::Unknown)
      return scan_symbol(max_size - 1);

    auto col = _col;
    _col += max_size;
    auto target = token { type, advance(max_size), _line, col };

    if (type == token_type::Unknown)
      _errors << error { error_type::UnknownCharacter, target };
    return target;
  }

  extern auto lexer::advance(std::size_t npos)
    -> std::string_view {
    auto value = _target.substr(0, npos);
    _target.remove_prefix(npos);
    return value;
  }

  extern auto lexer::skip_whitespace()
    -> void {
    auto pos = std::size_t { 0 };
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

