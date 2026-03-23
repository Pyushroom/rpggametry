#pragma once

#include "player.hpp"
#include "world.hpp"

#include <vector>

class Game
{
public:
    Game();

    int Run();

private:
    void Update(float deltaTime);
    void Draw() const;

    void ResetDialogueState();
    void StartDialogue(const DialogueData* dialogueData);
    void AdvanceDialoguePage();

private:
    enum class DialogueMode
    {
        Hidden,
        OpeningPages,
        ChoiceSelection,
        ResponsePages
    };

private:
    World m_world{};
    Player m_player{};
    SceneCoord m_currentCoord{0, 0};
    float m_transitionCooldown{0.0f};

    DialogueMode m_dialogueMode{DialogueMode::Hidden};
    const DialogueData* m_activeDialogue{nullptr};
    std::vector<const char*> m_currentPages{};
    int m_currentPageIndex{0};
    int m_selectedChoiceIndex{0};
};