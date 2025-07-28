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

#ifndef _THALIA_SYNTAX_ERRORS_
#define _THALIA_SYNTAX_ERRORS_

#include <utility>

namespace thalia::syntax {
  /**
   * @brief Represents a syntax-related error with a type and an associated target.
   * @tparam Type The enum type that represents the kind of error.
   * @tparam Target The source or object the error refers to (a token, AST node, etc).
   */
  template <typename Type, typename Target>
  struct error {
    Type type;
    Target target;

    /**
     * @brief Constructs an error with only a type; target is default-initialized.
     * @param type The type of the error.
     */
    error(Type type)
      : type { type }, target {} {}

    /**
     * @brief Constructs an error with a type and a const reference to a target.
     * @param type The type of the error.
     * @param target The target where the error occurred.
     */
    error(Type type, Target const& target)
      : type { type }, target { target } {}

    /**
     * @brief Constructs an error with a type and a movable target.
     * @param type The type of the error.
     * @param target The target to move into the error.
     */
    error(Type type, Target&& target)
      : type { type }, target { std::move(target) } {}
  };

  /**
   * @brief Abstract interface for managing and collecting syntax errors.
   * @tparam Type The error type enum.
   * @tparam Target The target where error the error occurred.
   *
   * This class provides a mechanism to report errors without knowing how they are stored or displayed.
   */
  template <typename Type, typename Target>
  class error_queue {
    public:
      /**
       * @brief Virtual destructor for safe polymorphic deletion.
       */
      virtual ~error_queue() = default;

      /**
       * @brief Adds an error to the queue.
       * @param error The error to be added.
       * @return Reference to the queue for chaining.
       */
      virtual auto operator<<(error<Type, Target> const& error)
        -> error_queue& = 0;
  };
}

#endif // _THALIA_SYNTAX_ERRORS_

