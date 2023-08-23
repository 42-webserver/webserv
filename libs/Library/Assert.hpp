#ifndef ASSERT_HPP
# define ASSERT_HPP

#include <iostream>
#include <cstdlib>

#define CONCATENATE(x, y) x##y
#define LINE_VAR(x) CONCATENATE(x, __LINE__)
#define static_check(condition, message) typedef char LINE_VAR(STATIC_ASSERT_)[(condition) ? 1 : -1]

namespace ft {

    class Assert {
    public:
        enum Level {
            RUNTIME,
            DEBUG,
            COMPILE
        };

        static Level current_level;

        static void RuntimeAssert(bool condition, const char* message) {
            if (!condition) {
                std::cerr << "Runtime assertion failed: " << message << std::endl;
            }
        }

        static void DebugAssert(bool condition, const char* message, const char* file, int line) {
            if (!condition) {
                std::cerr << "Debug assertion failed at " << file << ":" << line << ": " << message << std::endl;
                std::abort();
            }
        }
    };

    Assert::Level Assert::current_level = Assert::RUNTIME;

#define COMPILE_ASSERT(condition, message) static_check(condition, message)

#define ASSERT(condition, message) do { \
    if (ft::Assert::current_level == ft::Assert::COMPILE) { \
        static_check(condition, message); \
    } else if (ft::Assert::current_level == ft::Assert::DEBUG) { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << message << std::endl; \
            std::abort(); \
        } \
    } else if (ft::Assert::current_level == ft::Assert::RUNTIME) { \
        if (!(condition)) { \
            std::cerr << "Assertion failed: " << message << std::endl; \
        } \
    } \
} while (false)
}

#endif