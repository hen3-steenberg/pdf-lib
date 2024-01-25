#include "pdf-lib.hpp"
#include <iostream>


int main()
{
    pdf_lib::scan_to_pdf(std::cout, "image1.jpg", "image2.png") << std::endl;
    return 0;
}