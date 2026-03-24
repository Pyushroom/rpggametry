#pragma once

#include "player/player.hpp"
#include "world/world.hpp"

#include <string>
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
    void CloseDialogue();

    [[nodiscard]] std::vector<std::string> WrapTextToLines(const char* text, int fontSize, int maxWidth) const;
    [[nodiscard]] std::vector<std::string> PaginateText(
        const char* text,
        int fontSize,
        int maxWidth,
        int maxLinesPerPage) const;

    void SetDialoguePagesFromText(
        const char* text,
        int fontSize,
        int maxWidth,
        int maxLinesPerPage);

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
    std::vector<std::string> m_currentPages{};
    int m_currentPageIndex{0};
    int m_selectedChoiceIndex{0};
    const char* m_selectedChoiceText{nullptr};
};