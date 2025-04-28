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

#include "thalia-syntax/parser.hpp"
#include "thalia-syntax/exprs.hpp"
#include "thalia-syntax/stmts.hpp"
#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  extern auto parser::parse()
    -> std::vector<std::shared_ptr<statement>> {
    auto result = std::vector<std::shared_ptr<statement>>();
    while (!eof())
      result.push_back(parse_statement());
    return result;
  }

  extern auto parser::parse_statement()
    -> std::shared_ptr<statement> {
    switch (_current->type()) {
      case token_type::Return:
        return parse_stmt_return();
      case token_type::LBrace:
        return parse_stmt_block();
      case token_type::If:
        return parse_stmt_if();
      case token_type::While:
        return parse_stmt_while();
      /*case token_type::Def:*/
      /*  return parse_stmt_local();*/
      default:
        return parse_stmt_expr();
    }
  }

  extern auto parser::parse_stmt_if()
    -> std::shared_ptr<statement> {
    advance();
    if (!match(token_type::LParen)) {
      auto error = parser::error(error_type::ExpectedLParen, *_current);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }

    advance();
    auto condition = parse_expression();

    if (!match(token_type::RParen)) {
      auto error = parser::error(error_type::ExpectedRParen, *_current);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }

    advance();
    if (!match(token_type::LBrace)) {
      auto error = parser::error(error_type::ExpectedLBrace, *_current);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }

    auto main_body = parse_stmt_block();
    if (!match(token_type::Else)) {
      return std::make_shared<stmt_if>(
        stmt_if(condition, main_body)
      );
    }

    advance();
    if (!match(token_type::LBrace)) {
      auto error = parser::error(error_type::ExpectedLBrace, *_current);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }

    auto else_body = parse_stmt_block();
    return std::make_shared<stmt_if>(
      stmt_if(condition, main_body, else_body)
    );
  }

  extern auto parser::parse_stmt_while()
    -> std::shared_ptr<statement> {
    advance();
    if (!match(token_type::LParen)) {
      auto error = parser::error(error_type::ExpectedLParen, *_current);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }

    advance();
    auto condition = parse_expression();

    if (!match(token_type::RParen)) {
      auto error = parser::error(error_type::ExpectedRParen, *_current);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }

    advance();
    if (!match(token_type::LBrace)) {
      auto error = parser::error(error_type::ExpectedLBrace, *_current);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }

    auto body = parse_stmt_block();
    return std::make_shared<stmt_while>(
      stmt_while(condition, body)
    );
  }

  extern auto parser::parse_stmt_block()
    -> std::shared_ptr<statement> {
    auto content = std::vector<std::shared_ptr<statement>>();
    advance();
    while (!eof() && !match(token_type::RBrace))
      content.push_back(parse_statement());
    auto token = advance();
    if (!token.is(token_type::RBrace)) {
      auto error = parser::error(error_type::ExpectedRBrace, token);
      throw_error(error);
      return nullptr;
    }
    return std::make_shared<stmt_block>(
      stmt_block(content)
    );
  }

  extern auto parser::parse_stmt_return()
    -> std::shared_ptr<statement> {
    advance();
    auto value = parse_expression();
    auto token = advance();
    if (!token.is(token_type::Semi)) {
      auto error = parser::error(error_type::ExpectedSemi, token);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }
    return std::make_shared<stmt_return>(
      stmt_return(value)
    );
  }

  extern auto parser::parse_stmt_expr()
    -> std::shared_ptr<statement> {
    auto value = parse_expression();
    auto token = advance();
    if (!token.is(token_type::Semi)) {
      auto error = parser::error(error_type::ExpectedSemi, token);
      throw_error(error, { token_type::Semi, token_type::RBrace });
      return nullptr;
    }
    return std::make_shared<stmt_expr>(
      stmt_expr(value)
    );
  }

  extern auto parser::parse_expr_assign()
    -> std::shared_ptr<expression> {
    auto target = parse_expr_log_or();
    auto is_assign = match({
      token_type::Assign,
      token_type::AndAssign,
      token_type::OrAssign,
      token_type::RshAssign,
      token_type::LshAssign,
      token_type::DivAssign,
      token_type::ModAssign,
      token_type::MulAssign,
      token_type::PlusAssign,
      token_type::MinusAssign,
      token_type::XorAssign
    });
    if (!is_assign)
      return target;

    auto operation = advance();
    auto value = parse_expression();
    return std::make_shared<expr_assign>(
      expr_assign(operation, target, value)
    );
  }

  extern auto parser::parse_expr_log_or()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::LogOr },
      [&]() -> auto { return parse_expr_log_and(); }
    );
  }

  extern auto parser::parse_expr_log_and()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::LogAnd },
      [&]() -> auto { return parse_expr_bit_or(); }
    );
  }

  extern auto parser::parse_expr_bit_or()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::BitOr },
      [&]() -> auto { return parse_expr_xor(); }
    );
  }

  extern auto parser::parse_expr_xor()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::Xor },
      [&]() -> auto { return parse_expr_bit_and(); }
    );
  }

  extern auto parser::parse_expr_bit_and()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::BitAnd },
      [&]() -> auto { return parse_expr_equ(); }
    );
  }

  extern auto parser::parse_expr_equ()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::Equal, token_type::NotEqual },
      [&]() -> auto { return parse_expr_rel(); }
    );
  }

  extern auto parser::parse_expr_rel()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      {
        token_type::Grt,
        token_type::GrtEqual,
        token_type::Less,
        token_type::LessEqual
      },
      [&]() -> auto { return parse_expr_shift(); }
    );
  }

  extern auto parser::parse_expr_shift()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::RShift, token_type::LShift },
      [&]() -> auto { return parse_expr_add(); }
    );
  }

  extern auto parser::parse_expr_add()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::Plus, token_type::Minus },
      [&]() -> auto { return parse_expr_mul(); }
    );
  }

  extern auto parser::parse_expr_mul()
    -> std::shared_ptr<expression> {
    return parse_expr_binary(
      { token_type::Mul, token_type::Div, token_type::Mod },
      [&]() -> auto { return parse_expr_unary(); }
    );
  }

  extern auto parser::parse_expr_unary()
    -> std::shared_ptr<expression> {
    auto is_unary = match({
      token_type::Plus,
      token_type::Minus,
      token_type::LogNot,
      token_type::BitNot
    });

    if (!is_unary)
      return parse_expr_primary();

    auto operation = *_current;
    advance();
    auto value = parse_expr_primary();
    return std::make_shared<expr_unary>(
      expr_unary(operation, value)
    );
  }

  extern auto parser::parse_expr_primary()
    -> std::shared_ptr<expression> {
    if (match(token_type::LParen)) {
      advance();
      auto value = parse_expression();
      auto token = advance();

      if (!token.is(token_type::RParen)) {
        throw_error(error(error_type::ExpectedRParen, token));
        return nullptr;
      }

      return std::make_shared<expr_paren>(
        expr_paren(value)
      );
    }

    auto token = advance();
    if (token.is(token_type::Id)) {
      return std::make_shared<expr_id>(
        expr_id(token)
      );
    }

    if (token.is(token_type::Int)) {
      return std::make_shared<expr_base_lit>(
        expr_base_lit(token)
      );
    }

    throw_error(error(error_type::ExpectedPrimary, token));
    return nullptr;
  }

  extern auto parser::parse_expr_data_type()
    -> std::shared_ptr<expression> {
    auto is_type = match({
      token_type::Void,
      token_type::I8,
      token_type::I16,
      token_type::I32,
      token_type::I64
    });

    auto target = advance();
    if (!is_type) {
      throw_error(error(error_type::ExpectedDataType, target));
      return nullptr;
    }

    return std::make_shared<expr_data_type>(
      expr_data_type(target)
    );
  }

  extern auto parser::parse_expr_binary(
    std::initializer_list<token_type> types,
    std::function<std::shared_ptr<expression>()> next_value
  ) -> std::shared_ptr<expression> {
    auto result = next_value();
    while (match(types)) {
      auto operation = advance();
      auto rhs = next_value();
      result = std::make_shared<expr_binary>(
        expr_binary(operation, result, rhs)
      );
    }
    return result;
  }

  extern auto parser::advance() -> token const& {
    auto prev = _current;
    if (eof())
      _errors << error(error_type::UnexpectedEof, *prev);
    else ++_current;
    return *prev;
  }

  extern auto parser::throw_error(
    error error,
    std::initializer_list<token_type> skip_until
  ) -> void {
    _errors << error;
    if (skip_until.size() == 0) {
      while (!eof() && !match(skip_until))
        ++_current;
    }
  }
}

