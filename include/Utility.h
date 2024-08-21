#pragma once

#include "Settings.h"

class Utility : public Singleton<Utility>
{
public:
    inline static std::atomic<bool> is_highlight_pressed = false;
    inline static std::atomic<bool> is_highlight_over = false;

    inline static std::vector<std::pair<RE::ActorPtr, float>> targetList = {};

    static auto HighlightWorker() noexcept
    {
        logger::info("Worker thread started");

        while (true) {
            if (!is_highlight_pressed) {
                continue;
            }

            if (is_highlight_pressed) {
                if (!is_highlight_over) {
                    SKSE::GetTaskInterface()->AddTask([&] {
                        logger::debug("Highlight initiated");
                        IdentifyTargets();
                        HighlightTargets();
                    });
                    is_highlight_over    = true;
                    is_highlight_pressed = false;
                }
                else {
                    SKSE::GetTaskInterface()->AddTask([&] {
                        logger::debug("Unhighlighting");
                        UnHighlightTargets();
                    });
                    is_highlight_over    = false;
                    is_highlight_pressed = false;
                }
            }
        }
    }

    static auto Init() noexcept
    {
        if (const auto player{ RE::PlayerCharacter::GetSingleton() }) {
            std::jthread([=] { HighlightWorker(); }).detach();
        }
    }

    static void IdentifyTargets() noexcept
    {
        if (const auto player{ RE::PlayerCharacter::GetSingleton() }) {
            const auto playerPos = player->GetPosition();
            for (auto actorHandle : RE::ProcessLists::GetSingleton()->highActorHandles) {
                if (auto actorPtr = actorHandle.get()) {
                    if (auto actor = actorPtr.get()) {
                        if (actor->IsDead()) {
                            if (playerPos.GetDistance(actor->GetPosition()) <= Settings::check_dist) {
                                targetList.push_back(std::make_pair(actorPtr, Settings::alpha));
                            }
                        }
                    }
                }
            }
        }
    }

    static void HighlightTargets() noexcept
    {
        for (auto&& target : targetList) {
            if (auto actor = std::get<0>(target).get()) {
                if (auto obj3D = actor->Get3D()) {
                    RE::NiColorA color{ Settings::r_color, Settings::g_color, Settings::b_color, std::get<1>(target) };
                    obj3D->TintScenegraph(color);
                }
            }
        }
    }

    static void UnHighlightTargets() noexcept
    {
        for (auto&& target : targetList) {
            std::get<1>(target) = 0.0f;
        }

        HighlightTargets();
        targetList = {};
    }
};
