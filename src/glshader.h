#pragma once

#include <glad/glad.h>

#include <string>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class shader
        {
                unsigned mId;
                int mType;
                std::string mSource;

                void load();
                void check();
        public:
                shader(int type, const std::string& source);

                ~shader()                       { glDeleteShader(mId); }

                shader(const shader&);
                shader& operator=(const shader&);

                shader(shader&&);
                shader& operator=(shader&&);

                unsigned getId() const          { return mId; }
                int getType() const             { return mType; }
                std::string getSource() const   { return mSource; }
        };

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator==(const shader& first, const shader& second) { return first.getId() == second.getId(); }

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator!=(const shader& first, const shader& second) { return first.getId() != second.getId(); }
}

