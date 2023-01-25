#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class CommandLineArguments {
public:
    CommandLineArguments(size_t argc, char** argv);

    bool IsOptionExists(const std::string& s) const;
    size_t GetOptionsNumber() const;
    std::vector<std::string> GetOptions() const;
    std::vector<std::string> GetValuesByOption(const std::string& s) const;
    std::vector<std::string> GetValuesWithNoOption() const;
    std::string GetCommand() const;

private:
    std::string command_;
    std::vector<std::string> no_option_values_;
    std::unordered_map<std::string, std::vector<std::string>> option_values_;
};
