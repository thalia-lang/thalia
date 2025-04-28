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

#ifndef _THALIA_STMT_VIEW_
#define _THALIA_STMT_VIEW_

#include <memory>
#include <ostream>
#include <string>

#include <thalia-syntax/stmts.hpp>

namespace thalia {
  class stmt_view
    : public syntax::stmt_visitor<std::ostream&, std::ostream&> {
    public:
      stmt_view(
        std::shared_ptr<syntax::statement> const& node,
        std::size_t deep = 0
      ) : syntax::stmt_visitor<std::ostream&, std::ostream&>(node)
        , _deep(deep)
        , _space(deep * 2, ' ') {}

      friend auto operator<<(std::ostream&, stmt_view& view) -> std::ostream&;

    protected:
      auto visit_stmt_block(std::ostream& os) -> std::ostream& override;
      auto visit_stmt_return(std::ostream& os) -> std::ostream& override;
      auto visit_stmt_expr(std::ostream& os) -> std::ostream& override;
      auto visit_stmt_local(std::ostream& os) -> std::ostream& override;
      auto visit_stmt_if(std::ostream& os) -> std::ostream& override;
      auto visit_stmt_while(std::ostream& os) -> std::ostream& override;

    private:
      std::size_t _deep;
      std::string _space;
  };

  inline auto operator<<(std::ostream& os, stmt_view& view)
    -> std::ostream& {
    return view.visit_stmt(os);
  }
}

#endif // _THALIA_STMT_VIEW_

