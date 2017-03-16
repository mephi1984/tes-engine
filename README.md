# What is it
It is OpenGL engine to show GUI made by OpenGL for Threads ES.

# Dependencies

* eigen 3.3.3 (math and geometry library)
* boost 1.63.0 (C++ extension library)
* boost GIL extension (for loading and processing textures)
* libpng-1.5.10 (to load PNG textures)
* libjpeg-9 (to load JPEG textures)

The latest 3 (Boost GIL, libpng, libjpeg) are already stored in libs repository here: git@gitlab.fishrungames.com:salmon-engine-core/libs.git

# Installation

Prepare separated folder for project and dependencies. In this folder, check out from following git repositories:
* git@gitlab.fishrungames.com:salmon-engine-core/libs.git
* git@gitlab.fishrungames.com:threads-es/tes-engine.git
* git@gitlab.fishrungames.com:threads-es/tes-demo.git

Download and unzip boost_1_63_0 from http://boost.org

Download and unzip latest eigen build form http://eigen.tuxfamily.org to "eigen" folder. Or you can simply check out Mercurial repository from https://bitbucket.org/eigen/eigen/

Your project folder will contain 5 subfolders now:
```
/.
/boost_1_63_0/
/eigen/
/libs/
/tes-engine/
```

# Building
At first, you need to build boost C++ libraries. Open command line, go to boost_1_63_0 and run 2 commands one by one:
```
bootstrap
b2 -j8 toolset=msvc-14.0 address-model=32 architecture=x86 link=static threading=multi runtime-link=shared --build-type=minimal stage --stagedir=stage/x86 --with-filesystem --with-regex --with-thread --with-signals --with-date_time --with-thread --with-system --with-exception --with-random
```

To test, you need  to create demo project. See here for details: http://gitlab.fishrungames.com/threads-es/tes-demo