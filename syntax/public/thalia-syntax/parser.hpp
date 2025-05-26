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

#ifndef _THALIA_SYNTAX_PARSER_
#define _THALIA_SYNTAX_PARSER_

#include <initializer_list>
#include <memory>
#include <functional>
#include <vector>

#include "errors.hpp"
#include "exprs.hpp"
#include "stmts.hpp"
#include "token.hpp"

namespace thalia::syntax {
  class parser {
    public:
      enum class error_type {
        UnexpectedEof,
        ExpectedDataType,
        ExpectedPrimary,
        ExpectedLParen,
        ExpectedRParen,
        ExpectedSemi,
        ExpectedLBrace,
        ExpectedRBrace,
        ExpectedId,
        ExpectedColon,
        ExpectedConstValue,
        ExpectedLitType
      };

      using error = error<error_type, token>;
      using error_queue = error_queue<error_type, token>;

    public:
      parser(
        error_queue& equeue,
        std::vector<token> const& tokens
      ) : parser(equeue, tokens.cbegin(), tokens.cend()) {}

      auto parse() -> std::vector<std::shared_ptr<statement>>;

    private:
      parser(
        error_queue& equeue,
        std::vector<token>::const_iterator begin,
        std::vector<token>::const_iterator end
      ) : _errors(equeue)
        , _end(end)
        , _current(begin) {}

      auto eof() -> bool
        { return _current->is(token_type::Eof); }
      auto match(token_type type) -> bool
        { return _current->is(type); }
      auto match(std::initializer_list<token_type> types) -> bool
        { return _current->is(types); }

      auto advance() -> token const&;
      auto consume(
        std::initializer_list<token_type> types,
        error_type error
      ) -> token const&;
      auto skip_until(std::initializer_list<token_type> types) -> void;

      auto parse_statement() -> std::shared_ptr<statement>;
      auto parse_expression() -> std::shared_ptr<expression>;

      auto parse_stmt_block() -> std::shared_ptr<statement>;
      auto parse_stmt_return() -> std::shared_ptr<statement>;
      auto parse_stmt_expr() -> std::shared_ptr<statement>;
      auto parse_stmt_if() -> std::shared_ptr<statement>;
      auto parse_stmt_while() -> std::shared_ptr<statement>;
      auto parse_stmt_local() -> std::shared_ptr<statement>;

      auto parse_expr_assign() -> std::shared_ptr<expression>;
      auto parse_expr_log_or() -> std::shared_ptr<expression>;
      auto parse_expr_log_and() -> std::shared_ptr<expression>;
      auto parse_expr_bit_or() -> std::shared_ptr<expression>;
      auto parse_expr_xor() -> std::shared_ptr<expression>;
      auto parse_expr_bit_and() -> std::shared_ptr<expression>;
      auto parse_expr_equ() -> std::shared_ptr<expression>;
      auto parse_expr_rel() -> std::shared_ptr<expression>;
      auto parse_expr_shift() -> std::shared_ptr<expression>;
      auto parse_expr_add() -> std::shared_ptr<expression>;
      auto parse_expr_mul() -> std::shared_ptr<expression>;
      auto parse_expr_unary() -> std::shared_ptr<expression>;
      auto parse_expr_primary() -> std::shared_ptr<expression>;
      auto parse_expr_paren() -> std::shared_ptr<expression>;
      auto parse_expr_data_type() -> std::shared_ptr<expression>;
      auto parse_expr_binary(
        std::initializer_list<token_type> types,
        std::function<std::shared_ptr<expression>()> next_value
      ) -> std::shared_ptr<expression>;

    private:
      error_queue& _errors;
      std::vector<token>::const_iterator _end;
      std::vector<token>::const_iterator _current;
  };
}

#endif // _THALIA_SYNTAX_PARSER_

