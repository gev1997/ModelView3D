// project
#include "model_view_3D_app.h"

// third party

// std

int main(int argc, char* argv[])
{
	if (argc < 2)
		return 0;
	
	gui::model_view_3D_app app;

	if (!app.init())
		return -1;

	if (!app.load_model(argv[1]))
		return -1;

	app.main_loop();

	return 0;
}
