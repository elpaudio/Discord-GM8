#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include "discord.h"

#define GMEXPORT extern "C" _declspec(dllexport)

struct DiscordState {
	std::unique_ptr<discord::Core> core;
};

DiscordState state{};

GMEXPORT double setActivityEverything(char *name, char *state_text, char *large_text) {
	discord::Activity activity{};
	
	activity.SetState(state_text);
	activity.GetAssets().SetLargeText(large_text);
	activity.SetDetails(name);
	activity.SetType(discord::ActivityType::Listening);

	discord::ActivityTimestamps tmstp{};
	tmstp.SetStart(time(NULL));
	activity.GetTimestamps() = tmstp;
	activity.GetAssets().SetLargeImage("home");

	state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {}); //update activity
	return 1;
}

GMEXPORT double clearActivity()
{
	state.core->ActivityManager().ClearActivity([](discord::Result result) {});
	return 1;
}

GMEXPORT double callback()
{
	auto result = state.core->RunCallbacks();
	if (result == discord::Result::Ok) {
		return 1;
	}
	else {
		return 0;
	}
}

GMEXPORT double initialize()
{
	discord::Core* core{};
	/* Hello fellow source code watcher, put your bot id instead of 1275938097200238725. */
	discord::Core::Create(1275938097200238725, DiscordCreateFlags_NoRequireDiscord, &core);

	state.core.reset(core);

	if (!state.core) {
		return 0;
	}

	return 1;
}
