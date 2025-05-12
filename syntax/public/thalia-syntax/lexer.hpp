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
  class lexer {
    public:
      enum class error_type {
        UnknownCharacter
      };

      using error = error<error_type, token>;
      using error_queue = error_queue<error_type, token>;

    public:
      lexer(
        error_queue& equeue,
        std::string_view target,
        std::size_t init_line = 1,
        std::size_t init_col = 1
      ) : _errors(equeue)
        , _target(target)
        , _line(init_line)
        , _col(init_col) {}

      lexer(
        error_queue& equeue,
        std::string::const_iterator begin,
        std::string::const_iterator end,
        std::size_t init_line = 1,
        std::size_t init_col = 1
      ) : lexer(equeue, std::string_view(begin, end), init_line, init_col) {}

      lexer(
        error_queue& equeue,
        std::string const& target,
        std::size_t init_line = 1,
        std::size_t init_col = 1
      ) : lexer(equeue, std::string_view(target), init_line, init_col) {}

      auto scan_next() -> token;
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

