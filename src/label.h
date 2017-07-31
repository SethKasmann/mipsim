#ifndef LABEL_H
#define LABEL_H

#include <iostream>
#include <vector>
#include <string>

class Label
{
public:
    Label(const std::string& line, std::size_t index)
        : loc(index)
    {
        // Set label.
        std::string::const_iterator it0, it1;
        for (it0 = line.begin(); it0 < line.end(), isspace(*it0); ++it0);
        for (it1 = it0; it1 < line.end(), *it1 != ':'; ++it1);
        name = std::string(it0, it1);
    }
    int loc;
    std::string name;
private:
};

#endif