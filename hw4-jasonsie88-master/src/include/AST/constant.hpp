#ifndef __AST_CONSTANT_H
#define __AST_CONSTANT_H

#include "AST/PType.hpp"

#include <cstdint>
#include <memory>

class Constant {
  public:
    union ConstantValue {
        int32_t integer;
        double real;
        bool boolean;
        char *string;
    };
    Constant(const PTypeSharedPtr &new_p_type, ConstantValue &new_p_value);
    ~Constant();

    const PTypeSharedPtr &getTypePtr() const;
    const char *getConstantValueCString() const;

  public:
    PTypeSharedPtr m_type;
    ConstantValue m_value;
    mutable std::string m_constant_value_string;
    mutable bool m_constant_value_string_is_valid = false;
};

#endif
