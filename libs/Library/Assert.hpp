/*
#ifndef ASSERT_HPP
# define ASSERT_HPP

#include <iostream>
#include <cstdlib>

#define CONCATENATE(x, y) x##y
#define LINE_VAR(x) CONCATENATE(x, __LINE__)
#define STATIC_ASSERT(condition, message) typedef char LINE_VAR(STATIC_ASSERT_)[(condition) ? 1 : -1]

namespace ft {
    enum Level {
        RUNTIME,
        Debug,
    };
    class Assert {
    public:
        class ScopedAssertChange{
        private:
            Level _tmp;
        public:
            ScopedAssertChange(Level change) {
                _tmp = current_level();
                current_level() = change;
            }
            ~ScopedAssertChange(){
                current_level() = _tmp;
            }
         };

        static Level& current_level() {
            static Level level;
#ifdef DEBUG
            level = Debug;
#else
            level = RUNTIME;
#endif
            return level;
        }
        static void RuntimeAssert(bool condition, const char* message) {
            if (!condition) {
                std::cerr << "Runtime assertion failed: " << message << std::endl;
            }
        }

        static void DebugAssert(bool condition, const char* message) {
            if (!condition) {
                std::cerr << "Debug assertion failed at " << __FILE__ << ":" << __LINE__ << ": " << message << std::endl;
                std::abort();
            }
        }
     
        static void _assert(bool condition, const char *msg){
            if (current_level() == RUNTIME)
                RuntimeAssert(condition, msg);
            else   
                DebugAssert(condition, msg);
        }
    };
}

#define COMPILE_ASSERT(condition, message) static_check(condition, message)

#define ASSERT(condition, message) do { \
    if ( == ft::Assert::COMPILE) { \
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

#endif
*/
