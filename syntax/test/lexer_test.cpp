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

#include <cstddef>
#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>

#include "thalia-syntax/lexer.hpp"
#include "thalia-syntax/token.hpp"

#define CHECK_ALL

using namespace thalia;
using namespace thalia::syntax;

class fake_error_queue: public lexer::error_queue {
  public:
    fake_error_queue()
      : lexer::error_queue {}
      , _size { 0 } {}

    auto size() const
      -> std::size_t { return _size; }
    auto operator<<([[maybe_unused]] lexer::error const& error)
      -> error_queue& override { return ++_size, *this; }

  private:
    std::size_t _size;
};

static auto check_all(
  lexer::error_queue& queue,
  std::string code,
  std::initializer_list<token_type> types
) -> void {
  auto lexer = syntax::lexer { queue, code };
  auto tokens = lexer.scan_all();
  REQUIRE(tokens.size() == types.size());

  auto it = types.begin();
  for (auto const& token: tokens)
    CHECK(token.type() == *(it++));
}

TEST_CASE("lexer::scan_all") {
  auto queue = fake_error_queue {};

  SECTION("mixed token types") {
    check_all(
      queue, "def x = 0;",
      {
        token_type::Def,
        token_type::Id,
        token_type::Assign,
        token_type::Int,
        token_type::Semi,
        token_type::Eof
      }
    );
  }
}

