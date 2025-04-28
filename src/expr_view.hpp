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

#ifndef _THALIA_EXPR_VIEW_
#define _THALIA_EXPR_VIEW_

#include <memory>
#include <ostream>
#include <string>

#include <thalia-syntax/exprs.hpp>

namespace thalia {
  class expr_view
    : public syntax::expr_visitor<std::ostream&, std::ostream&> {
    public:
      expr_view(
        std::shared_ptr<syntax::expression> const& node,
        std::size_t deep = 0
      ) : syntax::expr_visitor<std::ostream&, std::ostream&>(node)
        , _deep(deep)
        , _space(deep * 2, ' ') {}

      friend auto operator<<(std::ostream&, expr_view& view) -> std::ostream&;

    protected:
      auto visit_expr_assign(std::ostream& os) -> std::ostream& override;
      auto visit_expr_binary(std::ostream& os) -> std::ostream& override;
      auto visit_expr_unary(std::ostream& os) -> std::ostream& override;
      auto visit_expr_paren(std::ostream& os) -> std::ostream& override;
      auto visit_expr_base_lit(std::ostream& os) -> std::ostream& override;
      auto visit_expr_id(std::ostream& os) -> std::ostream& override;
      auto visit_expr_data_type(std::ostream& os) -> std::ostream& override;

    private:
      std::size_t _deep;
      std::string _space;
  };

  inline auto operator<<(std::ostream& os, expr_view& view)
    -> std::ostream& {
    return view.visit_expr(os);
  }
}

#endif // _THALIA_EXPR_VIEW_

