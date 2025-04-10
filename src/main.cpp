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

#include "error_queue.hpp"

using namespace thalia;

extern int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "[ERROR]: Invalid number of args.\n";
    return 1;
  }

  error_queue equeue(std::cout);

  std::string code(argv[1]);
  syntax::lexer lexer(equeue, code.begin(), code.end());

  syntax::token token = lexer.next();
  while (!token.eof()) {
    if (!token.unknown())
      std::cout << token << "\n";
    token = lexer.next();
  }
  return 0;
}

