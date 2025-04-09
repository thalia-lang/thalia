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

#ifndef _THALIA_SYNTAX_TOKEN_
#define _THALIA_SYNTAX_TOKEN_

#include <cstddef>
#include <ostream>
#include <initializer_list>
#include <string_view>

namespace thalia::syntax {
  enum class token_type {
    Unknown,
    Eof,

    Int,
    Id,

    Void,
    I8,
    I16,
    I32,
    I64,

    Use,
    Global,
    Local,
    Return,
    While,
    If,
    Else,
    Mut,
    Def,

    Cast,
    Minus,
    Plus,
    Mul,
    Div,
    Mod,
    Less,
    LessEqual,
    Grt,
    GrtEqual,
    Equal,
    NotEqual,
    RShift,
    LShift,
    LogNot,
    LogOr,
    LogAnd,
    BitNot,
    BitAnd,
    BitOr,
    Xor,
    Assign,
    MinusAssign,
    PlusAssign,
    MulAssign,
    DivAssign,
    ModAssign,
    AndAssign,
    OrAssign,
    XorAssign,
    RshAssign,
    LshAssign,
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Comma,
    Semi,
    Colon
  };

  class token {
    public:
      explicit token(
        token_type type = token_type::Unknown,
        std::string_view value = {},
        std::size_t line = 1,
        std::size_t col = 1
      ) : _type(type)
        , _value(value)
        , _line(line)
        , _col(col) {}

      bool is(token_type type) const { return _type == type; }
      bool eof() const { return is(token_type::Eof); }
      bool unknown() const { return is(token_type::Unknown); }

      token_type type() const { return _type; }
      std::string_view value() const { return _value; }
      std::size_t line() const { return _line; }
      std::size_t col() const { return _col; }

      bool is(std::initializer_list<token_type> types) const;

      friend std::ostream& operator<<(std::ostream& os, token const& token);

    private:
      token_type _type;
		  std::string_view _value;
		  std::size_t _line;
      std::size_t _col;
  };

  extern std::ostream& operator<<(std::ostream& os, token_type type);
}

#endif // _THALIA_SYNTAX_TOKEN_

