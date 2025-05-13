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

#include <fstream>
#include <iostream>
#include <filesystem>

#include <thalia-syntax/lexer.hpp>
#include <thalia-syntax/parser.hpp>

#include "stmt_view.hpp"
#include "error_queue.hpp"

extern auto main(int argc, char** argv) -> int {
  if (argc < 2) {
    std::cout << "[ERROR]: Invalid number of args.\n";
    return 1;
  }

  auto path = std::filesystem::absolute(argv[1]);
  std::cout << "FILE: " << path << '\n';
  if (!std::filesystem::exists(path)) {
    std::cout << "[ERROR]: File does not exists.\n";
    return 1;
  }

  if (path.extension() != ".th") {
    std::cout << "[ERROR]: Invalid file extension.\n";
    return 1;
  }

  auto file = std::ifstream(path);
  auto code = std::string(
    (std::istreambuf_iterator<char>(file)),
    (std::istreambuf_iterator<char>())
  );

  using namespace thalia;
  auto equeue = error_queue(std::cout, 20);
  auto lexer = syntax::lexer(equeue, code);

  std::cout << "\n===   Lexemes   ===\n";
  auto tokens = lexer.scan_all();
  if (!equeue.empty())
    return 1;
  for (auto const& t: tokens)
    std::cout << t << '\n';

  auto parser = syntax::parser(equeue, tokens);

  std::cout << "\n=== Syntax Tree ===\n";
  auto ast = parser.parse();
  if (!equeue.empty())
    return 1;
  for (auto const& node: ast) {
    auto view = stmt_view(node);
    std::cout << view << '\n';
  }
  return 0;
}

