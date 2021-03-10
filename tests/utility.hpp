#pragma once
#include <cstring>
#include <string>

#if defined(_WIN32)
#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#include <algorithm>
#endif

#if defined(__APPLE__)
#include <sys/types.h>
#include <unistd.h>
#include <libproc.h>
#endif

#if defined(__linux__) 
#include <unistd.h>
#endif

#include <chrono>

inline uint64_t GetTimeNS() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();;
}

inline std::string GetExecutablePath() {
#if defined(_WIN32 )
#pragma warning(push)
#pragma warning(disable : 4996)
		TCHAR NPath[1024];
		GetModuleFileName(NULL, NPath, 1024);
		std::wstring w_arr(NPath);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::string path = converter.to_bytes(w_arr);
		std::replace(path.begin(), path.end(), '\\', '/');
		return path;
#pragma warning(pop)
#endif
#if defined(__APPLE__)
		int ret;
		pid_t pid;
		char pathBuf[1024];
		pid = getpid();
		ret = proc_pidpath(pid, pathBuf, sizeof(pathBuf));
		return std::string(pathBuf);
#endif
#if defined(__linux__)
		char buff[1024];
		ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
		if (len != -1) {
			buff[len] = '\0';
			return std::string(buff);
		}
#endif
		return "FAIL";
        }

    inline std::string GetExecutableDir()
	{
		const std::string& filePath = GetExecutablePath();

		uint32_t slash = (uint32_t)filePath.find_last_of("/\\");

		if (slash >= 0) {
			return filePath.substr(0, slash);
        }

		return std::string();
	}
	

	inline 
    std::string GetParentExecuteableDir(int levelsUp)
	{
		const std::string& exePath = GetExecutableDir();
		std::string dirPath = exePath;


        levelsUp+=3;

		for (int i = 0; i < levelsUp; i++) {
			uint32_t slash = (uint32_t)dirPath.find_last_of("/\\");
			if (slash >= 0) {
				dirPath = dirPath.substr(0, slash);
			} 
			else {
				break;
			}
		}

		return dirPath+"/";
	}