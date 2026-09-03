#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <string>
#include "discord.h"

#define GMEXPORT extern "C" _declspec(dllexport)

struct DiscordState {
	std::unique_ptr<discord::Core> core;
};

DiscordState state{};

discord::Activity myActivity{};

GMEXPORT double setActivityEverything(const char *name, const char *state_text, const char *large_text) {
	
	myActivity.SetState(state_text);
	myActivity.GetAssets().SetLargeText(large_text);
	myActivity.SetDetails(name);

	discord::ActivityTimestamps tmstp{};
	tmstp.SetStart(time(NULL));
	myActivity.GetTimestamps() = tmstp;
	myActivity.GetAssets().SetLargeImage("home");

	state.core->ActivityManager().UpdateActivity(myActivity, [](discord::Result result) {}); //update activity
	return 1;
}

GMEXPORT double setActivityType(double type) {
	myActivity.SetType((int)type);
	state.core->ActivityManager().UpdateActivity(myActivity, [](discord::Result result) {}); //update activity
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

GMEXPORT double initialize(const char *app_id)
{
	discord::Core* core{};
	/* Hello fellow source code watcher, no need to put your bot id instead of 1275938097200238725. now it's automatic. */
	std::string my_id = app_id;
	discord::Core::Create(std::stoull(my_id), DiscordCreateFlags_NoRequireDiscord, &core);

	state.core.reset(core);

	if (!state.core) {
		return 0;
	}

	return 1;
}
