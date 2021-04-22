#pragma once
#include <cstring>
#include <string>

#include "reflection/reflection.hpp"
#include <iostream>

#if defined(_WIN32)
#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#include <algorithm>
#include <libloaderapi.h>
#include <windows.h>
#include <errhandlingapi.h>
#include <process.h>
using dllptr = HMODULE;
using addrptr = FARPROC;
using ret_err = DWORD;

#endif

#define MS_xstr(a) MS_str(a)
#define MS_str(a) #a

#if defined(__APPLE__)
#include <sys/types.h>
#include <unistd.h>
#include <libproc.h>
#include <dlfcn.h>
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#endif

#if defined(__linux__) 
#include <unistd.h>
#include <dlfcn.h>
using dllptr = void*;
using addrptr = void*;
using ret_err = char*;
#endif

#include <chrono>

static std::string __DLL_prefix = "cpy_";
static bool _renameDLL = false;

inline uint64_t GetTimeNS() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();;
}

namespace dll {

	inline void movePDB(const std::string& dir, const std::string& name, const std::string& newPrefix, bool deleteOld) {

		std::string loc = dir + newPrefix + name + ".pdb";
		std::ifstream  src(dir + name + ".pdb", std::ios::binary);
		std::ofstream  dst(loc, std::ios::binary);
		if (src.fail()) {
			std::cout << "FAILED TO COPY " << dir + name + ".pdb" << std::endl;
			return;
		}
		dst << src.rdbuf();
		dst.close();
		src.close();
		if (deleteOld)
			::remove((dir + name + ".pdb").c_str());
		
	}

	inline ret_err dlerror() {
#if defined(__linux__) || defined(__APPLE__)
		return ::dlerror();
#endif
#ifdef _WIN32
		return ::GetLastError();
#endif
	}
	inline dllptr dlopen(const char* filename, int flags) {
#if defined(__linux__) || defined(__APPLE__)
		return ::dlopen(filename, RTLD_NOW);
#endif
#ifdef _WIN32
		return ::LoadLibraryA(filename);
#endif
	}
	inline  int dlclose(dllptr p) {
#if defined(__linux__) || defined(__APPLE__)
		return ::dlclose(p);
#endif
#ifdef _WIN32
		return (int)FreeLibrary(p);
#endif
	}
	inline  addrptr dlsym(dllptr p, const char* name) {
#if defined(__linux__) || defined(__APPLE__)
		return ::dlsym(p, name);
#endif
#ifdef _WIN32 
		return ::GetProcAddress(p, name);
#endif
	}

	inline std::string GetDLLExtensionName(std::string name) {
#if defined(__linux__)
		return "lib" + name + ".so";
#endif
#if defined(__APPLE__)
		return "lib" + name + ".dylib";
#endif
#ifdef _WIN32
		return name + ".dll";
#endif
	}
}

#ifndef BUILD_CONFIG
#define BUILD_CONFIG ""
#endif

#ifndef BUILD_ARCHITECTURE
#define BUILD_ARCHITECTURE ""
#endif

namespace sys {

	inline std::string exec_command(const std::string& cmd);

	inline std::string build_proj(const std::string& dir, const std::string& file) {
		std::string cmd = "";
		#if defined(__linux__) || defined(__APPLE__)
		cmd += "cd \"" + dir + "\" && \"./build/"+file;
			#ifdef __APPLE__
			cmd+= "_Mac.command\" ";
			#endif
			#ifdef __linux__
			cmd += "_Linux.sh\" ";
			#endif
		#endif
		#if defined(_WIN32)
			std::string drive = dir.substr(0, 2);
			cmd += dir + "build/" + file + "_Windows.bat";
			//cmd += "" + dir + "build\" && \""+file+"_Windows.bat\" ";
		#endif
			std::cout << "BUILD CMD: " << cmd << std::endl;
		return exec_command(cmd);
	}

