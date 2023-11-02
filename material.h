#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtcommon.h"

class hit_record;

class material {
	public:
	virtual ~material() = default;

	virtual bool scatter(
			const ray& r_in, const hit_record& rec, color& attenuation, ray& scatter) const = 0;
};

#endif
