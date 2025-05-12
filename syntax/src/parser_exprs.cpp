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
#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  extern auto parser::parse_expression()
    -> std::shared_ptr<expression> {
    try {
      return parse_expr_assign();
    } catch(error const& error) {
      _errors << error;
      skip_until({
        token_type::Semi,
        token_type::RParen,
        token_type::LBrace,
        token_type::RBrace
      });
      return nullptr;
    }
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
    return std::make_shared<expr_assign>(operation, target, value);
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

    auto operation = advance();
    auto value = parse_expr_primary();
    return std::make_shared<expr_unary>(operation, value);
  }

  extern auto parser::parse_expr_primary()
    -> std::shared_ptr<expression> {
    auto types = { token_type::LParen, token_type::Id, token_type::Int };
    auto token = consume(types, error_type::ExpectedPrimary);

    if (token.is(token_type::LParen))
      return parse_expr_paren();
    if (token.is(token_type::Id))
      return std::make_shared<expr_id>(token);
    return std::make_shared<expr_base_lit>(token);
  }

  extern auto parser::parse_expr_paren()
    -> std::shared_ptr<expression> {
    auto value = parse_expression();
    consume({ token_type::RParen }, error_type::ExpectedRParen);
    return std::make_shared<expr_paren>(value);
  }

  extern auto parser::parse_expr_data_type()
    -> std::shared_ptr<expression> {
    auto types = {
      token_type::Void,
      token_type::I8,
      token_type::I16,
      token_type::I32,
      token_type::I64
    };

    auto target = consume(types, error_type::ExpectedDataType);
    return std::make_shared<expr_data_type>(target);
  }

  extern auto parser::parse_expr_binary(
    std::initializer_list<token_type> types,
    std::function<std::shared_ptr<expression>()> next_value
  ) -> std::shared_ptr<expression> {
    auto result = next_value();
    while (match(types)) {
      auto operation = advance();
      auto rhs = next_value();
      result = std::make_shared<expr_binary>(operation, result, rhs);
    }
    return result;
  }
}