	inline std::string compile_proj(const std::string& dir, const std::string& file, const std::string& CXX = "") {
		::std::string cmd = "";
#if defined(_WIN32) && defined(COMPILER)
		cmd += "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat && ";
#endif
#if defined(_WIN32) && !defined(COMPILER)
		std::string _msBin = MS_xstr(MS_BUILD_BIN);
		_msBin.erase(_msBin.size() - 1);
		cmd += _msBin + "\\MSBuild.exe\" \"" + dir + file + ".vcxproj\" /verbosity:quiet /nologo ";
#endif
#if defined (__linux__) || defined (__APPLE__) || defined(COMPILER)
		cmd += "cd " + dir + " && "+MS_xstr(COMPILER)+" "+file + " ";
#endif
			const std::string config = MS_xstr(BUILD_CONFIG);;

		if (config.size() > 0) {
#if defined (__linux__) || defined (__APPLE__)
			std::transform(config.begin(), config.end(), config.begin(),
				[](unsigned char c) { return std::tolower(c); });
			cmd += "config=" + config;
#endif

#ifdef _WIN32
			cmd += "/p:Configuration=" + config + " ";
#endif
		}

		const ::std::string arch = BUILD_ARCHITECTURE;
		if (arch == "x86_64") {
#if defined (__linux__) || defined (__APPLE__)
			cmd += "_x86_64";
#endif
#ifdef _WIN32
			cmd += "/p:Platform=x64 ";
#endif
		}
		else if (arch == "x86") {
#if defined (__linux__) || defined (__APPLE__)
			cmd += "_x86";
#endif
#ifdef _WIN32
			cmd += "/p:Platform=Win32 ";
#endif
		}
		std::cout << "RUNNING COMMAND: \n\n" << cmd << "\n\n" << std::endl;
		return exec_command(cmd);

	}

#ifdef _WIN32
	inline PROCESS_INFORMATION CreateChildProcess(const std::string& cmd, HANDLE _ERR_WR, HANDLE _OUT_WR);
	inline std::string ReadFromPipe(HANDLE err_RD, HANDLE out_RD);
#endif

	inline std::string exec_command(const std::string& cmd) {
#if defined(__linux__) || defined (__APPLE__)
		char buffer[2048] = {0};
		std::string result = "";

		// Open pipe to file
		FILE* pipe = popen((cmd + " 2>&1").c_str(), "r");
		if (!pipe) {
			return "popen failed!";
		}

		// read till end of process:
		while (!feof(pipe)) {
			// use buffer to read and add to result
			if (fgets(buffer, 128, pipe) != NULL) {
				result += buffer;
			}
		}
		pclose(pipe);
		return result;
#endif
#ifdef _WIN32
		std::string output = "";
		HANDLE out_RD, out_WR, err_RD, err_WR;
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

		// Create a pipe for the child process's STDERR. 
		if (!CreatePipe(&err_RD, &err_WR, &sa, 0)) {
			return "FAILED TO CREATE ERROR PIPE";
		}
		// Ensure the read handle to the pipe for STDERR is not inherited.
		if (!SetHandleInformation(err_RD, HANDLE_FLAG_INHERIT, 0)) {
			return "FAILED TO HANDLE ERROR PIPE INFORMATION";
		}
		// Create a pipe for the child process's STDOUT. 
		if (!CreatePipe(&out_RD, &out_WR, &sa, 0)) {
			return "FAILED TO CREATE OUT PIPE";
		}
		// Ensure the read handle to the pipe for STDOUT is not inherited
		if (!SetHandleInformation(out_RD, HANDLE_FLAG_INHERIT, 0)) {
			return "FAILED TO HANDLE OUT PIPE INFORMATION";
		}

		PROCESS_INFORMATION info = CreateChildProcess(cmd, err_WR, out_WR);
		output = ReadFromPipe(err_RD, out_RD);
		return output;
#endif
	}
#ifdef _WIN32
	inline PROCESS_INFORMATION CreateChildProcess(const std::string& cmd, HANDLE _ERR_WR, HANDLE _OUT_WR) {
		PROCESS_INFORMATION piProcInfo;
		STARTUPINFOA siStartInfo;
		bool bSuccess = FALSE;

		// Set up members of the PROCESS_INFORMATION structure. 
		ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
		
		// Set up members of the STARTUPINFO structure. 
		// This structure specifies the STDERR and STDOUT handles for redirection.
		ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdError = _ERR_WR;
		siStartInfo.hStdOutput = _OUT_WR;
		siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

		LPSTR cmdLine = const_cast<char*>(cmd.c_str());

		// Create the child process. 
		bSuccess = CreateProcessA(NULL,
			cmdLine,     // command line 
			NULL,          // process security attributes 
			NULL,          // primary thread security attributes 
			TRUE,          // handles are inherited 
			0,             // creation flags 
			NULL,          // use parent's environment 
			NULL,          // use parent's current directory 
			&siStartInfo,  // STARTUPINFO pointer 
			&piProcInfo);  // receives PROCESS_INFORMATION

		DWORD d = WaitForSingleObject(piProcInfo.hProcess, INFINITE);

		CloseHandle(_ERR_WR);
		CloseHandle(_OUT_WR);

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
		// If an error occurs, exit the application. 
		if (!bSuccess) {
			std::cout << GetLastError() << std::endl;
		}
		return piProcInfo;
	}
#endif

#ifdef _WIN32
	inline std::string ReadFromPipe(HANDLE err_RD, HANDLE out_RD) {
		DWORD dwRead;
		CHAR* chBuf = (CHAR*)calloc(2048, sizeof(CHAR));
		bool bSuccess = FALSE;
		std::string out = "", err = "";
		for (;;) {
			bSuccess = ReadFile(out_RD, chBuf, 2048, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;

			std::string s(chBuf, dwRead);
			out += s;
		}
		dwRead = 0;
		for (;;) {
			bSuccess = ReadFile(err_RD, chBuf, 2048, &dwRead, NULL);
			if (!bSuccess || dwRead == 0) break;

			std::string s(chBuf, dwRead);
			err += s;

		}

		CloseHandle(err_RD);
		CloseHandle(out_RD);
		free(chBuf);
		if (err.size() > 0) {
			return err;
		}
		else {
			return out;
		}
		
	}
#endif


}

namespace files {

