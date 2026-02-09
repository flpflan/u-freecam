#include "schema.inc"
#include "config.hpp"

#include <string>

namespace user_config
{
    std::string schema()
    {
        return std::string(reinterpret_cast<const char *>(__schema_min_json_gz), __schema_min_json_gz_len);
    }
}
