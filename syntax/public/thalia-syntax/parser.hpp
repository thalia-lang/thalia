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

namespace thalia::syntax {
  class parser {
    public:
      enum class error_type {
        UnexpectedEof,
        ExpectedDataType,
        ExpectedPrimary,
        ExpectedRParen
      };

      using error = error<error_type, token>;
      using error_queue = error_queue<error_type, token>;

    public:
      explicit parser(
        error_queue& equeue,
        std::vector<token>::const_iterator begin,
        std::vector<token>::const_iterator end
      ) : _errors(equeue)
        , _end(end)
        , _current(begin) {}

      explicit parser(
        error_queue& equeue,
        std::vector<token> const& tokens
      ) : parser(equeue, tokens.cbegin(), tokens.cend()) {}

      auto parse() -> std::shared_ptr<expression>;

    private:
      auto advance() -> token const&;

      auto parse_expression() -> std::shared_ptr<expression>;
      auto parse_assign() -> std::shared_ptr<expression>;
      auto parse_log_or() -> std::shared_ptr<expression>;
      auto parse_log_and() -> std::shared_ptr<expression>;
      auto parse_bit_or() -> std::shared_ptr<expression>;
      auto parse_xor() -> std::shared_ptr<expression>;
      auto parse_bit_and() -> std::shared_ptr<expression>;
      auto parse_equ() -> std::shared_ptr<expression>;
      auto parse_rel() -> std::shared_ptr<expression>;
      auto parse_shift() -> std::shared_ptr<expression>;
      auto parse_add() -> std::shared_ptr<expression>;
      auto parse_mul() -> std::shared_ptr<expression>;
      auto parse_unary() -> std::shared_ptr<expression>;
      auto parse_primary() -> std::shared_ptr<expression>;
      auto parse_data_type() -> std::shared_ptr<expression>;

      auto parse_binary(
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

