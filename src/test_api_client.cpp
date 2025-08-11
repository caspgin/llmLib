#include <iostream>
#include <stdexcept>

#include "llm_api_client.h"

int main() {
    try {
        CallGemini();

    } catch (const std::runtime_error& e) {
        // This block catches the specific exception for "GetApiKey"
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        // This block catches any other standard C++ exception
        // that derives from std::exception.
        std::cerr << "Caught a standard exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        // This is the catch-all for anything else, even non-standard
        // exceptions. Use this as a last resort.
        std::cerr << "Caught an unknown, non-standard error." << std::endl;
        return 1;
    }
    return 0;
}
