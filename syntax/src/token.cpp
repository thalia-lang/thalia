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

#include <algorithm>
#include <ostream>

#include "thalia-syntax/token.hpp"

namespace thalia::syntax {
  extern auto token::is(std::initializer_list<token_type> types) const
    -> bool {
    auto const* result = std::find(types.begin(), types.end(), _type);
    return result != types.end();
  }

  extern auto operator<<(std::ostream& os, token const& token)
    -> std::ostream& {
    return os
      << token._type << "['"
      << token._value << "', "
      << token._line << ", "
      << token._col << "]";
  }

  extern auto operator<<(std::ostream& os, token_type type)
    -> std::ostream& {
    switch (type) {
      case token_type::Unknown:
        return os << "Unknown";
      case token_type::Eof:
        return os << "Eof";
      case token_type::Int:
        return os << "Int";
      case token_type::Id:
        return os << "Id";
      case token_type::Void:
        return os << "Void";
      case token_type::I8:
        return os << "I8";
      case token_type::I16:
        return os << "I16";
      case token_type::I32:
        return os << "I32";
      case token_type::I64:
        return os << "I64";
      case token_type::Use:
        return os << "Use";
      case token_type::Global:
        return os << "Global";
      case token_type::Local:
        return os << "Local";
      case token_type::Return:
        return os << "Return";
      case token_type::While:
        return os << "While";
      case token_type::If:
        return os << "If";
      case token_type::Else:
        return os << "Else";
      case token_type::Mut:
        return os << "Mut";
      case token_type::Def:
        return os << "Def";
      case token_type::Cast:
        return os << "Cast";
      case token_type::Minus:
        return os << "Minus";
      case token_type::Plus:
        return os << "Plus";
      case token_type::Mul:
        return os << "Mul";
      case token_type::Div:
        return os << "Div";
      case token_type::Mod:
        return os << "Mod";
      case token_type::Less:
        return os << "Less";
      case token_type::LessEqual:
        return os << "LessEqual";
      case token_type::Grt:
        return os << "Grt";
      case token_type::GrtEqual:
        return os << "GrtEqual";
      case token_type::Equal:
        return os << "Equal";
      case token_type::NotEqual:
        return os << "NotEqual";
      case token_type::RShift:
        return os << "RShift";
      case token_type::LShift:
        return os << "LShift";
      case token_type::LogNot:
        return os << "LogNot";
      case token_type::LogOr:
        return os << "LogOr";
      case token_type::LogAnd:
        return os << "LogAnd";
      case token_type::BitNot:
        return os << "BitNot";
      case token_type::BitAnd:
        return os << "BitAnd";
      case token_type::BitOr:
        return os << "BitOr";
      case token_type::Xor:
        return os << "Xor";
      case token_type::Assign:
        return os << "Assign";
      case token_type::MinusAssign:
        return os << "MinusAssign";
      case token_type::PlusAssign:
        return os << "PlusAssign";
      case token_type::MulAssign:
        return os << "MulAssign";
      case token_type::DivAssign:
        return os << "DivAssign";
      case token_type::ModAssign:
        return os << "ModAssign";
      case token_type::AndAssign:
        return os << "AndAssign";
      case token_type::OrAssign:
        return os << "OrAssign";
      case token_type::XorAssign:
        return os << "XorAssign";
      case token_type::RshAssign:
        return os << "RshAssign";
      case token_type::LshAssign:
        return os << "LshAssign";
      case token_type::LParen:
        return os << "LParen";
      case token_type::RParen:
        return os << "RParen";
      case token_type::LBrace:
        return os << "LBrace";
      case token_type::RBrace:
        return os << "RBrace";
      case token_type::LBracket:
        return os << "LBracket";
      case token_type::RBracket:
        return os << "RBracket";
      case token_type::Comma:
        return os << "Comma";
      case token_type::Semi:
        return os << "Semi";
      case token_type::Colon:
        return os << "Colon";
    }
  }
}

