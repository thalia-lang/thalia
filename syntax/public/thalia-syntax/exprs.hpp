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

#ifndef _THALIA_SYNTAX_EXPRS_
#define _THALIA_SYNTAX_EXPRS_

#include <memory>

#include "node.hpp"
#include "token.hpp"

namespace thalia::syntax {
  enum class expr_type {
    Assign,
    Binary,
    Unary,
    Paren,
    BaseLit,
    Id,
    DataType
  };

  using expression = node<expr_type>;

  class expr_assign: public expression {
    public:
      expr_assign(
        token const& operation,
        std::shared_ptr<expression> const& target,
        std::shared_ptr<expression> const& value
      ) : expression { expr_type::Assign }
        , _operation { operation }
        , _target { target }
        , _value { value } {}

      auto operation() const -> token
        { return _operation; }
      auto target() const -> std::shared_ptr<expression>
        { return _target; }
      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      token _operation;
      std::shared_ptr<expression> _target;
      std::shared_ptr<expression> _value;
  };

  class expr_binary: public expression {
    public:
      expr_binary(
        token const& operation,
        std::shared_ptr<expression> const& lhs,
        std::shared_ptr<expression> const& rhs
      ) : expression { expr_type::Binary }
        , _operation { operation }
        , _lhs { lhs }
        , _rhs { rhs } {}

      auto operation() const -> token
        { return _operation; }
      auto lhs() const -> std::shared_ptr<expression>
        { return _lhs; }
      auto rhs() const -> std::shared_ptr<expression>
        { return _rhs; }

    private:
      token _operation;
      std::shared_ptr<expression> _lhs;
      std::shared_ptr<expression> _rhs;
  };

  class expr_unary: public expression {
    public:
      expr_unary(
        token const& operation,
        std::shared_ptr<expression> const& value
      ) : expression { expr_type::Unary }
        , _operation { operation }
        , _value { value } {}

      auto operation() const -> token
        { return _operation; }
      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      token _operation;
      std::shared_ptr<expression> _value;
  };

  class expr_paren: public expression {
    public:
      expr_paren(std::shared_ptr<expression> const& value)
        : expression { expr_type::Paren }
        , _value { value } {}

      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  class expr_base_lit: public expression {
    public:
      expr_base_lit(
        token const& target,
        std::shared_ptr<expression> const& type = nullptr
      ) : expression { expr_type::BaseLit }
        , _target { target }
        , _data_type { type } {}

      auto target() const -> token
        { return _target; }
      auto data_type() const -> std::shared_ptr<expression>
        { return _data_type; }

    private:
      token _target;
      std::shared_ptr<expression> _data_type;
  };

  class expr_id: public expression {
    public:
      expr_id(token const& target)
        : expression { expr_type::Id }
        , _target { target } {}

      auto target() const -> token { return _target; }

    private:
      token _target;
  };

  class expr_data_type: public expression {
    public:
      expr_data_type(token const& target)
        : expression { expr_type::DataType }
        , _target { target } {}

      auto target() const -> token { return _target; }

    private:
      token _target;
  };

  template <typename Input, typename Output>
  class expr_visitor {
    public:
      expr_visitor(std::shared_ptr<expression> const& node)
        : _node { node } {}

      virtual ~expr_visitor() = default;

    protected:
      auto visit_expr(Input value) -> Output;

      virtual auto visit_expr_assign(Input value) -> Output = 0;
      virtual auto visit_expr_binary(Input value) -> Output = 0;
      virtual auto visit_expr_unary(Input value) -> Output = 0;
      virtual auto visit_expr_paren(Input value) -> Output = 0;
      virtual auto visit_expr_base_lit(Input value) -> Output = 0;
      virtual auto visit_expr_id(Input value) -> Output = 0;
      virtual auto visit_expr_data_type(Input value) -> Output = 0;

    protected:
      std::shared_ptr<expression> _node;
  };

  template <typename Input, typename Output>
  extern auto expr_visitor<Input, Output>::visit_expr(Input value)
    -> Output {
    switch (_node->type()) {
      case expr_type::Assign:
        return visit_expr_assign(value);
      case expr_type::Binary:
        return visit_expr_binary(value);
      case expr_type::Unary:
        return visit_expr_unary(value);
      case expr_type::Paren:
        return visit_expr_paren(value);
      case expr_type::BaseLit:
        return visit_expr_base_lit(value);
      case expr_type::Id:
        return visit_expr_id(value);
      case expr_type::DataType:
        return visit_expr_data_type(value);
    }
  }
}

#endif // _THALIA_SYNTAX_EXPRS_

