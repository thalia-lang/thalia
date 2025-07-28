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
  /**
   * @brief A generic base class representing a syntax tree node.
   * @tparam Type An enum type used to identify the kind of the node.
   */
  template <typename Type>
  class node {
    public:
      /**
       * @brief Constructs a node with the specified type.
       * @param type The type of the node.
       */
      node(Type type)
        : _type { type } {}

      /**
       * @brief Virtual destructor for safe polymorphic deletion.
       */
      virtual ~node() = default;

      /**
       * @brief Checks if the node is of a specific type.
       * @param type The type to check against.
       * @return True if the node's type matches.
       */
      auto is(Type type) const -> bool
        { return _type == type; }

      /**
       * @brief Checks if the node is one of the given types.
       * @param types A list of types to check against.
       * @return True if the node's type is in the list.
       */
      auto is(std::initializer_list<Type> types) const -> bool
        { return types.end() != std::find(types, _type); }

      /**
       * @brief Gets the type of the node.
       * @return The node's type.
       */
      auto type() const -> Type
        { return _type; }

    private:
      Type _type;
  };
}

#endif // _THALIA_SYNTAX_NODE_

