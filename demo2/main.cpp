
#include <iostream>// stl cout
#include <signal.h>//signal头文件
#include <chrono>  //stl chrono头文件，时间工具，可以精确到，可以精确到纳秒
#include <thread>  //stl thread

#include "include/glog/logging.h"
#pragma comment(lib, "lib/Release/glog.lib")
using namespace google;


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
					(std::chrono::system_clock::now().time_since_epoch()).count() - start_millseconds << std::endl;

				//c++11支持u8转utf8，否则写到文件里乱码
				LOG(INFO) << u8"运行毫秒数:" << std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now().time_since_epoch()).count() - start_millseconds ;
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	};
};


int main()
{
	google::InitGoogleLogging("demo2.exe");// 
	google::SetLogDestination(google::GLOG_INFO, "demo2_");
	LOG(INFO) << "Hello World!";
	std::cout << "Hello World!\n";
	signal(SIGINT, sigterm_handler); //ctrl + c中断


	App app;
	app.run();
	std::cout << "Exit!\n";
	LOG(INFO) << "Exit!";
	std::this_thread::sleep_for(std::chrono::seconds(1));

	google::ShutdownGoogleLogging();
	return 0;
}
//先编译好glog，复制到项目目录下。
//使用VC2017，需要设置：C++选项，包含文件夹加上 "include"，然后选择不要预编译头。然后按x64位工作。
