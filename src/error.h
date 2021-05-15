#pragma once

#include <stdexcept>
#include <string>

namespace al
{
        ////////////////////////////////////////////////////////////////////////////////
        enum class etype { expected, unexpected, fatal };

        ////////////////////////////////////////////////////////////////////////////////
        class exception : public std::exception
        {
                std::string mNamespace;
                std::string mClass;
                std::string mFunction;
                std::string mMessage;
                etype       mType;

        public:
                exception(std::string namespaceName, std::string className, std::string functionName, std::string message, etype type)
                        : mNamespace(namespaceName), mClass{className}, mFunction{functionName}, mMessage{message}, mType{type} {}

                std::string getNamespace() const                { return mNamespace; }
                std::string getClass() const                    { return mClass; }
                std::string getFunction() const                 { return mFunction; }
                std::string getMessage() const                  { return mMessage; }
                etype getType() const                           { return mType; }

                virtual const char* what() const noexcept override;
        };

        ////////////////////////////////////////////////////////////////////////////////
        inline const char* exception::what() const noexcept
        {
                static std::string s;
                s.clear();

                s += "\n= = = = = = = = = = lovelace exception = = = = = = = = = =\n";
                s += "namespace:\t" + mNamespace + '\n';
                s += "class:\t\t" + mClass + '\n';
                s += "function:\t" + mFunction + '\n';
                s += "message:\t" + mMessage + '\n';
                s += "type:\t\t";
                s += [](etype type) {
                        switch (type) {
                                case etype::expected:
                                        return "expected\n";
                                case etype::unexpected:
                                        return "unexpected\n";
                                case etype::fatal:
                                        return "fatal\n";
                                default:
                                        return "unknown\n";
                        }
                }(mType);
                
                return s.c_str();
        }
}
