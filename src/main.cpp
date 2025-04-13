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

#include <iostream>

#include <thalia-syntax/lexer.hpp>
#include <thalia-syntax/parser.hpp>

#include "ast_view.hpp"
#include "error_queue.hpp"

using namespace thalia;

extern int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "[ERROR]: Invalid number of args.\n";
    return 1;
  }

  std::string code(argv[1]);
  error_queue equeue(std::cout);

  std::cout << "=== LEXER ===\n";
  syntax::lexer lexer(equeue, code.begin(), code.end());
  auto tokens = lexer.scan_all();
  for (auto const& t: tokens)
    std::cout << t << '\n';
  if (!equeue.empty())
    return 1;

  std::cout << "\n=== PARSER ===\n";
  syntax::parser parser(equeue, tokens);
  auto ast = parser.parse();
  if (!equeue.empty())
    return 1;

  ast_view view(ast);
  std::cout << view << '\n';
  return 0;
}

