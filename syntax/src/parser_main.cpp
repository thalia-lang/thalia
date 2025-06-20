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

#include "thalia-syntax/parser.hpp"

namespace thalia::syntax {
  extern auto parser::parse()
    -> std::vector<std::shared_ptr<statement>> {
    auto result = std::vector<std::shared_ptr<statement>> {};
    while (!eof())
      result.push_back(parse_statement());
    return result;
  }

  extern auto parser::advance()
    -> token const& {
    auto prev = _current;
    if (eof())
      _errors << error { error_type::UnexpectedEof, *prev };
    else ++_current;
    return *prev;
  }

  extern auto parser::consume(
    std::initializer_list<token_type> types,
    error_type type
  ) -> token const& {
    if (!_current->is(types))
      throw error { type, *_current };
    return advance();
  }

  extern auto parser::skip_until(std::initializer_list<token_type> types)
    -> void {
    while (!eof() && !match(types))
      ++_current;
  }
}

