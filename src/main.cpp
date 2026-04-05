#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

std::string DEVTOOLS_VERSION = "1.0";

std::string HELP_OUT_STR =
"DevTools 1.0\n"
"\n"
"Commands:\n"
"  class <name> [flags]\n"
"  module <name> [flags]\n"
"  shader <name> [flags]\n"
"  set_lang [flag]\n"
"  help\n"
"  exit\n"
"\n"
"Documentation:\n"
"https://github.com/prbAsalad19/devtools-linux\n";

enum class Language
{
    Cpp,
    Python,
    Rust    
};

enum class TokenType
{
    Command, 
    Flag,
    Option
};

std::vector<std::string> commands = 
{
    "class",
    "shader",
    "module",
    "set_lang",
    "help",
    "exit"
};

//==================================== flags ========================================
std::unordered_map<std::string, std::vector<std::string>> CCppCommandFlags = 
{
    {"class", {"-h", "-cpp", "-c", "-hpp"}},
    {"module", {"-h", "-cpp", "-hpp", "-c"}}
};

std::vector<std::string> langCommandFlags = 
{
    "-cpp",
    "-py",
    "-rs"
};

std::vector<std::string> shaderCommandFlags = 
{
    "-vert", 
    "-frag", 
    "-geom", 
    "-comp",
    "-xs", 
    "-xsh", 
    "-glsl", 
    "-sha", 
    "-v", 
    "-f"
};
//===================================================================================

void clear_screen()
{
    std::cout << "\033[2J\033[H" << std::endl;
}

struct Token
{
    TokenType type;
    std::string value;
};

std::string get_line()
{
    std::string command = "";
    std::cout << ">> ";
    std::getline(std::cin, command);
    return command;
}

std::vector<Token> parse_line(const std::string& line)
{
    std::vector<Token> tokens;

    std::string tokenValue;

    bool isToken = false;
    bool isFlag = false;
    bool isOption = false;

    for (char c : line)
    {
        if (c == ' ')
        {
            if (isToken)
            {
                Token t;

                if (isFlag)
                {
                    t.type = TokenType::Flag;
                    isFlag = false;
                    isOption = true;
                }
                else if (isOption)
                {
                    t.type = TokenType::Option;
                }
                else
                {
                    t.type = TokenType::Command;
                    isOption = true;
                }

                t.value = tokenValue;
                tokens.push_back(t);

                isToken = false;
            }
        }
        else if (c == '-')
        {
            if (!isToken)
            {
                tokenValue.clear();
                isToken = true;
            }

            tokenValue += '-';
            isFlag = true;
            isOption = false;
        }
        else
        {
            if (!isToken)
            {
                tokenValue.clear();
                isToken = true;
            }

            tokenValue += c;
        }
    }

    if (isToken)
    {
        Token t;

        if (isFlag)
            t.type = TokenType::Flag;
        else if (isOption)
            t.type = TokenType::Option;
        else
            t.type = TokenType::Command;

        t.value = tokenValue;
        tokens.push_back(t);
    }

    return tokens;
}

