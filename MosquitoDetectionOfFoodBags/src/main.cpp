#include <QMessageBox>
#include <QtWidgets/QApplication>

#include "Modules.hpp"
#include "MDOFoodBags.h"


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

	// 检测运行环境
	if (!Modules::check())
	{
		return 1;
	}

	Modules::getInstance().build();
    MDOFoodBags w;
	Modules::getInstance().uiModule._wetPapers = &w;
	Modules::getInstance().connect();
	Modules::getInstance().start();

#ifdef NDEBUG
	w.showFullScreen();
#else
	w.show();
#endif
    return a.exec();
}
