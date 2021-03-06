﻿
# include <Siv3D.hpp> // OpenSiv3D v0.2.7
#include "app/clicker/app1.hpp"
#include "app/clicker/view.hpp"

void Main()
{
	s3d::Window::SetTitle(U"知恵クリッカー(仮)");
	cfn::mainloop_scheduler scheduler;
	auto dat = std::make_shared<app::Data>();

	auto model = std::make_shared<app::GameModel>(dat);

	auto view = std::make_shared<app::GameView>();

	model->onChangedBank().subscribe([&](auto n) {
		view->set_bank(n);
	});

	model->onChangedBuilding().subscribe([&](auto const&t) {
		auto[n,build] = t;
		view->set_building(n, build);
	});

	model->onChangedTps().subscribe([&](auto n) {
		view->set_tps(n);
	});

	for (std::size_t i = 0; i < app::BuildingMax; ++i) {
		view->onClickBuild(i).subscribe([=](auto) {
			model->buy_building(i);
		});
	}

	model->chInit();
	view->init();

	while (System::Update())
	{
		while (scheduler.has_event())
		{
			scheduler.update();
		}
		model->tik(System::DeltaTime());
		if (s3d::Key8.pressed()) {
			model->tik(1);
		}
		view->update();
		view->draw();
	}
}
