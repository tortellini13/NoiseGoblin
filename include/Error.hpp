#pragma once

#include <iostream>
#include <string>

/// @brief Prints an error message to the console
/// @param error_name Name of the function or file this is called in
/// @param error_message Message to be written to console
/// @param fatal Determines whether to stop the program on error
inline void error(std::string error_name, std::string error_message, bool fatal = false)
{
    std::string message = "Error [" + error_name + "]: " + error_message;
    std::cerr << message << "\n";

    if (fatal)
        throw std::runtime_error(message);
} // end error