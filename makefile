SOURCE = ./src/
#BUILD = ./src/
raytracer: main.cpp a.out, camera.h, color.h, hittable.h, hittable_list.h, interval.h, material.h, ray.h, rtcommon.h, sphere.h, stb_image_write.h, vec3.h
	g++ -std=c++11 -Werror -o raytracer $(SOURCE)main.cpp

