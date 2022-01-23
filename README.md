# tiff2jpeg

Utility to compress TIFF images via JPEG compression copying metadata.

## Dependencies

`tiff2jpeg` requires `libtiff`, `libjpeg` and `libexiv2`.

## Build

Generate a build system via cmake, e.g. Ninja:

``` sh
mkdir build
cd build
cmake -G Ninja ..
ninja
```