	inline std::string GetExecutablePath() {
#if defined(_WIN32 )
#pragma warning(push)
#pragma warning(disable : 4996)
		TCHAR NPath[1024];
		GetModuleFileName(NULL, NPath, 1024);
		std::string path(NPath);
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


	inline std::string GetParentExecuteableDir(int levelsUp)
	{
		const std::string& exePath = GetExecutableDir();
		std::string dirPath = exePath;

		for (int i = 0; i < levelsUp; i++) {
			uint32_t slash = (uint32_t)dirPath.find_last_of("/\\");
			if (slash >= 0) {
				dirPath = dirPath.substr(0, slash);
			}
			else {
				break;
			}
		}

		return dirPath + "/";
	}
}


inline void __LoadLib(dllptr* lib, refl::reflector& r, const std::string& name) {
	if (*lib) {
		std::cout << "LIB ALREADY LOADED" << std::endl;
		return;
	}
	
	std::string loc = files::GetParentExecuteableDir(0) + dll::GetDLLExtensionName((_renameDLL ? __DLL_prefix : "" )+name);
	if (_renameDLL) {
		std::ifstream src(files::GetParentExecuteableDir(0) + dll::GetDLLExtensionName(name), std::ios::binary);

		std::ofstream  dst(loc, std::ios::binary);
		dst << src.rdbuf();
		dst.close();
	}
#ifdef _WIN32
	dll::movePDB(files::GetParentExecuteableDir(0), "Scripts", "_", true);
#endif
	*lib = dll::dlopen(loc.c_str(), 0);

	if (!lib) {
		std::cout << "INVALID HANDLE: " << dll::dlerror() << std::endl;
		return;
	}
	
	void (*func_ptr)(::refl::store::storage*) = reinterpret_cast<void (*)(::refl::store::storage*)>(dll::dlsym(*lib, "__ReflectionMap__loadGeneratedFiles"));
	if (!func_ptr) {
		std::cout << "COULD NOT LOAD SYMBOL" << std::endl;
		dll::dlclose(*lib);
		lib = 0;
		return;
	}
	(*func_ptr)(r.GetStorage());
}

inline void __UnloadLib(dllptr* lib, refl::reflector& r) {
	if (*lib) {
		void (*func_ptr)(::refl::store::storage*) = reinterpret_cast<void (*)(::refl::store::storage*)>(dll::dlsym(*lib, "__ReflectionMap__unloadGeneratedFiles"));
		if (func_ptr) {
			(*func_ptr)(r.GetStorage());
		}
		dll::dlclose(*lib);
		*lib = 0;
		std::string loc = files::GetParentExecuteableDir(0) + dll::GetDLLExtensionName((_renameDLL ? __DLL_prefix : "" )+"Reflection_Tests_Scripts");
		if (_renameDLL)
			::remove(loc.c_str());
	}
}

inline void __GenerateLib(const std::string& path, const std::string& name, refl::reflector& r) {
	std::string in = path+name;
	std::ifstream t(in);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::cout << in << std::endl;
	r.Generate(in.c_str());
	if (r.HasError()) {
		std::cout << r.GetError() << std::endl;
	}
}