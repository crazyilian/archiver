#include "CommandLineArguments.h"

CommandLineArguments::CommandLineArguments(size_t argc, char** argv) {
    if (argc <= 0) {
        return;
    }
    command_ = argv[0];
    std::string current_option;
    bool no_option = true;
    for (size_t index = 1; index < argc; ++index) {
        std::string arg = argv[index];
        if (arg.starts_with('-')) {
            current_option = arg.starts_with("--") ? arg.substr(2) : arg.substr(1);
            option_values_[current_option];
            no_option = false;
        } else if (no_option) {
            no_option_values_.push_back(arg);
        } else {
            option_values_[current_option].push_back(arg);
        }
    }
}

bool CommandLineArguments::IsOptionExists(const std::string& s) const {
    return option_values_.contains(s);
}

size_t CommandLineArguments::GetOptionsNumber() const {
    return option_values_.size();
}

std::vector<std::string> CommandLineArguments::GetOptions() const {
    std::vector<std::string> options;
    options.reserve(option_values_.size());
    for (const auto& [option, values] : option_values_) {
        options.push_back(option);
    }
    return options;
}

std::vector<std::string> CommandLineArguments::GetValuesByOption(const std::string& s) const {
    if (!IsOptionExists(s)) {
        return {};
    }
    return option_values_.at(s);
}

std::vector<std::string> CommandLineArguments::GetValuesWithNoOption() const {
    return no_option_values_;
}

std::string CommandLineArguments::GetCommand() const {
    return command_;
}
