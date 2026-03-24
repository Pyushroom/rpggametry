#include "dialogue/dialogueDatabase.hpp"

namespace
{
const DialogueData travelerDialogue{
    "Wedrowiec",
    {
        "Witaj podrozniku!",
        "To dopiero poczatek twojej przygody.",
        "Rozejrzyj sie dobrze po okolicy."
    },
    {}
};

const DialogueData guardDialogue{
    "Straznik",
    {
        "Czego chcesz, wedrowcze?"
    },
    {
        DialogueChoice{
            "Kim jestes?",
            {
                "Jestem straznikiem tej drogi.",
                "Pilnuje porzadku i obserwuje, kto przechodzi."
            }
        },
        DialogueChoice{
            "Co jest dalej?",
            {
                "Na wschodzie kiedys powstanie miasto.",
                "Na razie droga nie jest jeszcze gotowa."
            }
        },
        DialogueChoice{
            "Do widzenia.",
            {
                "Ruszaj ostroznie."
            }
        }
    }
};

const DialogueData ladderGuideDialogue{
    "Robotnik",
    {
        "Potrzebujesz pomocy z ta drabina?"
    },
    {
        DialogueChoice{
            "Dokad prowadzi?",
            {
                "Na wyzszy poziom pola.",
                "Mozesz tez zejsc z powrotem druga drabina."
            }
        },
        DialogueChoice{
            "Dzieki.",
            {
                "Nie ma sprawy."
            }
        }
    }
};

const DialogueData hillWatcherDialogue{
    "Obserwator",
    {
        "Na wzgorzu latwiej wypatrzyc droge przed soba.",
        "Przy dobrej pogodzie widac nawet odlegle sciezki."
    },
    {}
};

const DialogueData houseOwnerDialogue{
    "Gospodarz",
    {
        "To moj dom.",
        "Rozgosc sie, ale niczego nie ruszaj."
    },
    {}
};

const DialogueData cartographerDialogue{
    "Kartograf",
    {
        "Przechowuje tutaj mapy okolicy."
    },
    {
        DialogueChoice{
            "Co rysujesz?",
            {
                "Na razie tylko okolice polany, drogi i wzgorza.",
                "Z czasem mapa stanie sie duzo wieksza."
            }
        },
        DialogueChoice{
            "Moge zobaczyc mapy?",
            {
                "Jeszcze nie.",
                "Najpierw musze je dokonczyc."
            }
        }
    }
};

const DialogueData herbalistDialogue{
    "Zielarka",
    {
        "Lubie rosliny.",
        "Kiedys bedzie tu prawdziwy zielnik."
    },
    {
        DialogueChoice{
            "Jakie rosliny zbierasz?",
            {
                "Glownie ziola z laki i krzewy z okolicy.",
                "Niektore nadaja sie do leczenia, inne do mikstur."
            }
        },
        DialogueChoice{
            "Brzmi dobrze.",
            {
                "Wroc pozniej, moze cos juz tu wyrosnie."
            }
        }
    }
};
}

namespace DialogueDatabase
{
const DialogueData& GetTravelerDialogue()
{
    return travelerDialogue;
}

const DialogueData& GetGuardDialogue()
{
    return guardDialogue;
}

const DialogueData& GetLadderGuideDialogue()
{
    return ladderGuideDialogue;
}

const DialogueData& GetHillWatcherDialogue()
{
    return hillWatcherDialogue;
}

const DialogueData& GetHouseOwnerDialogue()
{
    return houseOwnerDialogue;
}

const DialogueData& GetCartographerDialogue()
{
    return cartographerDialogue;
}

const DialogueData& GetHerbalistDialogue()
{
    return herbalistDialogue;
}
}