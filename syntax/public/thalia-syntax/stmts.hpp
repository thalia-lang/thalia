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

#ifndef _THALIA_SYNTAX_STMTS_
#define _THALIA_SYNTAX_STMTS_

#include <memory>
#include <vector>

#include "node.hpp"
#include "exprs.hpp"
#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  enum class stmt_type {
    Block,
    Expr,
    Return,
    If,
    While,
    Local
  };

  using statement = node<stmt_type>;

  class stmt_block: public statement {
    public:
      explicit stmt_block(
        std::vector<std::shared_ptr<statement>> const& content
      ) : statement(stmt_type::Block)
        , _content(content) {}

      std::vector<std::shared_ptr<statement>> content() { return _content; }

    private:
      std::vector<std::shared_ptr<statement>> _content;
  };

  class stmt_expr: public statement {
    public:
      explicit stmt_expr(
        std::shared_ptr<expression> const& value
      ) : statement(stmt_type::Expr)
        , _value(value) {}

      std::shared_ptr<expression> value() const { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  class stmt_return: public statement {
    public:
      explicit stmt_return(
        std::shared_ptr<expression> const& value
      ) : statement(stmt_type::Return)
        , _value(value) {}

      std::shared_ptr<expression> value() const { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  class stmt_if: public statement {
    public:
      explicit stmt_if(
        std::shared_ptr<expression> const& condition,
        std::shared_ptr<statement> const& main_body,
        std::shared_ptr<statement> const& else_body
      ) : statement(stmt_type::If)
        , _condition(condition)
        , _main_body(main_body)
        , _else_body(else_body) {}

      std::shared_ptr<expression> condition() { return _condition; }
      std::shared_ptr<statement> main_body() { return _main_body; }
      std::shared_ptr<statement> else_body() { return _else_body; }

    private:
      std::shared_ptr<expression> _condition;
      std::shared_ptr<statement> _main_body;
      std::shared_ptr<statement> _else_body;
  };

  class stmt_while: public statement {
    public:
      explicit stmt_while(
        std::shared_ptr<expression> const& condition,
        std::shared_ptr<statement> const& body
      ) : statement(stmt_type::While)
        , _condition(condition)
        , _body(body) {}

      std::shared_ptr<expression> condition() { return _condition; }
      std::shared_ptr<statement> body() { return _body; }

    private:
      std::shared_ptr<expression> _condition;
      std::shared_ptr<statement> _body;
  };

  class stmt_local: public statement {
    public:
      explicit stmt_local(
        token const& id,
        std::shared_ptr<expression> const& data_type,
        std::shared_ptr<expression> const& value,
        bool mut = false
      ) : statement(stmt_type::Local)
        , _id(id)
        , _data_type(data_type)
        , _value(value)
        , _mut(mut) {}

      bool is_mut() { return _mut; }
      token id() { return _id; }
      std::shared_ptr<expression> data_type() { return _data_type; }
      std::shared_ptr<expression> value() { return _value; }

    private:
      token _id;
      std::shared_ptr<expression> _data_type;
      std::shared_ptr<expression> _value;
      bool _mut;
  };

  template <typename Input, typename Output>
  class stmt_visitor {
    public:
      using input_type = Input;
      using output_type = Output;

    public:
      explicit stmt_visitor(std::shared_ptr<statement> const& node)
        : _node(node) {}

      virtual ~stmt_visitor() = default;

    protected:
      output_type visit_stmt(input_type value);

      virtual output_type visit_stmt_block(input_type value) = 0;
      virtual output_type visit_stmt_return(input_type value) = 0;
      virtual output_type visit_stmt_expr(input_type value) = 0;
      virtual output_type visit_stmt_if(input_type value) = 0;
      virtual output_type visit_stmt_while(input_type value) = 0;
      virtual output_type visit_stmt_local(input_type value) = 0;

    protected:
      std::shared_ptr<statement> _node;
  };

  template <typename Input, typename Output>
  extern Output stmt_visitor<Input, Output>::visit_stmt(Input value) {
    switch (_node->type()) {
      case stmt_type::Block:
        return visit_stmt_block(value);
      case stmt_type::Expr:
        return visit_stmt_expr(value);
      case stmt_type::Return:
        return visit_stmt_return(value);
      case stmt_type::If:
        return visit_stmt_if(value);
      case stmt_type::While:
        return visit_stmt_while(value);
      case stmt_type::Local:
        return visit_stmt_local(value);
    }
  }
}

#endif // _THALIA_SYNTAX_STMTS_

