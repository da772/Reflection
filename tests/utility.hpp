#pragma once
#include <cstring>
#include <string>

#if defined(_WIN32)
#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#include <algorithm>
#include <libloaderapi.h>
#include <windows.h>
using dllptr = HMODULE;
using addrptr = FARPROC;
#endif

#if defined(__APPLE__)
#include <sys/types.h>
#include <unistd.h>
#include <libproc.h>
using dllptr = void*;
using addrptr = void*;
#endif

#if defined(__linux__) 
#include <unistd.h>
#include <dlfcn.h>
using dllptr = void*;
using addrptr = void*;
#endif

#include <chrono>

inline uint64_t GetTimeNS() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();;
}

namespace utility {
	dllptr dlopen(const char* filename, int flags) {
		#ifdef __linux__
		return ::dlopen(filename, RTLD_NOW);
		#endif
#ifdef _WIN32
		return ::LoadLibraryA(filename);
#endif
	}
	int dlclose(dllptr p) {
		#ifdef __linux__
			return ::dlclose(p);
		#endif
	#ifdef _WIN32
			return (int)FreeLibrary(p);
	#endif
	}
	addrptr dlsym(dllptr p, const char* name) {
#ifdef __linux__ 
		return ::dlsym(p, name);
#endif
#ifdef _WIN32 
		return ::GetProcAddress(p, name);
#endif
	}

	std::string GetDLLExtensionName(std::string name) {
		#ifdef __linux__
		return "lib"+name+".so";
		#endif
#ifdef _WIN32
		return name + ".dll";
#endif
	}
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


        //levelsUp+=3;

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