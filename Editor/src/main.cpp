#include <iostream>
#include <memory>
#include <Core/app.hpp>

class MyApp : public msen::App
{
	virtual void on_update() override {

		//std::cout << "Update frame: " << frame<<std::endl;
		frame += 1;
	}

	int frame = 0;

};

int main() {
	
	auto App = std::make_unique<MyApp>();
	int returnCode = App->start(1024, 768, "my app window");
	return returnCode;
}