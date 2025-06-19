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

#define CATCH_CONFIG_MAIN

#include <initializer_list>
#include <string_view>
#include <catch2/catch_test_macros.hpp>

#include "thalia-syntax/token.hpp"

using namespace thalia;

TEST_CASE("token::is") {
  auto types1 = std::initializer_list<syntax::token_type> {
    syntax::token_type::Unknown,
    syntax::token_type::Id,
    syntax::token_type::Mut,
    syntax::token_type::LogOr
  };

  auto types2 = std::initializer_list<syntax::token_type> {
    syntax::token_type::Eof,
    syntax::token_type::Assign,
    syntax::token_type::Def,
    syntax::token_type::I8
  };

  for (auto type1: types1) {
    auto token = syntax::token { type1 };

    CHECK(token.is(type1));
    for (auto type2: types2)
      CHECK_FALSE(token.is(type2));

    CHECK(token.is(types1));
    CHECK_FALSE(token.is(types2));
  }
}

TEST_CASE("token::type") {
  auto types = std::initializer_list<syntax::token_type> {
    syntax::token_type::Unknown,
    syntax::token_type::Id,
    syntax::token_type::Mut,
    syntax::token_type::LogOr
  };

  for (auto type: types) {
    auto token = syntax::token { type };
    CHECK(token.type() == type);
  }
}

TEST_CASE("token::size") {
  auto values = std::initializer_list<std::string_view> {
    "vdfnsk",
    "x",
    "def",
    "keyword"
  };

  for (auto value: values) {
    auto token = syntax::token {
      syntax::token_type::Unknown,
      value
    };
    CHECK(token.size() == value.size());
  }
}

TEST_CASE("token::value") {
  auto values = std::initializer_list<std::string_view> {
    "vdfnsk",
    "x",
    "def",
    "keyword"
  };

  for (auto value: values) {
    auto token = syntax::token {
      syntax::token_type::Unknown,
      value
    };
    CHECK(token.value() == value);
  }
}

