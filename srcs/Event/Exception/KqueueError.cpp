#include <Event/Exception/KqueueError.hpp>

const char	*KqueueError::what(void) const throw() { return ("Kqueue error"); }