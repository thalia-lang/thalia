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

#include "thalia-syntax/stmts.hpp"

#include "stmt_view.hpp"
#include "expr_view.hpp"

namespace thalia {
  extern auto stmt_view::visit_stmt_return(std::ostream& os)
    -> std::ostream& {
      auto root = std::static_pointer_cast<syntax::stmt_return>(_node);
      auto value = expr_view(root->value(), _deep + 1);
      return os
      << _space << "StmtReturn {\n"
      << value << "\n"
      << _space << "}";
  }

  extern auto stmt_view::visit_stmt_expr(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::stmt_expr>(_node);
    auto value = expr_view(root->value(), _deep + 1);
    return os
      << _space << "StmtExpr {\n"
      << value << "\n"
      << _space << "}";
  }

  extern auto stmt_view::visit_stmt_local(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::stmt_local>(_node);
    return os
      << _space << "StmtLocal { " << " }";
  }

  extern auto stmt_view::visit_stmt_block(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::stmt_block>(_node);

    os << _space << "StmtBlock {\n";
    for (const auto& node: root->content()) {
      auto view = stmt_view(node, _deep + 1);
      os << view << "\n";
    }
    return os
      << _space << "}";
  }

  extern auto stmt_view::visit_stmt_if(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::stmt_if>(_node);
    auto condition = expr_view(root->condition(), _deep + 1);
    auto main_body = stmt_view(root->main_body(), _deep + 1);

    os
      << _space << "StmtIf {\n"
      << condition << "\n"
      << main_body << "\n";
    if (root->else_body()) {
      auto else_body = stmt_view(root->else_body(), _deep + 1);
      os << else_body << "\n";
    }

    return os
      << _space << "}";
  }

  extern auto stmt_view::visit_stmt_while(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::stmt_while>(_node);
    auto condition = expr_view(root->condition(), _deep + 1);
    auto body = stmt_view(root->body(), _deep + 1);
    return os
      << _space << "StmtWhile {\n"
      << condition << "\n"
      << body << "\n"
      << _space << "}";
  }
}

