#ifndef MISC_H
#define MISC_H

#include <string>
#include <algorithm>
#include <cassert>

template<class It, class X>
void safe_increment(It& it, const X& x, int n)
{
    if (std::distance(it, x.end()) >= n)
        std::advance(it, n);
}

void lowercase(std::string& s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        if (isalpha(*it))
            *it = tolower(*it);
    }
}

// Returns the correct escape character. Assumes c follows a '\'
char get_escape(char c)
{
    assert(c == 'n' || c == 't');
    return c == 'n' ? '\n' : '\t';
}

void remove_comments(std::string& line)
{
    line = std::string(line.begin(), std::find(line.begin(), line.end(), '#'));
    /*
    auto it = find(line.begin(), line.end(), '#');
    if (it != line.end())
        line.erase(it, line.end());
        */
}

bool is_whitespace(const std::string& line)
{
    return line.find_first_not_of("\t\n ") == std::string::npos;
}

char first_char(const std::string& line)
{
    for (auto it = line.begin(); it != line.end(); ++it)
    {
        if (isspace(*it))
            continue;
        return *it;
    }
}

#endif