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

GMEXPORT double setActivity(char* name, char *state_text, char *large_text)
{
	discord::Activity activity{};
	activity.SetDetails(name);
	activity.SetState(state_text);
	activity.GetAssets().SetLargeImage("home");
	activity.GetAssets().SetLargeText(large_text);
	discord::ActivityTimestamps tmstp{};
	tmstp.SetStart(time(NULL));
	activity.GetTimestamps() = tmstp;

	state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});

	return 1;
}

GMEXPORT double setActivityType(double type) {
	discord::Activity activity{};
	activity.SetType((discord::ActivityType)(int)type);

	state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});

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

GMEXPORT double initialize(double client_id)
{
	discord::Core* core{};
	discord::Core::Create((discord::ClientId)(int)client_id, DiscordCreateFlags_NoRequireDiscord, &core);

	state.core.reset(core);

	if (!state.core) {
		return 0;
	}

	return 1;
}