#ifndef MISC_H
#define MISC_H

#include <string>

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

std::string remove_comments(const std::string& line)
{
    std::string::const_iterator it;
    it = find(line.begin(), line.end(), '#');
    if (it != line.end())
        return std::string(line.begin(), it);
    else
        return line;
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