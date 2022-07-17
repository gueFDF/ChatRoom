#include "glog/logging.h"
#include <iostream>
 
using namespace std;
 
#define NDEBUG 0
int main(int argc, char* argv[])
{
    //几个参数设置
	// 设置日志消息是否转到标准输出而不是日志文件
	FLAGS_logtostderr = false;
	// 设置日志前缀是否应该添加到每行输出
	FLAGS_log_prefix = true;
	// 设置可以缓冲日志的最大秒数，0指实时输出
	FLAGS_logbufsecs = 0;
	// 设置最大日志文件大小（以MB为单位）
	FLAGS_max_log_size = 10;
	// 设置是否在磁盘已满时避免日志记录到磁盘
	FLAGS_stop_logging_if_full_disk = true;
	// 设置记录到stderr的log的级别。这仅仅是对于记录Log到stderr中的设置，当我们需要将WARNING等级的log记录到stderr时，
	// 我们可以通过设置FLAGS_stderrthreshold = 1，那么WARNING以及更高级别的Log将会记录到stderr中。INFO: 0, WARNING: 
	// 1, ERROR: 2 FATAL: 3;
	FLAGS_stderrthreshold = 1;
	// 设置需要记录log的级别。我们可以指定那些Log被记录（无论是在本地还是stderr）,通过设置FLAGS_minloglevel = 0标志，
	// 默认是0，设置这个标志表明，INFO以及更高级别的Log将会被记录；INFO: 0, WARNING: 1, ERROR: 2 FATAL: 3。
	FLAGS_minloglevel = 0;
	 
    #if NDEBUG
		std::cout << "Build-RELASE" << std::endl;
		FLAGS_alsologtostderr = false;
	#else
		std::cout << "Build-DEBUG" << std::endl;
		//设置日志消息除日志文件之外同时输出到标准输出
		FLAGS_alsologtostderr = true;
		//设置记录到标准输出的颜色消息（如果终端支持）
		FLAGS_colorlogtostderr = true;
	#endif
	// 设置日志文件保存目录,这个目录必须是已经存在的,否则不能生成日志文件.
	FLAGS_log_dir = "test.glog";
 
    google::InitGoogleLogging(argv[0]);                // 全局初始化glog，argv[0]是程序名
    google::SetStderrLogging(google::GLOG_INFO);       // 设置glog的输出级别，这里的含义是输出INFO级别以上的信息
    // 设置INFO级别以上的信息log文件的路径和前缀名
    google::SetLogDestination(google::GLOG_INFO, "log/info_");
 
    // 设置WARNING级别以上的信息log文件的路径和前缀名
    google::SetLogDestination(google::GLOG_WARNING, "log/warn_");
 
    // 设置ERROR级别以上的信息log文件的路径和前缀名
    google::SetLogDestination(google::GLOG_ERROR, "log/errr_");
 
    FLAGS_colorlogtostderr = true;                     // 开启终端颜色区分
 
    LOG(INFO) << "This is my first glog INFO ";        // 像C++标准流一样去使用就可以了，把这条信息定义为INFO级别
    LOG(WARNING) << "This is my first glog WARNING";   // 像C++标准流一样去使用就可以了，把这条信息定义为WARNING级别
    LOG(ERROR) << "This is my first glog ERROR";       // 像C++标准流一样去使用就可以了，把这条信息定义为ERROR级别
 
    google::ShutdownGoogleLogging();                   // 全局关闭glog
    return 0;
}
