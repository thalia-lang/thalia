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

#ifndef _THALIA_SYNTAX_NODE_
#define _THALIA_SYNTAX_NODE_

#include <algorithm>
#include <initializer_list>

namespace thalia::syntax {
  template <typename Type>
  class node {
    public:
      explicit node(Type type)
        : _type(type) {}

      virtual ~node() = default;

      bool is(Type type) const { return _type == type; }
      Type type() const { return _type; }

      bool is(std::initializer_list<Type> types) const;

    private:
      Type _type;
  };

  template <typename Type>
  extern bool node<Type>::is(
    std::initializer_list<Type> types
  ) const {
    auto const* result = std::find(types.begin(), types.end(), _type);
    return result != types.end();
  }
}

#endif // _THALIA_SYNTAX_NODE_

