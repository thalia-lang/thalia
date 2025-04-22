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
  extern auto parser::parse()
    -> std::shared_ptr<expression> {
    return parse_expression();
  }

  extern auto parser::parse_expression()
    -> std::shared_ptr<expression> {
    return parse_assign();
  }

  extern auto parser::parse_assign()
    -> std::shared_ptr<expression> {
    auto target = parse_log_or();
    auto is_assign = _current->is({
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

  extern auto parser::parse_log_or()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::LogOr },
      [&]() -> auto { return parse_log_and(); }
    );
  }

  extern auto parser::parse_log_and()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::LogAnd },
      [&]() -> auto { return parse_bit_or(); }
    );
  }

  extern auto parser::parse_bit_or()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::BitOr },
      [&]() -> auto { return parse_xor(); }
    );
  }

  extern auto parser::parse_xor()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::Xor },
      [&]() -> auto { return parse_bit_and(); }
    );
  }

  extern auto parser::parse_bit_and()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::BitAnd },
      [&]() -> auto { return parse_equ(); }
    );
  }

  extern auto parser::parse_equ()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::Equal, token_type::NotEqual },
      [&]() -> auto { return parse_rel(); }
    );
  }

  extern auto parser::parse_rel()
    -> std::shared_ptr<expression> {
    return parse_binary(
      {
        token_type::Grt,
        token_type::GrtEqual,
        token_type::Less,
        token_type::LessEqual
      },
      [&]() -> auto { return parse_shift(); }
    );
  }

  extern auto parser::parse_shift()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::RShift, token_type::LShift },
      [&]() -> auto { return parse_add(); }
    );
  }

  extern auto parser::parse_add()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::Plus, token_type::Minus },
      [&]() -> auto { return parse_mul(); }
    );
  }

  extern auto parser::parse_mul()
    -> std::shared_ptr<expression> {
    return parse_binary(
      { token_type::Mul, token_type::Div, token_type::Mod },
      [&]() -> auto { return parse_unary(); }
    );
  }

  extern auto parser::parse_unary()
    -> std::shared_ptr<expression> {
    auto is_unary = _current->is({
      token_type::Plus,
      token_type::Minus,
      token_type::LogNot,
      token_type::BitNot
    });

    if (!is_unary)
      return parse_primary();

    auto operation = *_current;
    advance();
    auto value = parse_primary();
    return std::make_shared<expr_unary>(
      expr_unary(operation, value)
    );
  }

  extern auto parser::parse_primary()
    -> std::shared_ptr<expression> {
    if (_current->is(token_type::LParen)) {
      advance();
      auto value = parse_expression();
      auto token = advance();

      if (!token.is(token_type::RParen)) {
        _errors << error(error_type::ExpectedRParen, token);
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

    _errors << error(error_type::ExpectedPrimary, token);
    return nullptr;
  }

  extern auto parser::parse_data_type()
    -> std::shared_ptr<expression> {
    auto is_type = _current->is({
      token_type::Void,
      token_type::I8,
      token_type::I16,
      token_type::I32,
      token_type::I64
    });

    auto target = advance();
    if (!is_type) {
      _errors << error(error_type::ExpectedDataType, target);
      return nullptr;
    }

    return std::make_shared<expr_data_type>(
      expr_data_type(target)
    );
  }

  extern auto parser::parse_binary(
    std::initializer_list<token_type> types,
    std::function<std::shared_ptr<expression>()> next_value
  ) -> std::shared_ptr<expression> {
    auto result = next_value();
    while (_current->is(types)) {
      auto operation = advance();
      auto rhs = next_value();
      result = std::make_shared<expr_binary>(
        expr_binary(operation, result, rhs)
      );
    }
    return result;
  }

  extern token const& parser::advance() {
    if (_current == _end)
      _errors << error(error_type::UnexpectedEof, *_current);
    return *(_current++);
  }
}

