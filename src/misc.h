#ifndef MISC_H
#define MISC_H

#include <string>
#include <algorithm>
#include <cassert>

template<class It, class X>
inline void safe_increment(It& it, const X& x, int n)
{
    if (std::distance(it, x.end()) >= n)
        std::advance(it, n);
}

/* Converts a string to all lowercase characters */
inline void lowercase(std::string& s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        if (isalpha(*it))
            *it = tolower(*it);
    }
}

/* Returns the correct escape character. Assumes c follows a '\' */
inline char get_escape(char c)
{
    assert(c == 'n' || c == 't');
    return c == 'n' ? '\n' : '\t';
}

/* Removes the comments from a line of source code */
inline void remove_comments(std::string& line)
{
    line = std::string(line.begin(), std::find(line.begin(), line.end(), '#'));
}

/* Checks to see if a line contains only whitespace */
inline bool is_whitespace(const std::string& line)
{
    return line.find_first_not_of("\t\n ") == std::string::npos;
}

/* Returns the first character from a line of source code that is not
   whitespace */
inline char first_char(const std::string& line)
{
    for (auto it = line.begin(); it != line.end(); ++it)
    {
        if (isspace(*it))
            continue;
        return *it;
    }
}

#endif