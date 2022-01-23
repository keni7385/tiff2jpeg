#pragma once

#include <string>


void tiff2jpeg(std::string const&, std::string const&);
void write_jpeg_file(std::string const& filename, uint32_t* tiff_buffer, uint32_t image_width, uint32_t image_height);
