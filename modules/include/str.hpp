#include <string>
#include <vector>

namespace tftp
{
    namespace str
    {
        std::vector<std::string> tokenize(const std::string &source, char delim);
        std::vector<std::string> toLines(const std::string &str);
        std::string lastLine(const std::string &str);
        std::string penultimateLine(const std::string &str);
        void rtrim(std::string &str);
        bool startsWith(const std::string &str, const std::string &substr);
    }
}
