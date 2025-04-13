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

#include "ast_view.hpp"

namespace thalia {
  ast_view::output_type ast_view::visit_expr_assign(ast_view::input_type os) {
    auto root = (std::shared_ptr<syntax::expr_assign> const&)_node;
    ast_view target(root->target(), _deep + 1);
    ast_view value(root->value(), _deep + 1);
    return os
      << _space << "ExprAssign {\n  "
      << _space << root->operation() << ",\n"
      << target << ",\n"
      << value << "\n"
      << _space << "}";
  }

  ast_view::output_type ast_view::visit_expr_binary(ast_view::input_type os) {
    auto root = (std::shared_ptr<syntax::expr_binary> const&)_node;
    ast_view lhs(root->lhs(), _deep + 1);
    ast_view rhs(root->rhs(), _deep + 1);
    return os
      << _space << "ExprBinary {\n  "
      << _space << root->operation() << ",\n"
      << lhs << ",\n"
      << rhs << "\n"
      << _space << "}";
  }

  ast_view::output_type ast_view::visit_expr_unary(ast_view::input_type os) {
    auto root = (std::shared_ptr<syntax::expr_unary> const&)_node;
    ast_view value(root->value(), _deep + 1);
    return os
      << _space << "ExprUnary {\n  "
      << _space << root->operation() << ",\n"
      << value << "\n"
      << _space << "}";
  }

  ast_view::output_type ast_view::visit_expr_paren(ast_view::input_type os) {
    auto root = (std::shared_ptr<syntax::expr_paren> const&)_node;
    ast_view value(root->value(), _deep + 1);
    return os
      << _space << "ExprParen {\n  "
      << value << "\n"
      << _space << "}";
  }

  ast_view::output_type ast_view::visit_expr_base_lit(ast_view::input_type os) {
    auto root = (std::shared_ptr<syntax::expr_base_lit> const&)_node;
    return os
      << _space << "ExprBaseLit { " << root->target() << " }";
  }

  ast_view::output_type ast_view::visit_expr_id(ast_view::input_type os) {
    auto root = (std::shared_ptr<syntax::expr_id> const&)_node;
    return os
      << _space << "ExprId { " << root->target() << " }";
  }

  ast_view::output_type ast_view::visit_expr_data_type(ast_view::input_type os) {
    auto root = (std::shared_ptr<syntax::expr_data_type> const&)_node;
    return os
      << _space << "ExprDataType { " << root->target() << " }";
  }
}

