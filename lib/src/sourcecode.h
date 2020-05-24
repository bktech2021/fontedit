#ifndef SOURCECODE_H
#define SOURCECODE_H

#include "fontdata.h"
#include <string>
#include <iostream>
#include <variant>

namespace f2b  {

namespace SourceCode {

/// A struct representing a Tabulation character
struct Tab {};

/// A struct representing a repeated space character (used instead of tabulator)
struct Space { std::size_t num_spaces; };

/// Indentation can be either a Tab, or multiple Spaces.
using Indentation = std::variant<Tab,Space>;

/**
 * This namespace gathers building blocks for a source code generator:
 * - begin (source code file)
 * - begin array
 * - begin array row
 * - constant definition
 * - value (e.g. byte)
 * - comment
 * - line break with an array
 * - end array
 * - end (source code file).
 *
 * All the structs in this namespace are templates taking Source Code Format
 * as a template parameter. They also take parameters as necessary (e.g.
 * \c array_name for \c BeginArray).
 * All the structs define the \c operator<< which outputs the relevant source
 * code idiom to the given output stream. \c FontSourceCodeGenerator uses them
 * with an \c std::stringstream to output the resulting source code of the font face.
 */

namespace Idiom {

template<typename T>
struct Begin {
    std::string font_name;
    font::Size font_size;
    std::string timestamp;
};

template<typename T, typename V>
struct Constant {
    std::string name;
    V value;
};

template<typename T, typename V>
struct BeginArray {
    std::string array_name;
};

template<typename T, typename V>
struct BeginArrayRow {
    Indentation tab;
};

template<typename T, typename V>
struct Value {
    V value;
};

template<typename T, typename V = void>
struct Comment {
    std::string comment;
};

template<typename T, typename V = void>
struct ArrayLineBreak {};

template<typename T, typename V = void>
struct EndArray {};

template<typename T>
struct End {};

} // namespace Idiom

inline bool operator==(const Indentation& lhs, const Indentation& rhs) {
    if (std::holds_alternative<Tab>(lhs) && std::holds_alternative<Tab>(rhs)) {
        return true;
    }
    if (std::holds_alternative<Space>(lhs) && std::holds_alternative<Space>(rhs)) {
        return std::get<Space>(lhs).num_spaces == std::get<Space>(rhs).num_spaces;
    }
    return false;
}

inline std::ostream& operator<<(std::ostream& o, const Indentation& t) {
    if (std::holds_alternative<Space>(t)) {
        o << std::string(std::get<Space>(t).num_spaces, ' ');
    } else {
        o << "\t";
    }
    return o;
}

} // namespace SourceCode
} // namespace f2b

#endif // SOURCECODE_H
