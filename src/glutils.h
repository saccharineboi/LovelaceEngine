#pragma once

#include <glad/glad.h>

namespace al::gl::utils
{
        ////////////////////////////////////////////////////////////////////////////////
        template <typename IT>
        constexpr int findEboType()
        {
                static_assert(
                                sizeof(IT) == sizeof(unsigned char)  ||
                                sizeof(IT) == sizeof(unsigned short) ||
                                sizeof(IT) == sizeof(unsigned int)
                );
                switch (sizeof(IT)) {
                        case sizeof(unsigned char):
                                return GL_UNSIGNED_BYTE;
                        case sizeof(unsigned short):
                                return GL_UNSIGNED_SHORT;
                        case sizeof(unsigned int):
                                return GL_UNSIGNED_INT;
                        default:
                                return -1;
                }
        }
}
