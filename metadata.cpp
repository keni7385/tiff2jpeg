#include "metadata.h"

#include <exiv2/exiv2.hpp>

void copy_metadata(std::string const& program_name, std::string const& filename_source, std::string const& filename_target)
{
    // Adapted from: https://exiv2.org/doc/metacopy_8cpp-example.html
    Exiv2::XmpParser::initialize();

    Exiv2::Image::AutoPtr image_source = Exiv2::ImageFactory::open(filename_source);
    assert(image_source.get() != 0);
    image_source->readMetadata();

    Exiv2::Image::AutoPtr image_target = Exiv2::ImageFactory::open(filename_target);
    assert(image_target.get() != 0);
    image_target->setIptcData(image_source->iptcData());
    image_target->setExifData(image_source->exifData());
    image_target->exifData()["Exif.Image.Software"] = program_name;
    image_target->setXmpData(image_source->xmpData());
    image_target->writeMetadata();

    Exiv2::XmpParser::terminate();
}
