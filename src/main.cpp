#include "ModelView3D/model_view_3D_app.h"

int main()
{
	model_view_3D_app app;

	if (!app.init())
		return -1;

	if (!app.load_model("D:\\Project\\ModelView3D\\models\\eiffel.stl"))
		return -1;

	app.main_loop();

	return 0;
}
