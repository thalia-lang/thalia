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

#ifndef _THALIA_ERROR_QUEUE_
#define _THALIA_ERROR_QUEUE_

#include <ostream>

#include <thalia-syntax/lexer.hpp>
#include <thalia-syntax/parser.hpp>

namespace thalia {
  class error_queue
    : public syntax::lexer::error_queue
    , public syntax::parser::error_queue {
    public:
      explicit error_queue(std::ostream& os, std::size_t max_size = 0)
        : _os(os)
        , _max_size(max_size)
        , _size(0) {}

      auto empty() const -> bool
        { return _max_size == 0; }
      auto full() const -> bool
        { return _max_size && _size >= _max_size; }

      auto operator<<(syntax::lexer::error const& error)
        -> error_queue& override;
      auto operator<<(syntax::parser::error const& error)
        -> error_queue& override;

    private:
      std::ostream& _os;
      std::size_t _max_size;
      std::size_t _size;
  };
}

#endif // _THALIA_ERROR_QUEUE_

