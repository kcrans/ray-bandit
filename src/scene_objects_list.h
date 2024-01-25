#ifndef SCENE_OBJECTS_LIST_H
#define SCENE_OBJECTS_LIST_H

#include "scene_objects.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class scene_objects_list : public scene_object {
    public:
    std::vector<shared_ptr<scene_object>> objects;

    scene_objects_list() {}
    scene_objects_list(shared_ptr<scene_object> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<scene_object> object) {
	    objects.push_back(object);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
	    hit_record temp_rec;
	    bool hit_anything = false;
	    auto closest_so_far = ray_t.max;

	    // Loop through objects and keep track of closest object
	    // according to the value of t
	    for (const auto& object : objects) {
		    if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
			    hit_anything = true;
			    closest_so_far = temp_rec.t;
			    rec = temp_rec;
		    }
	    }
	    
	    return hit_anything;
    }
};


#endif
