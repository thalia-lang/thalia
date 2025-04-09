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
  error_queue& error_queue::operator<<(
    const syntax::lexer::error& error
  ) {
    using t = syntax::lexer::error_type;

    _os << "[ERROR]: ";
    switch (error.type) {
      case t::UnknownCharacter:
        _os << "Unknown character";
    }

    _os
      << " '" << error.target.value()
      << "'\n    ---> on line " << error.target.line()
      << ", column " << error.target.col() << ".\n";
    return *this;
  }
}

