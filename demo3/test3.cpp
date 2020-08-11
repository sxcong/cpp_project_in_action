
#include <iostream>// stl cout
#include <signal.h>//signal头文件
#include <chrono>  //stl chrono头文件，时间工具，可以精确到，可以精确到纳秒
#include <thread>  //stl thread

#include "include/glog/logging.h"
#pragma comment(lib, "lib/Release/glog.lib")
using namespace google;

class IAppCB
{
public:
	virtual int onInit() = 0;
	virtual int onDestroy() = 0;
};

class AppCBImpl :public IAppCB
{
public:
	virtual int onInit()
	{
		std::cout << "onInit\n";
		
		return 0;
	};
	virtual int onDestroy()
	{
		std::cout << "onDestroy\n";
		return 0;
	};
};


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

IAppCB* m_pCB = nullptr;
int main()
{
	m_pCB = new AppCBImpl;
	google::InitGoogleLogging("demo3.exe");// 
	google::SetLogDestination(google::GLOG_INFO, "demo3_");
	LOG(INFO) << "Hello World!";
	std::cout << "Hello World!\n";
	if (m_pCB->onInit() != 0)//程序启动时由外部确定需要执行哪些操作，比如显示欢迎页。如果程序初始化失败，onInit 非0数值，程序退出。
	{
		delete m_pCB;
		m_pCB = nullptr;
		return 0;
	}
	signal(SIGINT, sigterm_handler); //ctrl + c中断


	App app;
	app.run();
	std::cout << "Exit!\n";
	LOG(INFO) << "Exit!";
	m_pCB->onDestroy();//程序退出前由外部决定还要执行哪些操作，比如各种流氓软件退出时弹出页面。

	std::this_thread::sleep_for(std::chrono::seconds(1));
	google::ShutdownGoogleLogging();
	
	delete m_pCB;
	m_pCB = nullptr;
	return 0;
}

