
#include <iostream>
//C++环境下头文件的引入
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_video.h"
};

//SDL库和FFmpeg库所需的声明
#define SDL_MAIN_HANDLED
#define __STDC_CONSTANT_MACROS

#pragma comment(lib ,"SDL2.lib")
#pragma comment(lib ,"SDL2main.lib")

using namespace std;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		printf("Input the file you want to play：%s \n", argv[0]);
	}

	//FFmpeg的数据结构
	AVFormatContext* pformatContext;//封装格式上下文结构体，保存了视频文件封装格式相关信息
	AVCodecContext* pcodecContext;//编解码器上下文结构体，保存了视频（音频）编解码相关信息。
	AVCodec* pcodec;//每种视频（音频）编解码器(例如H.264解码器)对应一个该结构体
	AVPacket* packet;//用于存储一帧压缩编码数据
	AVFrame* pFrame, *pFrameYUV;//用于存储一帧解码后像素数据
	struct SwsContext* pconvertContext;//用于视频图像转换
	unsigned char* buffer;//用于存储图像数据
	int videoindex = -1;//视频流索引
	int ret;
	int getpicture;

	//SDL的数据结构
	int wide, height;//窗口宽高     
	SDL_Window* window;//窗口
	SDL_Renderer* renderer;//渲染器
	SDL_Texture* texture;//纹理
	SDL_Rect rec;

	av_register_all();//注册所有组件
	avformat_network_init();
	pformatContext = avformat_alloc_context();
	//打开输入视频文件
	if (avformat_open_input(&pformatContext, argv[1], NULL, NULL) != 0)
	{
	   cout << "Couldn't open the input stream"<<endl;
		return -1;
	}
	//获取视频文件中的信息
	if (avformat_find_stream_info(pformatContext, NULL) < 0)
	{
		cout << "failed to open the information" << endl;
		return -1;
	}
	
	//区分视音频
	for (int i = 0; i<pformatContext->nb_streams; i++)
	{
		if (pformatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoindex = i;
			break;
		}
	}
	if (videoindex == -1)
	{
		cout<<"No video stream"<<endl;
		return -1;
	}
	//为图像帧分配内存并进行转换
	packet = (AVPacket*)av_malloc(sizeof(AVPacket));
	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();
	//查找解码器
	pcodecContext = pformatContext->streams[videoindex]->codec;
	pcodec = avcodec_find_decoder(pcodecContext->codec_id);
	if (pcodec == NULL)
	{
		cout<<"decoder not found"<<endl;
		return -1;
	}
	//打开解码器
	if (avcodec_open2(pcodecContext, pcodec, NULL) < 0)
	{
		cout << "decoder opened failed" << endl;
		return -1;
	}

	buffer = (unsigned char*)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pcodecContext->width, pcodecContext->height));
	avpicture_fill((AVPicture*)pFrameYUV, buffer, AV_PIX_FMT_YUV420P, pcodecContext->width, pcodecContext->height);
	pconvertContext = sws_getContext(pcodecContext->width, pcodecContext->height, pcodecContext->pix_fmt,pcodecContext->width, pcodecContext->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	//初始化SDL系统
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
	{
		cout << "SDL initialization failed" << endl;
		return -1;
	}
	//创建窗口
	wide = pcodecContext->width;
	height = pcodecContext->height;
	window = SDL_CreateWindow("Video Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wide,height, SDL_WINDOW_OPENGL);
	if (!window)
	{
		cout << "SDL window opened failed" << endl;
		return -1;
	}

	//渲染器
	renderer = SDL_CreateRenderer(window, -1, 0);


	//纹理
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pcodecContext->width, pcodecContext->height);
	
	//设置矩形窗的位置和大小
	rec.x = 0;
	rec.y = 0;
	rec.w = wide;
	rec.h = height;

	//逐镇读取-解码-显示
	for (;;)
	{
		
		if (av_read_frame(pformatContext, packet) >= 0)
		{
			if (packet->stream_index == videoindex)
			{
				ret = avcodec_decode_video2(pcodecContext, pFrame, &getpicture, packet);
				if (ret < 0)
				{
					cout << "Decode failed" << endl;
					return -1;
				}
				if (getpicture)//将解码的yuv显示出来
				{
					sws_scale(pconvertContext, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pcodecContext->height, pFrameYUV->data, pFrameYUV->linesize);
					SDL_UpdateTexture(texture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0]);
					SDL_RenderClear(renderer);
					SDL_RenderCopy(renderer, texture, NULL, NULL);
					SDL_RenderPresent(renderer);
					SDL_Delay(40);//根据帧率进行设定
				}
			}
			else
				av_free_packet(packet);

		}
		else
			break;
	}

	sws_freeContext(pconvertContext);
	SDL_Quit();
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pcodecContext);
	avformat_close_input(&pformatContext);

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
