#include "forgottentones.hpp"

namespace tftp
{
    namespace str
    {
        std::vector<std::string> tokenize(const std::string &source, char delim)
        {
            std::vector<std::string> tokens;

            std::stringstream stream(source);
            std::string token;
            while (std::getline(stream, token, delim))
            {
                tokens.push_back(token);
            }
            return tokens;
        }

        std::vector<std::string> toLines(const std::string &str)
        {
            return tokenize(str, '\n');
        }

        std::string lastLine(const std::string &str)
        {
            // return tokenize(str, '\n').back();
            return tokenize(str, '\n').end()[-2];
        }

        std::string penultimateLine(const std::string &str)
        {
            return tokenize(str, '\n').end()[-3];
        }

        void rtrim(std::string &str)
        {
            str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch)
                                   { return !std::isspace(ch); })
                          .base(),
                      str.end());
        }

        bool startsWith(const std::string &str, const std::string &substr)
        {
            return str.rfind(substr, 0) == 0;
        }
    }
}
