#include "Hooks.h"
#include "Settings.h"
#include <Windows.h>
#include <stdint.h>

#pragma region SFSE_SETUP_CODE
#define MAKE_VERSION(major, minor, build) ((((major)&0xFF) << 24) | (((minor)&0xFF) << 16) | (((build)&0xFFF) << 4))

typedef uint32_t PluginHandle;

typedef struct SFSEPluginVersionData_t
{
    uint32_t dataVersion;
    uint32_t pluginVersion;
    char     name[256];
    char     author[256];
    uint32_t addressIndependence;
    uint32_t structureIndependence;
    uint32_t compatibleVersions[16];
    uint32_t seVersionRequired;
    uint32_t reservedNonBreaking;
    uint32_t reservedBreaking;
} SFSEPluginVersionData;

typedef struct SFSEPluginInfo_t
{
    uint32_t    infoVersion;
    const char* name;
    uint32_t    version;
} SFSEPluginInfo;

using namespace RE;
class HelloWorldSink : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
    // Hérité via BSTEventSink
    BSEventNotifyControl ProcessEvent(const MenuOpenCloseEvent& a_event, BSTEventSource<MenuOpenCloseEvent>* a_source) override
    {
        RE::DebugNotification("A menu was opened or closed!!");

        RE::ConsoleLog::GetSingleton()->Log("A menu was opened or closed!");

        logger::info("A menu was opened or closed!");

        return BSEventNotifyControl();
    }

public:
    HelloWorldSink() = default;
};


extern "C" __declspec(dllexport) SFSEPluginVersionData SFSEPlugin_Version = {
    1, // SFSE api version
    1, // Plugin version
    "HelloWorld",
    "aleclev",
    1, // AddressIndependence::Signatures
    1, // StructureIndependence::NoStructs
    {MAKE_VERSION(1, 8, 88), 0}, //  game version 1.7.29
    0, //  does not rely on any sfse version
    0,
    0  //  reserved fields
};


// SFSE message listener, use this to do stuff at specific moments during runtime
void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
    switch (message->type) {
    case SFSE::MessagingInterface::kPostLoad: {
    }
    case SFSE::MessagingInterface::kPostPostLoad: {
    }
    case SFSE::MessagingInterface::kPostDataLoad: {
        Settings::LoadSettings();
        Hooks::Install();

        //RE::ConsoleLog::GetSingleton()->Log("A menu was opened or closed!");

        HelloWorldSink* sink = new HelloWorldSink();

        RE::UI::GetSingleton()->RegisterSink(sink);
    }
    case SFSE::MessagingInterface::kPostPostDataLoad: {
    }
    default: {
    }
    }
}

// Main SFSE plugin entry point, initialize everything here
SFSEPluginLoad(const SFSE::LoadInterface* sfse)
{
    Init(sfse);

    //logger::info("{} {} is loading...", Plugin::Name, Plugin::Version.string());

    logger::info("{} {} is loading...", "hellowowlrd", "stuff");

    if (const auto messaging{ SFSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    //logger::info("{} has finished loading.", Plugin::Name);

    logger::info("{} has finished loading.", "helloword");

    return true;
}
