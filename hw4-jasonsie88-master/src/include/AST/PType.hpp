#ifndef __AST_PTYPE_H
#define __AST_PTYPE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class PType;

typedef std::shared_ptr<const PType> PTypeSharedPtr;

class PType {
  public:
    enum class PrimitiveTypeEnum : uint8_t {
        kVoidType,
        kIntegerType,
        kRealType,
        kBoolType,
        kStringType
    };

    PType(PrimitiveTypeEnum new_type);
    PType(PrimitiveTypeEnum new_type, std::vector<uint64_t> new_dims);
    ~PType() = default;

    bool operator==(PType b);
    bool operator!=(PType b){return !(*this == b);};
    void setDimensions(std::vector<uint64_t> &new_dims);
    std::vector<uint64_t> get_dim(){
        return m_dimensions;
      }

    const PrimitiveTypeEnum getPrimitiveType() const;
    const char *getPTypeCString() const;

  private:
    PrimitiveTypeEnum m_type;
    std::vector<uint64_t> m_dimensions;
    mutable std::string m_type_string;
    mutable bool m_type_string_is_valid = false;
};

#endif
