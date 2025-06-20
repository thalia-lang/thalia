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
  /**
   * @brief Represents the different kinds of tokens in the Thalia language.
   */
  enum class token_type {
    Unknown,
    Eof,

    Int,
    Id,

    // Keywords
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

    // Operators
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

    // Punctuation
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

  /**
   * @brief Represents a lexical token with type, source value, and position.
   */
  class token {
    public:
      /**
       * @brief Constructs a token with the given properties.
       *
       * @param type The type of the token.
       * @param value The raw text value associated with the token.
       * @param line The line number where the token was found.
       * @param col The column number where the token starts.
       */
      token(
        token_type type = token_type::Unknown,
        std::string_view value = {},
        std::size_t line = 1,
        std::size_t col = 1
      ) : _type { type }
        , _value { value }
        , _line { line }
        , _col { col } {}

      /**
       * @brief Checks if the token is the end-of-file token.
       * @return True if token type is Eof.
       */
      auto eof() const -> bool
        { return is(token_type::Eof); }

      /**
       * @brief Checks if the token is of type Unknown.
       * @return True if token type is Unknown.
       */
      auto unknown() const -> bool
        { return is(token_type::Unknown); }

      /**
       * @brief Checks if the token is of the given type.
       * @param type The token type to compare against.
       * @return True if token is of the given type.
       */
      auto is(token_type type) const -> bool
        { return _type == type; }

      /**
       * @brief Checks if the token is one of several types.
       * @param types A list of token types to check against.
       * @return True if token matches any of the types.
       */
      auto is(std::initializer_list<token_type> types) const -> bool;

      /**
       * @brief Gets the token's type.
       * @return The type of the token.
       */
      auto type() const -> token_type
        { return _type; }

      /**
       * @brief Gets the token's string value.
       * @return The text associated with the token.
       */
      auto value() const -> std::string_view
        { return _value; }

      /**
       * @brief Gets the line number where the token appears.
       * @return Line number.
       */
      auto line() const -> std::size_t
        { return _line; }

      /**
       * @brief Gets the column number where the token starts.
       * @return Column number.
       */
      auto col() const -> std::size_t
        { return _col; }

      /**
       * @brief Gets the size of the token's string value.
       * @return The size associated with the token's value.
       */
      auto size() const -> std::size_t
        { return _value.size(); }

      /**
       * @brief Prints a formatted token to the output stream.
       *
       * @param os Output stream.
       * @param token The token to print.
       * @return Reference to the output stream.
       */
      friend auto operator<<(std::ostream& os, token const& token)
        -> std::ostream&;

    private:
      token_type _type;
		  std::string_view _value;
		  std::size_t _line;
      std::size_t _col;
  };

  /**
   * @brief Prints a token_type to an output stream.
   *
   * @param os Output stream.
   * @param type Token type to print.
   * @return Reference to the output stream.
   */
  extern auto operator<<(std::ostream& os, token_type type) -> std::ostream&;
}

#endif // _THALIA_SYNTAX_TOKEN_

