#ifndef PDF_LIB_DEFINITION
#define PDF_LIB_DEFINITION 1
#include <string_view>
#include <ostream>
#include <initializer_list>

namespace pdf_lib{

    std::ostream & scan_to_pdf(std::ostream & destination, std::initializer_list<std::string_view> names);

    template<typename ... Images>
    std::ostream &scan_to_pdf(std::ostream & destination, Images ... image_names)
    {
        return scan_to_pdf(destination, {image_names...});
    }
}

#endif