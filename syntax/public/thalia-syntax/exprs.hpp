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
  /**
   * @brief Enumerates the kinds of expressions in the Thalia syntax tree.
   */
  enum class expr_type {
    Assign,
    Binary,
    Unary,
    Paren,
    BaseLit,
    Id,
    DataType
  };

  /**
   * @brief Alias for a polymorphic expression node using expr_type.
   */
  using expression = node<expr_type>;

  /**
   * @brief Represents an assignment expression (e.g., `a = b`).
   */
  class expr_assign: public expression {
    public:
      /**
       * @brief Constructs an assignment expression.
       * @param operation The assignment operator token (e.g., `=`).
       * @param target The target being assigned to.
       * @param value The value being assigned.
       */
      expr_assign(
        token const& operation,
        std::shared_ptr<expression> const& target,
        std::shared_ptr<expression> const& value
      ) : expression { expr_type::Assign }
        , _operation { operation }
        , _target { target }
        , _value { value } {}

      /**
       * @brief Gets the assignment operator token (e.g., `=`).
       * @return The assignment token.
       */
      auto operation() const -> token
        { return _operation; }

      /**
       * @brief Gets the left-hand side of the assignment.
       * @return Expression assigned to.
       */
      auto target() const -> std::shared_ptr<expression>
        { return _target; }

      /**
       * @brief Gets the right-hand side of the assignment.
       * @return Expression being assigned.
       */
      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      token _operation;
      std::shared_ptr<expression> _target;
      std::shared_ptr<expression> _value;
  };

  /**
   * @brief Represents a binary operation expression (e.g., `a + b`).
   */
  class expr_binary: public expression {
    public:
      /**
       * @brief Constructs a binary expression.
       * @param operation The binary operator token.
       * @param lhs The left-hand operand.
       * @param rhs The right-hand operand.
       */
      expr_binary(
        token const& operation,
        std::shared_ptr<expression> const& lhs,
        std::shared_ptr<expression> const& rhs
      ) : expression { expr_type::Binary }
        , _operation { operation }
        , _lhs { lhs }
        , _rhs { rhs } {}

      /**
       * @brief Gets the binary operator token (e.g., `+`, `-`).
       * @return The operator token.
       */
      auto operation() const -> token
        { return _operation; }

      /**
       * @brief Gets the left-hand operand.
       * @return The left operand expression.
       */
      auto lhs() const -> std::shared_ptr<expression>
        { return _lhs; }

      /**
       * @brief Gets the left-hand operand.
       * @return The left operand expression.
       */
      auto rhs() const -> std::shared_ptr<expression>
        { return _rhs; }

    private:
      token _operation;
      std::shared_ptr<expression> _lhs;
      std::shared_ptr<expression> _rhs;
  };

  /**
   * @brief Represents a unary operation expression (e.g., `-x`).
   */
  class expr_unary: public expression {
    public:
      /**
       * @brief Constructs a unary expression.
       * @param operation The unary operator token.
       * @param value The operand.
       */
      expr_unary(
        token const& operation,
        std::shared_ptr<expression> const& value
      ) : expression { expr_type::Unary }
        , _operation { operation }
        , _value { value } {}

      /**
       * @brief Gets the unary operator token (e.g., `-`, `!`).
       * @return The operator token.
       */
      auto operation() const -> token
        { return _operation; }

      /**
       * @brief Gets the operand of the unary operation.
       * @return The operand expression.
       */
      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      token _operation;
      std::shared_ptr<expression> _value;
  };

  /**
   * @brief Represents a parenthesized expression (e.g., `(x + y)`).
   */
  class expr_paren: public expression {
    public:
      /**
       * @brief Constructs a parenthesized expression.
       * @param value The enclosed expression.
       */
      expr_paren(std::shared_ptr<expression> const& value)
        : expression { expr_type::Paren }
        , _value { value } {}

      /**
       * @brief Gets the inner expression inside parentheses.
       * @return The enclosed expression.
       */
      auto value() const -> std::shared_ptr<expression>
        { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  /**
   * @brief Represents a base literal expression (e.g., integer literal).
   */
  class expr_base_lit: public expression {
    public:
      /**
       * @brief Constructs a base literal expression.
       * @param target The literal token.
       * @param type Optional type annotation.
       */
      expr_base_lit(
        token const& target,
        std::shared_ptr<expression> const& type = nullptr
      ) : expression { expr_type::BaseLit }
        , _target { target }
        , _data_type { type } {}

      /**
       * @brief Gets the token representing the literal value.
       * @return The literal token.
       */
      auto target() const -> token
        { return _target; }

      /**
       * @brief Gets the optional type annotation for the literal.
       * @return Expression representing the type, or nullptr.
       */
      auto data_type() const -> std::shared_ptr<expression>
        { return _data_type; }

    private:
      token _target;
      std::shared_ptr<expression> _data_type;
  };

  /**
   * @brief Represents an identifier expression.
   */
  class expr_id: public expression {
    public:
      /**
       * @brief Constructs an identifier expression.
       * @param target The identifier token.
       */
      expr_id(token const& target)
        : expression { expr_type::Id }
        , _target { target } {}

      /**
       * @brief Gets the token representing the identifier.
       * @return The identifier token.
       */
      auto target() const -> token { return _target; }

    private:
      token _target;
  };

  /**
   * @brief Represents a type literal expression (e.g., `i32`, `void`).
   */
  class expr_data_type: public expression {
    public:
      /**
       * @brief Constructs a type literal expression.
       * @param target The type token.
       */
      expr_data_type(token const& target)
        : expression { expr_type::DataType }
        , _target { target } {}

      /**
       * @brief Gets the token representing the type.
       * @return The type token.
       */
      auto target() const -> token { return _target; }

    private:
      token _target;
  };

  /**
   * @brief Base class for implementing the visitor pattern for expressions.
   * @tparam Input The input parameter type passed to visitor methods.
   * @tparam Output The result type returned from visitor methods.
   */
  template <typename Input, typename Output>
  class expr_visitor {
    public:
      /**
       * @brief Constructs the visitor with the target expression node.
       * @param node The expression to be visited.
       */
      expr_visitor(std::shared_ptr<expression> const& node)
        : _node { node } {}

      /**
       * @brief Virtual destructor for safe polymorphic deletion.
       */
      virtual ~expr_visitor() = default;

    protected:
      /**
       * @brief Dispatches the visit based on the expression type.
       * @param value The external input to pass to the visitor.
       * @return The result of visiting the expression.
       */
      auto visit_expr(Input value) -> Output;

      // Visitor entrypoints for each concrete expression type
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

