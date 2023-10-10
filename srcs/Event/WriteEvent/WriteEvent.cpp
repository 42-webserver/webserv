#include "../../../incs/Event/WriteEvent/WriteEvent.hpp"
#include "../../../incs/FtUtil/ft.hpp"

// To do: implement WriteEvent

WriteEvent::WriteEvent(WriteEventHandler* writeEventHandler): Event(writeEventHandler) {}
WriteEvent::~WriteEvent(void) {}

