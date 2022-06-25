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
		std::chrono::duration<double, std::micro> start;
		std::chrono::microseconds elapsed;
		std::thread::id threadId;
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
			std::lock_guard lock(mMutex);

			mOutput.open(filepath);
			if (mOutput.is_open())
			{
				WriteHeader();
				mSession = new ProfileSession{ name };
			}
			else
			{
#ifdef RB_DEBUG
				if (Log::GetCoreLogger())
				{
					RB_CORE_ERROR("Profiler failed to open file '{}'", filepath);
				}
#endif
			}
		}

		void EndSession()
		{
			std::lock_guard lock(mMutex);
			WriteFooter();
			mOutput.close();
			delete mSession;
			mSession = nullptr;
		}

		void WriteHeader()
		{
			mOutput << "{\"otherData\": {},\"traceEvents\":[{}";
			mOutput.flush();
		}

		void WriteFooter()
		{
			mOutput << "]}";
			mOutput.flush();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;
			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << result.elapsed.count() << ',';
			json << "\"name\":\"" << name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadId << ",";
			json << "\"ts\":" << result.start.count();
			json << "}";

			std::lock_guard lock(mMutex);
			if (mSession)
			{
				mOutput << json.str();
				mOutput.flush();
			}
		}

		static Profiler& Get()
		{
			static Profiler instance;
			return instance;
		}

	private:
		ProfileSession* mSession = nullptr;
		std::ofstream mOutput;
		std::mutex mMutex;
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
			auto endPoint = std::chrono::steady_clock::now();
			auto highResStart = std::chrono::duration<double, std::micro>{ mStartPoint.time_since_epoch() };
			auto duration = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(mStartPoint).time_since_epoch();

			Profiler::Get().WriteProfile({ mName, highResStart, duration, std::this_thread::get_id() });

			mStopped = true;
		}
	private:
		const char* mName;
		std::chrono::time_point<std::chrono::steady_clock> mStartPoint;
		bool mStopped;
	};
}

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