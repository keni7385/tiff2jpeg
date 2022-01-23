#include <string>
#include <exiv2/version.hpp>
#include <filesystem>
#include <iostream>
#include <jconfig.h>
#include <optional>
#include <string_view>
#include <tiffvers.h>

#include "config.h"
#include "metadata.h"
#include "tiff2jpeg.h"

using namespace std;

void print_versions(std::ostream&);
void print_usage(std::ostream&);
int error_usage(std::string_view);
int error_usage(std::ostream&);

int main([[maybe_unused]] int argc, char* argv[]) {

    string_view first_flag{ argv[1] };
    if (first_flag == "-v" || first_flag == "--version") {
        print_versions(cout);
        return 0;
    }
    else if (first_flag == "-h" || first_flag == "--help") {
        print_usage(cout);
        return 0;
    }

    if (argc < 5)
        return error_usage("Error: incorrect usage of tiff2jpeg.");

    optional<string> filename_source;
    optional<string> filename_target;
    optional<string> software_name;
    for (auto i=1; i<argc; i+=2) {
        string_view argument{ argv[i] };
        if (argument == "-t" || argument == "--tiff_file")
            filename_source = argv[i + 1];
        else if (argument == "-j" || argument == "--jpeg_file")
            filename_target = argv[i + 1];
        else if (argument == "-s" || argument == "--software")
            software_name = argv[i + 1];
        else {
            return error_usage(cerr << "Error: invalid argument `" << argument << "`.");
        }
    }

    if (!filename_source.has_value())
        return error_usage("Error: no TIFF file given.");

    if (!filename_target.has_value())
        return error_usage("Error: no jpeg file given.");

    if(!std::filesystem::exists(*filename_source))
        return error_usage(cerr << "Error: file `" << *filename_source << "` does not exists.");

    tiff2jpeg(*filename_source, *filename_target);
    copy_metadata(software_name.value_or("tiff2jpeg"), *filename_source, *filename_target);

    return 0;
}


void print_versions(std::ostream& os)
{
    os << "tiff2jpeg " << TIFF2JPEG_VERSION << "\n"
       << "Copyright (C) 2022 Andrea Corsini (keni7385).\n\n"

       << "Build-dependency library versions:\n"
       << "  libtiff version: " << TIFFLIB_VERSION << "\n"
       << "  libjpeg version: " << JPEG_LIB_VERSION
#ifdef LIBJPEG_TURBO_VERSION
#define _TIFF2JPEG_STR(s) #s
#define TIFF2JPEG_STR(s) _TIFF2JPEG_STR(s)
       << " (libjpeg-turbo " << TIFF2JPEG_STR(LIBJPEG_TURBO_VERSION) << ")"
#undef TIFF2JPEG_STR
#undef _TIFF2JPEG_STR
#endif
       << "\n"
       << "  exiv2   version: " << Exiv2::versionString() << endl << endl;
}

void print_usage(std::ostream& os)
{
    os << "Usage: tiff2jpeg <arguments>\n\n"

       << "Arguments for information:\n"
       << "  -h / --help\n"
       << "    Print this help message.\n\n"
       << "  -v / --version\n"
       << "    Print tiff2jpeg and build dependencies versions\n\n"

       << "Arguments to convert TIFF_FILENAME to JPEG_FILENAME:\n"
       << "  -t / --tiff_file TIFF_FILENAME (required)\n"
       << "  -j / --jepg_file JPEG_FILENAME (required)\n"
       << "  -s / --software SOFTWARE_NAME (optional)\n"
       << endl;
}

int error_usage(std::string_view error_message)
{
    cerr << error_message << endl;
    print_usage(cerr);
    return -1;
}

int error_usage(std::ostream& os)
{
    os << endl;
    print_usage(cerr);
    return -1;
}
