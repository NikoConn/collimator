docker run \
	--mount type=bind,source="$(pwd)/../../Collimator",target="/collimator" \
	--mount type=bind,source="$(pwd)/deb",target="/deb" \
	collimator
	
