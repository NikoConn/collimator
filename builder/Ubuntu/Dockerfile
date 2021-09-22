FROM ubuntu:latest

#Requierd folders creation
RUN mkdir -p /collimator
RUN mkdir -p /deb

#Dependences
RUN apt-get update && DEBIAN_FRONTEND="noninteractive" apt-get install -y cmake g++ build-essential qt5-default libindi-dev libopencv-dev libcfitsio-dev zlib1g-dev

#start script
CMD cd /collimator && qmake Collimator.pro && make -j$(nproc) \
	&& cp /collimator/CollimatorDesktop/CollimatorDesktop /deb/CollimatorDesktop/bin/CollimatorDesktop \
	&& cp /collimator/CollimatorLib/libCollimatorLib.so /deb/CollimatorDesktop/usr/lib/libCollimatorLib.so.1 \
	&& cd /deb && dpkg-deb --build --root-owner-group CollimatorDesktop

