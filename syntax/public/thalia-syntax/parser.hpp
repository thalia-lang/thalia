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

#ifndef _THALIA_SYNTAX_PARSER_
#define _THALIA_SYNTAX_PARSER_

#include <memory>

#include "errors.hpp"
#include "lexer.hpp"

namespace thalia::syntax {
  class parser {
    public:
      enum class error_type {};

      using error = error<error_type, token>;
      using error_queue = error_queue<error_type, token>;

    public:
      explicit parser(
        std::shared_ptr<error_queue> const& equeue,
        lexer const& lexer
      ) : _errors(equeue)
        , _lexer(lexer) {}

    private:
      std::shared_ptr<error_queue> _errors;
      lexer _lexer;
  };
}

#endif // _THALIA_SYNTAX_PARSER_

