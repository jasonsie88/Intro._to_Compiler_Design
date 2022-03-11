#include "AST/constant.hpp"
#include "AST/PType.hpp"

#include <cstdlib>

const char *kTFString[] = {"false", "true"};

Constant::Constant(const PTypeSharedPtr &new_p_type, ConstantValue &new_p_value)
    : m_type(new_p_type), m_value(new_p_value) {}

Constant::~Constant() {
    if (m_type->getPrimitiveType() == PType::PrimitiveTypeEnum::kStringType) {
        free(m_value.string);
    }
}

const PTypeSharedPtr &Constant::getTypePtr() const { return m_type; }

// logical constness
const char *Constant::getConstantValueCString() const {
    if (!m_constant_value_string_is_valid) {
        switch (m_type->getPrimitiveType()) {
        case PType::PrimitiveTypeEnum::kIntegerType:
            m_constant_value_string = std::to_string(m_value.integer);
            break;
        case PType::PrimitiveTypeEnum::kRealType:
            m_constant_value_string = std::to_string(m_value.real);
            break;
        case PType::PrimitiveTypeEnum::kBoolType:
            m_constant_value_string = kTFString[m_value.boolean];
            break;
        case PType::PrimitiveTypeEnum::kStringType:
            m_constant_value_string = m_value.string;
            break;
        case PType::PrimitiveTypeEnum::kVoidType:
        default:
            break;
        }
        m_constant_value_string_is_valid = true;
    }
    return m_constant_value_string.c_str();
}
