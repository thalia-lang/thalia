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

#include <memory>
#include <vector>

namespace thalia::syntax {
	template <typename T>
	using node = std::shared_ptr<T>;

	template <typename T>
	using nodes = std::vector<node<T>>;

	template <typename T>
	inline node<T> make_node(T const& value) {
		return std::make_shared<T>(value);
	}
}

#endif // _THALIA_SYNTAX_NODE_

