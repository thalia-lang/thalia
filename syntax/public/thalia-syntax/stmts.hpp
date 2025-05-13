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

#include <charconv>
#include <memory>
#include <vector>
#include <span>

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
      stmt_block(
        std::vector<std::shared_ptr<statement>> const& content
      ) : statement(stmt_type::Block)
        , _content(content) {}

      auto content() const -> std::span<std::shared_ptr<statement> const>
        { return _content; }

    private:
      std::vector<std::shared_ptr<statement>> _content;
  };

  class stmt_expr: public statement {
    public:
      stmt_expr(
        std::shared_ptr<expression> const& value
      ) : statement(stmt_type::Expr)
        , _value(value) {}

      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  class stmt_return: public statement {
    public:
      stmt_return(
        std::shared_ptr<expression> const& value
      ) : statement(stmt_type::Return)
        , _value(value) {}

      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  class stmt_if: public statement {
    public:
      stmt_if(
        std::shared_ptr<expression> const& condition,
        std::shared_ptr<statement> const& main_body,
        std::shared_ptr<statement> const& else_body = nullptr
      ) : statement(stmt_type::If)
        , _condition(condition)
        , _main_body(main_body)
        , _else_body(else_body) {}

      auto condition() const -> std::shared_ptr<expression>
        { return _condition; }
      auto main_body() const -> std::shared_ptr<statement>
        { return _main_body; }
      auto else_body() const -> std::shared_ptr<statement>
        { return _else_body; }

    private:
      std::shared_ptr<expression> _condition;
      std::shared_ptr<statement> _main_body;
      std::shared_ptr<statement> _else_body;
  };

  class stmt_while: public statement {
    public:
      stmt_while(
        std::shared_ptr<expression> const& condition,
        std::shared_ptr<statement> const& body
      ) : statement(stmt_type::While)
        , _condition(condition)
        , _body(body) {}

      auto condition() const -> std::shared_ptr<expression>
        { return _condition; }
      auto body() const -> std::shared_ptr<statement>
        { return _body; }

    private:
      std::shared_ptr<expression> _condition;
      std::shared_ptr<statement> _body;
  };

  class stmt_local: public statement {
    public:
      struct variable {
        token id;
        std::shared_ptr<expression> data_type;
        std::shared_ptr<expression> value;
        bool mut;

        variable(
          bool mut,
          token const& id,
          std::shared_ptr<expression> const& data_type,
          std::shared_ptr<expression> const& value = nullptr
        ) : id(id)
          , data_type(data_type)
          , value(value)
          , mut(mut) {}
      };

    public:
      stmt_local(
        std::vector<variable> const& content
      ) : statement(stmt_type::Local)
        , _content(content) {}

      auto content() const -> std::span<variable const>
        { return _content; }

    private:
      std::vector<variable> _content;
  };

  template <typename Input, typename Output>
  class stmt_visitor {
    public:
      stmt_visitor(std::shared_ptr<statement> const& node)
        : _node(node) {}

      virtual ~stmt_visitor() = default;

    protected:
      auto visit_stmt(Input value) -> Output;

      virtual auto visit_stmt_block(Input value) -> Output = 0;
      virtual auto visit_stmt_return(Input value) -> Output = 0;
      virtual auto visit_stmt_expr(Input value) -> Output = 0;
      virtual auto visit_stmt_if(Input value) -> Output = 0;
      virtual auto visit_stmt_while(Input value) -> Output = 0;
      virtual auto visit_stmt_local(Input value) -> Output = 0;

    protected:
      std::shared_ptr<statement> _node;
  };

  template <typename Input, typename Output>
  extern auto stmt_visitor<Input, Output>::visit_stmt(Input value)
    -> Output {
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

