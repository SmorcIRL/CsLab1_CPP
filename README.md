### The idea

To find the shift between images, we can simply find the difference in position of the same object. So we can just use the ROI of this object as a template for template matching.

It's possible to use either amplitudes (with scaled differences between pixels) or phases. I mainly tried amplitudes because objects tend to look the same on different images. Phases are also ok, but CV sometimes gives worse results.

### Dependencies

The application uses two libraries:
- `matio` for reading Matlab files
- `opencv` for template matching

I'm not very familiar with CMake, so I installed both with `vcpkg`, like here https://vcpkg.io/en/getting-started.html

### Configuration

Configuration is done using command line args. Example for VS is in `CsLab1_CPP.vcxproj.user`.. The format is as follows

- Full path to the mat file
- Name of the source variable
- Name of the target variable
- Full path to folder to store some images (just for debugging)
- Threshold for CV template matching `[0,1]` (`TM_CCOEFF_NORMED` is used)
- ROI of template in source image: `"x y width height"`