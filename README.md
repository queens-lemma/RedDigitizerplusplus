# RedDigitizer++

A [CAEN](https://www.caen.it/) C++ wrapper for their C API.

CAEN is an italian company that produces digitizer, power supplies, 
amplifiers, and many others types of electronics for particle/nuclear physics.

Their code is great, but it is not beginner friendly. This 
header-only library will help with that. It uses C++20, and it is written for 
readability and memory safety.

This code also contains several examples that can be used to inspire your 
own code writing.

# Installation

There are two ways to install this library.

1. Copy everything inside the "include" folder into your own include folder. 
   Done!
2. Clone this github repository anywhere in your project and in your 
   "CMakeLists.txt" add a line called `add_subdirectory
   (LOCATION_OF_THIS_LIBRARY)`. Then, in `target_link_libraries` add
   `RedDigitizer++::RedDigitizer++`. Done!

# About loggers

RedDigitizer++ library was written to use a logger as dependency injection.
The logger can output to a console, to a log file, or something else. 
There are two basic loggers included in this library, but they have limited 
features. (Spdlog)[https://github.com/gabime/spdlog] is a feature-rich 3rd-party 
library for logging and RedDigitizer++ was designed with it in mind.