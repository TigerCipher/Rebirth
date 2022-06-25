// ------------------------------------------------------------------------------
// 
// Rebirth
//    Copyright 2022 Matthew Rogers
// 
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// 
// File Name: Profiler.h
// Date File Created: 6/24/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

namespace rebirth
{
	struct ProfileResult
	{
		std::string name;
		long long start;
		long long end;
		uint32_t threadId;
	};

	struct ProfileSession
	{
		std::string name;
	};

	class Profiler
	{
	public:
		Profiler() = default;


		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			mOutput.open(filepath);
			WriteHeader();
			mSession = new ProfileSession{name};
		}

		void EndSession()
		{
			WriteFooter();
			mOutput.close();
			delete mSession;
			mSession = nullptr;
			mProfileCount = 0;
		}

		void WriteHeader()
		{
			mOutput << "{\"otherData\": {},\"traceEvents\":[";
			mOutput.flush();
		}

		void WriteFooter()
		{
			mOutput << "]}";
			mOutput.flush();
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (mProfileCount++ > 0)
				mOutput << ",";

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			mOutput << "{";
			mOutput << "\"cat\":\"function\",";
			mOutput << "\"dur\":" << (result.end - result.start) << ',';
			mOutput << "\"name\":\"" << name << "\",";
			mOutput << "\"ph\":\"X\",";
			mOutput << "\"pid\":0,";
			mOutput << "\"tid\":" << result.threadId << ",";
			mOutput << "\"ts\":" << result.start;
			mOutput << "}";

			mOutput.flush();
		}

		static Profiler& Get()
		{
			static Profiler instance;
			return instance;
		}

	private:
		ProfileSession* mSession = nullptr;
		int mProfileCount = 0;
		std::ofstream mOutput;
	};

	class ProfileTimer
	{
	public:
		ProfileTimer(const char* name) : mName(name), mStopped(false)
		{
			mStartPoint = std::chrono::high_resolution_clock::now();
		}

		~ProfileTimer()
		{
			if (!mStopped) Stop();
		}

		void Stop()
		{
			auto endPoint = std::chrono::high_resolution_clock::now();
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartPoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

			uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Profiler::Get().WriteProfile({ mName, start, end, threadId });

			mStopped = true;
		}
	private:
		const char* mName;
		std::chrono::time_point<std::chrono::steady_clock> mStartPoint;
		bool mStopped;
	};
}

#define RB_ENABLE_PROFILER 1

#if RB_ENABLE_PROFILER
#	define RB_PROFILE_BEGIN_SESSION(name, filepath) ::rebirth::Profiler::Get().BeginSession(name, filepath)
#	define RB_PROFILE_END_SESSION() ::rebirth::Profiler::Get().EndSession()
#	define RB_PROFILE_SCOPE(name) ::rebirth::ProfileTimer timer##__LINE__(name)
#	define RB_PROFILE_FUNC() RB_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define RB_PROFILE_BEGIN_SESSION(name, filepath)
	#define RB_PROFILE_END_SESSION()
	#define RB_PROFILE_SCOPE(name)
	#define RB_PROFILE_FUNC()
#endif