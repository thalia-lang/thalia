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

#include "expr_view.hpp"

namespace thalia {
  extern auto expr_view::visit_expr_assign(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::expr_assign>(_node);
    auto target = expr_view(root->target(), _deep + 1);
    auto value = expr_view(root->value(), _deep + 1);
    return os
      << _space << "ExprAssign {\n  "
      << _space << root->operation() << ",\n"
      << target << ",\n"
      << value << "\n"
      << _space << "}";
  }

  extern auto expr_view::visit_expr_binary(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::expr_binary>(_node);
    auto lhs = expr_view(root->lhs(), _deep + 1);
    auto rhs = expr_view(root->rhs(), _deep + 1);
    return os
      << _space << "ExprBinary {\n  "
      << _space << root->operation() << ",\n"
      << lhs << ",\n"
      << rhs << "\n"
      << _space << "}";
  }

  extern auto expr_view::visit_expr_unary(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::expr_unary>(_node);
    auto value = expr_view(root->value(), _deep + 1);
    return os
      << _space << "ExprUnary {\n  "
      << _space << root->operation() << ",\n"
      << value << "\n"
      << _space << "}";
  }

  extern auto expr_view::visit_expr_paren(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::expr_paren>(_node);
    auto value = expr_view(root->value(), _deep + 1);
    return os
      << _space << "ExprParen {\n"
      << value << "\n"
      << _space << "}";
  }

  extern auto expr_view::visit_expr_base_lit(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::expr_base_lit>(_node);
    return os
      << _space << "ExprBaseLit { " << root->target() << " }";
  }

  extern auto expr_view::visit_expr_id(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::expr_id>(_node);
    return os
      << _space << "ExprId { " << root->target() << " }";
  }

  extern auto expr_view::visit_expr_data_type(std::ostream& os)
    -> std::ostream& {
    auto root = std::static_pointer_cast<syntax::expr_data_type>(_node);
    return os
      << _space << "ExprDataType { " << root->target() << " }";
  }
}

