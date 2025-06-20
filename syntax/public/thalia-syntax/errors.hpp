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
  template <typename Type, typename Target>
  struct error {
    Type type;
    Target target;

    error(Type type)
      : type { type }, target {} {}

    error(Type type, Target const& target)
      : type { type }, target { target } {}

    error(Type type, Target&& target)
      : type { type }, target { std::move(target) } {}
  };

  template <typename Type, typename Target>
  class error_queue {
    public:
      virtual ~error_queue() = default;

      virtual auto operator<<(error<Type, Target> const& error)
        -> error_queue& = 0;
  };
}

#endif // _THALIA_SYNTAX_ERRORS_

