#pragma once

#include "dialogue/dialogue.hpp"
#include "npc/npcData.hpp"

#include <string>
#include <vector>

class DialogueController
{
public:
    DialogueController() = default;

    void StartDialogue(const DialogueData* dialogueData, const NpcData* npcData = nullptr);
    void Update();
    void Draw() const;
    void Close();

    [[nodiscard]] bool IsActive() const;
    [[nodiscard]] bool IsHidden() const;

private:
    enum class Mode
    {
        Hidden,
        OpeningPages,
        ChoiceSelection,
        ResponsePages
    };

private:
    void ResetState();
    void AdvancePage();

    [[nodiscard]] std::vector<std::string> WrapTextToLines(
        const char* text,
        int fontSize,
        int maxWidth) const;

    [[nodiscard]] std::vector<std::string> PaginateText(
        const char* text,
        int fontSize,
        int maxWidth,
        int maxLinesPerPage) const;

    void SetPagesFromText(
        const char* text,
        int fontSize,
        int maxWidth,
        int maxLinesPerPage);

private:
    Mode m_mode{Mode::Hidden};
    const DialogueData* m_activeDialogue{nullptr};
    const NpcData* m_activeNpcData{nullptr};
    std::vector<std::string> m_currentPages{};
    int m_currentPageIndex{0};
    int m_selectedChoiceIndex{0};
    const char* m_selectedChoiceText{nullptr};
};