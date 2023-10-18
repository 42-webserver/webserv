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

#endif