#ifndef IOJSON_HPP
#define IOJSON_HPP


#include <cctype>
#include <exception>
#include <iostream>




namespace iojson {




struct whitespace
{
};


struct colon
{
    static const char CHAR = ':';
};


struct comma
{
    static const char CHAR = ',';
};


struct left_brace
{
    static const char CHAR = '{';
};


struct right_brace
{
    static const char CHAR = '}';
};


struct left_bracket
{
    static const char CHAR = '[';
};


struct right_bracket
{
    static const char CHAR = ']';
};



struct string
{
    std::string value;
};


struct key : public string
{
};




std::istream& operator>>(std::istream& is, whitespace& c)
{
    int next = is.peek();

    while (isspace(next))
    {
        is.get();
        next = is.peek();
    }

    return is;
}




std::istream& operator>>(std::istream& is, colon& c)
{
    whitespace ws;

    is >> ws;

    int next = is.peek();

    if (next == colon::CHAR)
    {
        is.get();
    }
    else
    {
        throw std::exception();
    }

    is >> ws;

    return is;
}




std::ostream& operator<<(std::ostream& os, const colon& c)
{
    os << " " << colon::CHAR << " ";

    return os;
}




std::istream& operator>>(std::istream& is, comma& c)
{
    whitespace ws;

    is >> ws;

    int next = is.peek();
    if (next == comma::CHAR)
    {
        is.get();
        next = is.peek();
    }
    else
    {
        throw std::exception();
    }

    is >> ws;

    return is;
}




std::ostream& operator<<(std::ostream& os, const comma& c)
{
    os << " " << comma::CHAR << " ";

    return os;
}




std::istream& operator>>(std::istream& is, left_brace& c)
{
    whitespace ws;

    is >> ws;

    int next = is.peek();
    if (next == left_brace::CHAR)
    {
        is.get();
    }
    else
    {
        char c = next;

        throw std::exception();
    }

    is >> ws;

    return is;
}




std::ostream& operator<<(std::ostream& os, const left_brace& c)
{
    os << " " << left_brace::CHAR << " ";

    return os;
}




std::istream& operator>>(std::istream& is, right_brace& c)
{
    whitespace ws;

    is >> ws;

    int next = is.peek();
    if (next == right_brace::CHAR)
    {
        is.get();
    }
    else
    {
        throw std::exception();
    }

    is >> ws;

    return is;
}




std::ostream& operator<<(std::ostream& os, const right_brace& c)
{
    os << " " << right_brace::CHAR << " ";

    return os;
}




std::istream& operator>>(std::istream& is, left_bracket& c)
{
    whitespace ws;

    is >> ws;

    int next = is.peek();
    if (next == left_bracket::CHAR)
    {
        is.get();
    }
    else
    {
        throw std::exception();
    }

    is >> ws;

    return is;
}




std::istream& operator>>(std::istream& is, right_bracket& c)
{
    whitespace ws;

    is >> ws;

    int next = is.peek();
    if (next == right_bracket::CHAR)
    {
        is.get();
    }
    else
    {
        throw std::exception();
    }

    is >> ws;

    return is;
}




std::istream& operator>>(std::istream& is, string& s)
{
    int next = is.peek();

    while (isspace(next))
    {
        is.get();
        next = is.peek();
    }

    if (next != '"')
    {
        return is;
    }
    else
    {
        is.get();
        next = is.peek();
    }

    while (next != '"')
    {
        s.value += is.get();
        next = is.peek();
    }

    is.get();
    next = is.peek();

    while (isspace(next))
    {
        is.get();
        next = is.peek();
    }

    return is;
}




std::ostream& operator<<(std::ostream& os, const string& s)
{
    os << "\"";

    os << s.value;

    os << "\"";
    return os;
}





} // namespace iojson




#endif//IOJSON_HPP
