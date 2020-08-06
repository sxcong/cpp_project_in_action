
#include <iostream>// stl cout
#include <signal.h>//signal头文件
#include <chrono>  //stl chrono头文件，时间工具，可以精确到，可以精确到纳秒
#include <thread>  //stl thread

static int signaled = 0;
static void sigterm_handler(int sig)
{
	signaled = 1;
}

class App
{
public:
	void run()
	{
		//std::chrono::milliseconds可以是以下四个中的一个：seconds,milliseconds,microseconds,nanoseconds
		uint64_t start_millseconds = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count();
		for (;;)
		{
			if (signaled == 1)
				break;
			else
			{
				std::cout << "运行毫秒数:" << std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now().time_since_epoch()).count()- start_millseconds << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));;
			}
		}
	};
};


int main()
{
    std::cout << "Hello World!\n"; 
	signal(SIGINT, sigterm_handler); //ctrl + c中断


	App app;
	app.run();
	std::cout << "Exit!\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));;
}
