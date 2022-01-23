#include "tiff2jpeg.h"

#include <cstdio>
#include <iostream>
#include <jpeglib.h>
#include <string>
#include <tiffio.h>


void tiff2jpeg(std::string const& filename_tiff, std::string const& filename_jpeg)
{
    TIFF* tiff_file = TIFFOpen(filename_tiff.c_str(), "r");

    if (tiff_file) {
        uint32_t width, height;
        size_t pixels_number;
        uint32_t* raster_image;

        TIFFGetField(tiff_file, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tiff_file, TIFFTAG_IMAGELENGTH, &height);
        pixels_number = width * height;

        raster_image = static_cast<uint32_t*>( _TIFFmalloc(pixels_number * sizeof(uint32_t)));
        if (raster_image != NULL) {
            if (TIFFReadRGBAImage(tiff_file, width, height, raster_image, 0)) {
                write_jpeg_file(filename_jpeg, raster_image, width, height);
            }
            _TIFFfree(raster_image);
        }

        TIFFClose(tiff_file);
    }
}

void write_jpeg_file(std::string const& filename, uint32_t* tiff_buffer, uint32_t image_width, uint32_t image_height)
{
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;

    FILE* jpeg_file;
    JSAMPROW row_pointer[1];
    int row_stride;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    if ((jpeg_file = fopen(filename.c_str(), "wb")) == NULL) {
        std::cerr << "Can't open jpeg file " << filename << std::endl;
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, jpeg_file);

    cinfo.image_width = image_width;
    cinfo.image_height = image_height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = image_width * 3; /* JSAMPLEs per row in image_buffer */
    JSAMPLE* row_buffer = new JSAMPLE[row_stride];
    while (cinfo.next_scanline < cinfo.image_height) {

        for (auto w=0u; w < image_width; w++) {
            // TIFF stores images from the images bottom left corner.
            // JPEG expects from the upper left corner, so read it flipped:
            uint32_t tiff_pixel = tiff_buffer[(image_height-1 - cinfo.next_scanline)*image_width + w];
            row_buffer[w*3] = TIFFGetR(tiff_pixel);
            row_buffer[w*3 + 1] = TIFFGetG(tiff_pixel);
            row_buffer[w*3 + 2] = TIFFGetB(tiff_pixel);
        }
        row_pointer[0] = row_buffer;
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    delete[] row_buffer;

    jpeg_finish_compress(&cinfo);
    fclose(jpeg_file);
    jpeg_destroy_compress(&cinfo);
}
