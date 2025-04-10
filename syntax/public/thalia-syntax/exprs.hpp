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

#ifndef _THALIA_SYNTAX_EXPRS_
#define _THALIA_SYNTAX_EXPRS_

#include <memory>

#include "node.hpp"
#include "token.hpp"

namespace thalia::syntax {
  enum class expr_type {
    Assign,
    Binary,
    Unary,
    Paren,
    BaseLit,
    Id,
    DataType
  };

  using expression = node<expr_type>;

  class expr_assign: public expression {
    public:
      explicit expr_assign(
        token const& operation,
        std::shared_ptr<expression> const& target,
        std::shared_ptr<expression> const& value
      ) : expression(expr_type::Assign)
        , _operation(operation)
        , _target(target)
        , _value(value) {}

      token operation() const { return _operation; }
      std::shared_ptr<expression> target() const { return _target; }
      std::shared_ptr<expression> value() const { return _value; }

    private:
      token _operation;
      std::shared_ptr<expression> _target;
      std::shared_ptr<expression> _value;
  };

  class expr_binary: public expression {
    public:
      explicit expr_binary(
        token const& operation,
        std::shared_ptr<expression> const& lhs,
        std::shared_ptr<expression> const& rhs
      ) : expression(expr_type::Binary)
        , _operation(operation)
        , _lhs(lhs)
        , _rhs(rhs) {}

      token operation() const { return _operation; }
      std::shared_ptr<expression> lhs() const { return _lhs; }
      std::shared_ptr<expression> rhs() const { return _rhs; }

    private:
      token _operation;
      std::shared_ptr<expression> _lhs;
      std::shared_ptr<expression> _rhs;
  };

  class expr_unary: public expression {
    public:
      explicit expr_unary(
        token const& operation,
        std::shared_ptr<expression> const& value
      ) : expression(expr_type::Unary)
        , _operation(operation)
        , _value(value) {}

      token operation() const { return _operation; }
      std::shared_ptr<expression> value() const { return _value; }

    private:
      token _operation;
      std::shared_ptr<expression> _value;
  };

  class expr_paren: public expression {
    public:
      explicit expr_paren(
        std::shared_ptr<expression> const& value
      ) : expression(expr_type::Paren)
        , _value(value) {}

      std::shared_ptr<expression> value() const { return _value; }

    private:
      std::shared_ptr<expression> _value;
  };

  class expr_base_lit: public expression {
    public:
      explicit expr_base_lit(token const& target)
        : expression(expr_type::BaseLit)
        , _target(target) {}

      token operation() const { return _target; }

    private:
      token _target;
  };

  class expr_id: public expression {
    public:
      explicit expr_id(token const& target)
        : expression(expr_type::Id)
        , _target(target) {}

      token operation() const { return _target; }

    private:
      token _target;
  };

  class expr_data_type: public expression {
    public:
      explicit expr_data_type(token const& target)
        : expression(expr_type::DataType)
        , _target(target) {}

      token operation() const { return _target; }

    private:
      token _target;
  };
}

#endif // _THALIA_SYNTAX_EXPRS_

