#include "AST/PType.hpp"
#include<string.h>

const char *kTypeString[] = {"void", "integer", "real", "boolean", "string"};

PType::PType(PrimitiveTypeEnum new_type) : m_type(new_type) {}

PType::PType(PrimitiveTypeEnum new_type, std::vector<uint64_t> new_dims) : m_type(new_type) 
{
    m_dimensions = std::move(new_dims);
}

bool PType::operator==(PType b)
{
    if(strcmp(getPTypeCString(), b.getPTypeCString())!= 0){
        return false;
    }else{
        return true;
    }
        
}

void PType::setDimensions(std::vector<uint64_t> &new_dims) {
    m_dimensions = std::move(new_dims);
}

const PType::PrimitiveTypeEnum PType::getPrimitiveType() const { 
            return m_type; 
    }

// logical constness
const char *PType::getPTypeCString() const {
    if (!m_type_string_is_valid) {
        m_type_string += kTypeString[static_cast<int>(m_type)];
        if (m_dimensions.size() != 0) {
            m_type_string += " ";
            for (const auto &d : m_dimensions) {
                m_type_string += "[" + std::to_string(d) + "]";
            }
        }
        m_type_string_is_valid = true;
    }

    return m_type_string.c_str();
}
