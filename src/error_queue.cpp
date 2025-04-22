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

#include "error_queue.hpp"

namespace thalia {
  extern auto error_queue::operator<<(
    syntax::lexer::error const& error
  ) -> error_queue& {
    using t = syntax::lexer::error_type;
    _os << "[ERROR]: ";
    switch (error.type) {
      case t::UnknownCharacter:
        _os << "Unknown character"; break;
    }

    _os
      << " '" << error.target.value()
      << "'\n    ---> on line " << error.target.line()
      << ", column " << error.target.col() << ".\n";

    ++_size;
    if (_max_size != 0 && _size >= _max_size) {
      _os << "[INFO]: Too many errors, stopping now.\n";
      std::exit(EXIT_FAILURE);
    }

    return *this;
  }

  extern auto error_queue::operator<<(
    syntax::parser::error const& error
  ) -> error_queue& {
    using t = syntax::parser::error_type;
    _os << "[ERROR]: ";
    switch (error.type) {
      case t::UnexpectedEof:
        _os << "Unexpected end of the file"; break;
      case t::ExpectedDataType:
        _os << "Expected a data type"; break;
      case t::ExpectedRParen:
        _os << "Expected '(' after expression"; break;
      case t::ExpectedPrimary:
        _os << "Expected a primary expression"; break;
    }

    _os
      << "\n    ---> on value '" << error.target.value()
      << "'\n    ---> on line " << error.target.line()
      << ", column " << error.target.col() << ".\n";

    ++_size;
    if (_max_size != 0 && _size >= _max_size) {
      _os << "[INFO]: Too many errors, stopping now.\n";
      std::exit(EXIT_FAILURE);
    }

    return *this;
  }
}

