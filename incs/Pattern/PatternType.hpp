#ifndef PATTERNTYPE_HPP
# define PATTERNTYPE_HPP

typedef enum PatternType{
    FILE_WRITE = 0,
    FILE_READ,
    CGI_READ,
    DELETE,
    REDIRECTION,
    PatternSize
} PatternType;

typedef enum {
    FAILURE = 0,
    SUCCESS,
    WAITING
} e_pattern_Process_result;

#endif