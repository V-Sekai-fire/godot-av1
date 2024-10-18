#include "register_types.h"

#include <godot_cpp/classes/engine.hpp>

#include <stdio.h>
#include "../deps/dav1d/build/include/dav1d/version.h"
#include "../deps/dav1d/include/dav1d/dav1d.h"

void print_video_info(const Dav1dPicture *pic) {
    if (pic == NULL) {
        printf("Invalid picture reference.\n");
        return;
    }

    // Print resolution
    printf("Resolution: %dx%d\n", pic->p.w, pic->p.h);

    // Print bit depth
    printf("Bit Depth: %d\n", pic->p.bpc);

    // Determine and print video format
    const char *format;
    switch (pic->p.layout) {
        case DAV1D_PIXEL_LAYOUT_I400:
            format = "Monochrome";
            break;
        case DAV1D_PIXEL_LAYOUT_I420:
            format = "4:2:0";
            break;
        case DAV1D_PIXEL_LAYOUT_I422:
            format = "4:2:2";
            break;
        case DAV1D_PIXEL_LAYOUT_I444:
            format = "4:4:4";
            break;
        default:
            format = "Unknown";
            break;
    }
    printf("Video Format: %s\n", format);
}

using namespace godot;

void initialize_godot_av1_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}    
	Dav1dPicture example_pic;
    example_pic.p.w = 1920;
    example_pic.p.h = 1080;
    example_pic.p.bpc = 10;
    example_pic.p.layout = DAV1D_PIXEL_LAYOUT_I420;
    print_video_info(&example_pic);
}

void uninitialize_godot_av1_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
	// Initialization.
	GDExtensionBool GDE_EXPORT godot_av1_library_init(GDExtensionInterface const* p_interface, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
		GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

		init_obj.register_initializer(initialize_godot_av1_types);
		init_obj.register_terminator(uninitialize_godot_av1_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}