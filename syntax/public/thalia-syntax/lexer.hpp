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

#ifndef _THALIA_SYNTAX_LEXER_
#define _THALIA_SYNTAX_LEXER_

#include <cstddef>
#include <string_view>
#include <vector>

#include "errors.hpp"
#include "token.hpp"

namespace thalia::syntax {
  /**
   * @brief Performs lexical analysis on a source string.
   *
   * The lexer reads a character stream and produces a sequence of tokens.
   * Errors encountered during lexing are reported to an external error queue.
   */
  class lexer {
    public:
      /**
       * @brief Describes the types of errors the lexer can emit.
       */
      enum class error_type {
        UnknownCharacter
      };

      /**
       * @brief Alias for a lexer-specific error.
       */
      using error = error<error_type, token>;

      /**
       * @brief Alias for the lexer’s error queue interface.
       */
      using error_queue = error_queue<error_type, token>;

    public:
      /**
       * @brief Constructs a lexer from a string view.
       * @param equeue Reference to an error queue used for reporting.
       * @param target The input source code as a string view.
       * @param init_line The initial line number for the cursor (default is 1).
       * @param init_col The initial column number for the cursor (default is 1).
       */
      lexer(
        error_queue& equeue,
        std::string_view target,
        std::size_t init_line = 1,
        std::size_t init_col = 1
      ) : _errors { equeue }
        , _target { target }
        , _line { init_line }
        , _col { init_col } {}

      /**
       * @brief Constructs a lexer from a pair of string iterators.
       * @param equeue Reference to an error queue used for reporting.
       * @param begin Iterator pointing to the beginning of the input string.
       * @param end Iterator pointing to the end of the input string.
       * @param init_line The initial line number for the cursor (default is 1).
       * @param init_col The initial column number for the cursor (default is 1).
       */
      lexer(
        error_queue& equeue,
        std::string::const_iterator begin,
        std::string::const_iterator end,
        std::size_t init_line = 1,
        std::size_t init_col = 1
      ) : lexer {
          equeue, std::string_view { begin, end },
          init_line, init_col
        } {}

      /**
       * @brief Constructs a lexer from a full std::string.
       * @param equeue Reference to an error queue used for reporting.
       * @param target The input source code as a full string.
       * @param init_line The initial line number for the cursor (default is 1).
       * @param init_col The initial column number for the cursor (default is 1).
       */
      lexer(
        error_queue& equeue,
        std::string const& target,
        std::size_t init_line = 1,
        std::size_t init_col = 1
      ) : lexer {
          equeue, std::string_view { target },
          init_line, init_col
        } {}

      /**
       * @brief Scans and returns the next token from the input.
       * @return The next token in the input stream.
       *
       * Advances the cursor past the returned token. If the end of input is reached, a token of type `Eof` is returned.
       */
      auto scan_next() -> token;

      /**
       * @brief Scans the entire input and returns all tokens except those of type `Unknown`.
       * @return A vector containing all tokens found in the input.
       *
       * This method repeatedly calls `scan_next()` until an EOF token is reached.
       */
      auto scan_all() -> std::vector<token>;

    private:
      auto skip_whitespace() -> void;
      auto advance(std::size_t npos) -> std::string_view;
      auto scan_number() -> token;
      auto scan_kw_or_id() -> token;
      auto scan_symbol(std::size_t max_size = 1) -> token;

    private:
      error_queue& _errors;
      std::string_view _target;
      std::size_t _line;
      std::size_t _col;
  };
}

#endif // _THALIA_SYNTAX_LEXER_

