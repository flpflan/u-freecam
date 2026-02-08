#include "json.hpp"

#include <optional>
#include <string>
#include <stdexcept>

using json::Json;
using JsonObject = Json::Object;
using JsonArray = Json::Array;

class JsonParser
{
public:
    explicit JsonParser(std::string_view input) : src(input) {}

    std::optional<Json> parse()
    {
        skipWhitespace();
        Json v = parseValue();
        skipWhitespace();
        if (!eof()) throw std::runtime_error("Extra characters after JSON");
        return v;
    }

private:
    std::string_view src;
    size_t pos = 0;

    bool eof() const { return pos >= src.size(); }

    char peek() const { return eof() ? '\0' : src[pos]; }

    char get() { return eof() ? '\0' : src[pos++]; }

    void skipWhitespace()
    {
        while (!eof() && std::isspace(static_cast<unsigned char>(peek())))
            ++pos;
    }

    Json parseValue()
    {
        skipWhitespace();
        char c = peek();

        if (c == '"') return Json{parseString()};
        if (c == '{') return Json{parseObject()};
        if (c == '[') return Json{parseArray()};
        if (c == 't') return parseLiteral("true", true);
        if (c == 'f') return parseLiteral("false", false);
        if (c == 'n') return parseLiteral("null", nullptr);
        if (c == '-' || std::isdigit(c)) return Json{parseNumber()};

        throw std::runtime_error("Invalid JSON value");
    }

    std::string parseString()
    {
        if (get() != '"') throw std::runtime_error("Expected '\"'");

        std::string result;
        while (true)
        {
            if (eof()) throw std::runtime_error("Unterminated string");

            char c = get();
            if (c == '"') break;

            if (c == '\\')
            {
                char esc = get();
                switch (esc)
                {
                case '"':
                    result.push_back('"');
                    break;
                case '\\':
                    result.push_back('\\');
                    break;
                case '/':
                    result.push_back('/');
                    break;
                case 'b':
                    result.push_back('\b');
                    break;
                case 'f':
                    result.push_back('\f');
                    break;
                case 'n':
                    result.push_back('\n');
                    break;
                case 'r':
                    result.push_back('\r');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                default:
                    throw std::runtime_error("Invalid escape");
                }
            }
            else
            {
                result.push_back(c);
            }
        }
        return result;
    }

    double parseNumber()
    {
        size_t start = pos;

        if (peek() == '-') get();
        while (std::isdigit(peek()))
            get();

        if (peek() == '.')
        {
            get();
            while (std::isdigit(peek()))
                get();
        }

        if (peek() == 'e' || peek() == 'E')
        {
            get();
            if (peek() == '+' || peek() == '-') get();
            while (std::isdigit(peek()))
                get();
        }

        return std::stod(std::string(src.substr(start, pos - start)));
    }
    JsonArray parseArray()
    {
        JsonArray arr;
        get(); // '['
        skipWhitespace();

        if (peek() == ']')
        {
            get();
            return arr;
        }

        while (true)
        {
            arr.push_back(parseValue());
            skipWhitespace();

            char c = get();
            if (c == ']') break;
            if (c != ',') throw std::runtime_error("Expected ',' or ']'");
        }
        return arr;
    }
    JsonObject parseObject()
    {
        JsonObject obj;
        get(); // '{'
        skipWhitespace();

        if (peek() == '}')
        {
            get();
            return obj;
        }

        while (true)
        {
            skipWhitespace();
            if (peek() != '"') throw std::runtime_error("Expected string key");

            std::string key = parseString();
            skipWhitespace();

            if (get() != ':') throw std::runtime_error("Expected ':'");

            obj.emplace(std::move(key), parseValue());
            skipWhitespace();

            char c = get();
            if (c == '}') break;
            if (c != ',') throw std::runtime_error("Expected ',' or '}'");
        }
        return obj;
    }
    template <typename T>
    Json parseLiteral(std::string_view text, T value)
    {
        if (src.substr(pos, text.size()) != text) throw std::runtime_error("Invalid literal");

        pos += text.size();
        return Json{value};
    }
};

namespace json
{

    std::optional<Json> parse(const std::string &raw)
    {
        try
        {
            return JsonParser(raw).parse();
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
}
