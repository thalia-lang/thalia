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
#include <span>

#include "node.hpp"
#include "exprs.hpp"
#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  /**
   * @brief Enumerates the kinds of statements in the Thalia syntax tree.
   */
  enum class stmt_type {
    Block,
    Expr,
    Return,
    If,
    While,
    Local
  };

  /**
   * @brief Alias for a polymorphic expression node using expr_type.
   */
  using statement = node<stmt_type>;

  /**
   * @brief Represents a block of statements.
   */
  class stmt_block: public statement {
    public:
      /**
       * @brief Constructs a block statement with a list of child statements.
       * @param content The list of statements in the block.
       */
      stmt_block(
        std::vector<std::shared_ptr<statement>> const& content
      ) : statement { stmt_type::Block }
        , _content { content } {}

      /**
       * @brief Returns the list of statements in the block.
       * @return A span of statement pointers.
       */
      auto content() const -> std::span<std::shared_ptr<statement> const>
        { return _content; }

    private:
      std::vector<std::shared_ptr<statement>> _content;
  };

  /**
   * @brief Represents an expression used as a statement.
   */
  class stmt_expr: public statement {
    public:
      /**
       * @brief Constructs an expression statement from an expression.
       * @param value The expression being evaluated.
       */
      stmt_expr(
        std::shared_ptr<expression> const& value
      ) : statement { stmt_type::Expr }
        , _value { value } {}

      /**
       * @brief Returns the expression of the statement.
       * @return A pointer to the expression.
       */
      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  /**
   * @brief Represents a return statement.
   */
  class stmt_return: public statement {
    public:
      /**
       * @brief Constructs a return statement with an optional return value.
       * @param value The optional return value expression.
       */
      stmt_return(
        std::shared_ptr<expression> const& value
      ) : statement { stmt_type::Return }
        , _value { value } {}

      /**
       * @brief Returns the return value expression.
       * @return A pointer to the return expression.
       */
      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  /**
   * @brief Represents an `if` or `if-else` conditional statement.
   */
  class stmt_if: public statement {
    public:
      /**
       * @brief Constructs an if/else conditional statement.
       * @param condition The condition expression.
       * @param main_body The statement executed when condition is true.
       * @param else_body Optional statement for the else branch.
       */
      stmt_if(
        std::shared_ptr<expression> const& condition,
        std::shared_ptr<statement> const& main_body,
        std::shared_ptr<statement> const& else_body = nullptr
      ) : statement { stmt_type::If }
        , _condition { condition }
        , _main_body { main_body }
        , _else_body { else_body } {}

      /**
       * @brief Returns the condition expression.
       * @return A pointer to the condition expression.
       */
      auto condition() const -> std::shared_ptr<expression>
        { return _condition; }

      /**
       * @brief Returns the main branch body.
       * @return A pointer to the main statement.
       */
      auto main_body() const -> std::shared_ptr<statement>
        { return _main_body; }

      /**
       * @brief Returns the optional else branch body.
       * @return A pointer to the else statement, or nullptr.
       */
      auto else_body() const -> std::shared_ptr<statement>
        { return _else_body; }

    private:
      std::shared_ptr<expression> _condition;
      std::shared_ptr<statement> _main_body;
      std::shared_ptr<statement> _else_body;
  };

  /**
   * @brief Represents a while-loop statement.
   */
  class stmt_while: public statement {
    public:
      /**
       * @brief Constructs a while-loop with a condition and a body.
       * @param condition The loop condition expression.
       * @param body The loop body statement.
       */
      stmt_while(
        std::shared_ptr<expression> const& condition,
        std::shared_ptr<statement> const& body
      ) : statement { stmt_type::While }
        , _condition { condition }
        , _body { body } {}

      /**
       * @brief Returns the loop condition expression.
       * @return A pointer to the condition expression.
       */
      auto condition() const -> std::shared_ptr<expression>
        { return _condition; }

      /**
       * @brief Returns the loop body.
       * @return A pointer to the body statement.
       */
      auto body() const -> std::shared_ptr<statement>
        { return _body; }

    private:
      std::shared_ptr<expression> _condition;
      std::shared_ptr<statement> _body;
  };

  /**
   * @brief Represents a local variable declaration statement.
   */
  class stmt_local: public statement {
    public:
      /**
       * @brief Represents a declared local variable declaration.
       */
      struct variable {
        token id;
        std::shared_ptr<expression> data_type;
        std::shared_ptr<expression> value;
        bool mut;

        /**
         * @brief Constructs a local variable.
         * @param mut Whether the variable is mutable.
         * @param id The identifier token.
         * @param data_type The type annotation.
         * @param value Optional initializer expression.
         */
        variable(
          bool mut,
          token const& id,
          std::shared_ptr<expression> const& data_type,
          std::shared_ptr<expression> const& value = nullptr
        ) : id { id }
          , data_type { data_type }
          , value { value }
          , mut { mut } {}
      };

    public:
      /**
       * @brief Constructs a local statement with a list of variable declarations.
       * @param content A list of variable declarations.
       */
      stmt_local(
        std::vector<variable> const& content
      ) : statement(stmt_type::Local)
        , _content { content } {}

      /**
       * @brief Returns the declared variables in the local statement.
       * @return A span of variable declarations.
       */
      auto content() const -> std::span<variable const>
        { return _content; }

    private:
      std::vector<variable> _content;
  };

  /**
   * @brief Abstract visitor base class for statement traversal.
   * @tparam Input The input type passed to the visitor.
   * @tparam Output The return type of each visit method.
   */
  template <typename Input, typename Output>
  class stmt_visitor {
    public:
      /**
       * @brief Constructs the visitor with the target statement node.
       * @param node The root statement node to visit.
       */
      stmt_visitor(std::shared_ptr<statement> const& node)
        : _node { node } {}

      /**
       * @brief Virtual destructor for safe polymorphic deletion.
       */
      virtual ~stmt_visitor() = default;

    protected:
      /**
       * @brief Dispatches the visit based on the statement type.
       * @param value The external input to pass to the visitor.
       * @return The result of visiting the expression.
       */
      auto visit_stmt(Input value) -> Output;

      // Visitor entrypoints for each concrete statement type
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

