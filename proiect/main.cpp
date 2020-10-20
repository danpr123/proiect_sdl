#include "joc.h"

int main(int argc, char* args[])
{
	joc* app = new joc;
	app->ini();
	app->loop();
	delete app;

	return 0;
}