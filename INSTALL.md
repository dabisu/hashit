## Compiling & Installing Hashit

```bash
$ cmake . 
$ make
$ sudo make install
```


**hashit** now uses the CMake building system.

To install **hashit** with a different prefix, cmake needs to be invoked with the proper environment variables, so to install sakura in `/usr` (default is `/usr/local`), you should type:

```bash
$ cmake -DCMAKE_INSTALL_PREFIX=/usr .
```

Other options that can be passed to cmake:

* -DCMAKE_BUILD_TYPE to specify if you want debug symbols ("Debug", default is "Release").
* -DINSTALL_PKGCONFIG_DIR to select another directory to install the pkgconfig file.
* -DINSTALL_MAN_DIR to select another directory to install the man pages.

**hashit** includes a pkg-config file, so just the pkg-config utility in the usual way to link your binary to hashit.


\
David Gómez <[david@pleyades.net](david@pleyades.net)>

\
Enjoy **hashit**!