int handle_command(std::vector<Token>& tokens, Language& currentLang, std::string& invalidFlag)
{
    int commandID = -1;
    std::string optionValue = "";
    std::vector<std::string> flagsUsed;

    if (tokens.empty())
        return 4;

    for (const Token& t : tokens)
    {
        if (t.type == TokenType::Command)
        {
            for (size_t i = 0; i < commands.size(); i++)
            {
                if (t.value == commands[i])
                {
                    commandID = i;
                    break;
                }
            }
            break;
        }
    }

    if (commandID == -1)
        return 1;

    std::string commandName = commands[commandID];

    if (commandName == "exit")
        return -1;

    if (commandName == "help")
    {
        std::cout << HELP_OUT_STR << std::endl;
        return 0;
    }

    for (const Token& t : tokens)
    {
        if (t.type == TokenType::Flag)
        {
            int flagID = -1;

            if (commandName == "shader")
            {
                for (size_t i = 0; i < shaderCommandFlags.size(); i++)
                {
                    if (t.value == shaderCommandFlags[i])
                    {
                        flagID = i;
                        flagsUsed.push_back(t.value);
                        break;
                    }
                }

                if (flagID == -1)
                {
                    invalidFlag = t.value;
                    return 2;
                }
            }
            else if (commandName == "set_lang")
            {
                for (size_t i = 0; i < langCommandFlags.size(); i++)
                {
                    if (t.value == langCommandFlags[i])
                    {
                        flagID = i;
                        flagsUsed.push_back(t.value);
                        break;
                    }
                }

                if (flagID == -1)
                {
                    invalidFlag = t.value;
                    return 2;
                }
            }
            else if (currentLang == Language::Cpp)
            {
                auto& flags = CCppCommandFlags[commandName];

                for (size_t i = 0; i < flags.size(); i++)
                {
                    if (t.value == flags[i])
                    {
                        flagID = i;
                        flagsUsed.push_back(t.value);
                        break;
                    }
                }

                if (flagID == -1)
                {
                    invalidFlag = t.value;
                    return 2;
                }
            }
            else if (currentLang == Language::Python)
            {
                invalidFlag = t.value;
                return 3;
            }
            else if (currentLang == Language::Rust)
            {
                invalidFlag = t.value;
                return 3;
            }
        }
        else if (t.type == TokenType::Option)
        {
            optionValue = t.value;
        }
    }

    auto create_header = [&](const std::string& name)
    {
        std::ofstream file(name);
        file << "#pragma once\n\n";
    };

    auto create_source = [&](const std::string& name)
    {
        std::ofstream file(name);
        file << "#include \"" << optionValue << ".h\"\n\n";
    };

    auto create_file = [&](const std::string& name)
    {
        std::ofstream file(name);
    };

    // ================= class =================
    if (commandName == "class")
    {
        if (currentLang == Language::Cpp)
        {
            if (flagsUsed.empty())
            {
                create_header(optionValue + ".h");
                create_source(optionValue + ".cpp");

                std::cout << "Created: " << optionValue << ".h and " << optionValue << ".cpp\n";
            }

            for (auto& f : flagsUsed)
            {
                if (f == "-h")
                {
                    create_header(optionValue + ".h");
                    std::cout << "Created: " << optionValue << ".h\n";
                }
                else if (f == "-hpp")
                {
                    create_header(optionValue + ".hpp");
                    std::cout << "Created: " << optionValue << ".hpp\n";
                }
                else if (f == "-c")
                {
                    create_source(optionValue + ".c");
                    std::cout << "Created: " << optionValue << ".c\n";
                }
                else if (f == "-cpp")
                {
                    create_source(optionValue + ".cpp");
                    std::cout << "Created: " << optionValue << ".cpp\n";
                }
            }
        }
        else if (currentLang == Language::Python)
        {
            create_file(optionValue + ".py");
            std::cout << "Created: " << optionValue << ".py\n";
        }
        else if (currentLang == Language::Rust)
        {
            create_file(optionValue + ".rs");
            std::cout << "Created: " << optionValue << ".rs\n";
        }
    }

    // ================= module =================
    else if (commandName == "module")
    {
        std::string base = optionValue + "/";
        std::filesystem::create_directories(base); // crea la cartella

        if (currentLang == Language::Cpp)
        {
            if (flagsUsed.empty())
            {
                create_header(base + optionValue + ".h");
                create_source(base + optionValue + ".cpp");
            }
            else
            {
                for (auto& f : flagsUsed)
                {
                    if (f == "-h") create_header(base + optionValue + ".h");
                    else if (f == "-hpp") create_header(base + optionValue + ".hpp");
                    else if (f == "-c") create_source(base + optionValue + ".c");
                    else if (f == "-cpp") create_source(base + optionValue + ".cpp");
                }
            }
        }
        else if (currentLang == Language::Python)
        {
            create_file(base + optionValue + ".py");
        }
        else if (currentLang == Language::Rust)
        {
            create_file(base + optionValue + ".rs");
        }

        std::cout << "Created module: " << optionValue << std::endl; // stampa sempre
    }

    // ================= shader =================
    else if (commandName == "shader")
    {
        if (flagsUsed.empty())
        {
            create_file(optionValue + ".vert");
            create_file(optionValue + ".frag");

            std::cout << "Created: " << optionValue << ".vert and " << optionValue << ".frag\n";
        }

        for (auto& f : flagsUsed)
        {
            std::string ext = f.substr(1);
            create_file(optionValue + "." + ext);
            std::cout << "Created: " << optionValue << "." << ext << "\n";
        }
    }

    // ================= set lang =================
    else if (commandName == "set_lang")
    {
        if (flagsUsed.empty())
        {
            currentLang = Language::Cpp;
            std::cout << "Language set to cpp\n";
        }
        else if (flagsUsed.back() == "-cpp")
        {
            currentLang = Language::Cpp;
            std::cout << "Language set to cpp\n";
        }
        else if (flagsUsed.back() == "-py")
        {
            currentLang = Language::Python;
            std::cout << "Language set to python\n";
        }
        else if (flagsUsed.back() == "-rs")
        {
            currentLang = Language::Rust;
            std::cout << "Language set to rust\n";
        }
    }

    return 0;
}

int main()
{
    Language currentLang = Language::Cpp;
    std::vector<Token> tokens;
    int activeCommand;
    std::string invFlag = "";

    clear_screen();

    std::cout << "======= Dev Tools " << DEVTOOLS_VERSION << " =======" << std::endl;
    while (true)
    {
        std::string line;
        line = get_line();
        if (line != "")
        {
            tokens = parse_line(line);
            int lineCommandStatus = handle_command(tokens, currentLang, invFlag);
            switch (lineCommandStatus)
            {
                case -1:
                {
                    return 0;
                }
                case 0:
                {
                    break;
                }
                case 1:
                {
                    std::cout << "Unknown command" << std::endl;
                    break;
                }
                case 2:
                {
                    std::cout << "Invalid flag: " << invFlag << std::endl;
                    break;
                }
                case 3:
                {
                    std::cout << "Invalid flag for the selected language: " << invFlag << std::endl;
                    break;
                }
                case 4:
                {
                    std::cout << "Invalid command" << std::endl;
                    break;
                }
            }
        }
    }
    return 0;
}
