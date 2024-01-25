#include "pdf-lib.hpp"

std::ostream & pdf_lib::scan_to_pdf(std::ostream & destination, std::initializer_list<std::string_view> image_names)
{
    for(auto const& name: image_names)
    {
        destination << name << "\n";
    }
    return destination;
}