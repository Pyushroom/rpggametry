#include "quest/questDatabase.hpp"

namespace
{
    const std::vector<QuestData> quests{
        QuestData{
            "guard_help",
            "Pomoc dla straznika",
            "quest_guard_started",
            "quest_guard_finished",
            {
                QuestObjective{"Porozmawiaj z kartografem", "talked_to_cartographer"},
                QuestObjective{"Porozmawiaj z zielarka", "talked_to_herbalist"},
                QuestObjective{"Wroc do straznika", "quest_guard_finished"}
            }
        },
        QuestData{
            "hill_watch",
            "Wiadomosc z wzgorza",
            "quest_hill_started",
            "quest_hill_finished",
            {
                QuestObjective{"Porozmawiaj z obserwatorem", "talked_to_hill_watcher"},
                QuestObjective{"Wroc do wedrowca", "quest_hill_finished"}
            }
        }
    };
}

namespace QuestDatabase
{
    const std::vector<QuestData>& GetAllQuests()
    {
        return quests;
    }
}