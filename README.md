# CollimatorDesktop

<img src="https://i.postimg.cc/QtgYqKcj/collimator.png">
CollimatorDesktop is a program for easy telescope collimation for Linux.

## Installation
CollimatorDesktop depends on <a href="https://opencv.org/">opencv4</a>, <a href="https://heasarc.gsfc.nasa.gov/fitsio/">cfitsio</a> and <a href="https://www.indilib.org/">indilib 1.9</a>

### Ubuntu / Debian dependeces
```sh
sudo apt-get install -y cmake g++ build-essential qt5-default libindi-dev libopencv-dev libcfitsio-dev zlib1g-dev
```

For other distros, you can install opencv4, cfitsio and indilib from sources or install thorugh package manager.

### Build
Enter Collimator dir and run:
```sh
qmake && make -j$(nproc)
```
