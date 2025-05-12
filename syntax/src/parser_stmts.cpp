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
#include "thalia-syntax/stmts.hpp"
#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  extern auto parser::parse_statement()
    -> std::shared_ptr<statement> {
    try {
      switch (_current->type()) {
        case token_type::Return:
          return parse_stmt_return();
        case token_type::LBrace:
          return parse_stmt_block();
        case token_type::If:
          return parse_stmt_if();
        case token_type::While:
          return parse_stmt_while();
        case token_type::Def:
          return parse_stmt_local();
        default:
          return parse_stmt_expr();
      }
    } catch (error const& error) {
      _errors << error;
      skip_until({
        token_type::Semi,
        token_type::RParen,
        token_type::RBrace
      });
      return nullptr;
    }
  }

  extern auto parser::parse_stmt_local()
    -> std::shared_ptr<statement> {
    return nullptr;
  }

  extern auto parser::parse_stmt_if()
    -> std::shared_ptr<statement> {
    advance();
    auto condition = parse_expression();
    consume({ token_type::LBrace }, error_type::ExpectedLBrace);
    auto main_body = parse_stmt_block();

    if (!match(token_type::Else))
      return std::make_shared<stmt_if>(condition, main_body);

    advance();
    consume({ token_type::LBrace }, error_type::ExpectedLBrace);
    auto else_body = parse_stmt_block();
    return std::make_shared<stmt_if>(condition, main_body, else_body);
  }

  extern auto parser::parse_stmt_while()
    -> std::shared_ptr<statement> {
    advance();
    auto condition = parse_expression();
    consume({ token_type::LBrace }, error_type::ExpectedLBrace);
    auto body = parse_stmt_block();
    return std::make_shared<stmt_while>(condition, body);
  }

  extern auto parser::parse_stmt_block()
    -> std::shared_ptr<statement> {
    auto content = std::vector<std::shared_ptr<statement>>();
    advance();
    while (!eof() && !match(token_type::RBrace))
      content.push_back(parse_statement());
    consume({ token_type::RBrace }, error_type::ExpectedRBrace);
    return std::make_shared<stmt_block>(content);
  }

  extern auto parser::parse_stmt_return()
    -> std::shared_ptr<statement> {
    advance();
    auto value = parse_expression();
    consume({ token_type::Semi }, error_type::ExpectedSemi);
    return std::make_shared<stmt_return>(value);
  }

  extern auto parser::parse_stmt_expr()
    -> std::shared_ptr<statement> {
    auto value = parse_expression();
    consume({ token_type::Semi }, error_type::ExpectedSemi);
    return std::make_shared<stmt_expr>(value);
  }
}

