# Installing Nazzida

Nazzida is currently available for Sailfish OS via the Jolla Store and [OpenRepos community repositories](https://openrepos.net/content/huessenbergnetz/nazzida). You can also download the sources and build it by yourself.

## Sailfish OS building instructions

To build Nazzida for Sailfish OS, you need the [Sailfish OS Application SDK](https://docs.sailfishos.org/Tools/Sailfish_SDK/) and some other tools.

### Required tools
* [Sailfish OS Application SDK](https://docs.sailfishos.org/Tools/Sailfish_SDK/) (required to build the code)
* [git](https://git-scm.com/) (required to get the code)
* [lrelease](https://doc.qt.io/qt-5/linguist-manager.html) (required to generate the translations)
* [librsvg-tools](https://wiki.gnome.org/Projects/LibRsvg) (required to generate the icons)
* [bc](http://www.gnu.org/software/bc/bc.html) (required to generate the icons)

When using the Sailfish OS Application SDK, all requirements should be pulled in automatically.

### Get the code
At first you have to clone this repository including all submodules ([libfirfuorida](https://github.com/Huessenbergnetz/libfirfuorida/), [HBN SFOS Components](https://github.com/Huessenbergnetz/HBN_SFOS_Components), [HBN SFOS Cmake](https://github.com/Huessenbergnetz/HBN_SFOS_Cmake)).

    git clone --recursive https://github.com/Huessenbergnetz/nazzida.git

### Configure the project
Nazzida should already be configured properly to be build inside Sailfish OS SDK as this is the default build target.

### Build the code
Now you are ready to build and run Nazzida on your Sailfish OS device or in the emulatoer.
