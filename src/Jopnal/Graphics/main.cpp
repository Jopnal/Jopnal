#include <Jopnal\Jopnal.hpp>


class EventHandler : public jop::WindowEventHandler
{
public:

	EventHandler(jop::Window& w)
		: WindowEventHandler(w)
	{}

	~EventHandler() override
	{}

	void closed() override
	{
		jop::Engine::exit();
	}

	void keyPressed(const int key, const int, const int) override
	{
		if (key == jop::Keyboard::Escape)
			closed();
	}

};

int main(int c,char** v)
{
	using namespace jop;
	Engine e("jopnaltest", c, v);
	e.loadDefaultSubsystems();

	auto w = static_cast<Window*>(e.getSubsystem("Window"));
	w->setEventHandler<EventHandler>(*w);

	return e.runMainLoop();
}