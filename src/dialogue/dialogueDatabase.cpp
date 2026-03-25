#include "dialogue/DialogueDatabase.hpp"

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

    const DialogueData guardAfterDialogue{
        "Straznik",
        {
            "Znowu ty?",
            "Mowilem juz wszystko co wiem."
        },
        {}
    };

    const DialogueData guardQuestStartDialogue{
        "Straznik",
        {
            "Mam dla ciebie proste zadanie.",
            "Porozmawiaj z kartografem i zielarka.",
            "Wroc do mnie, gdy zbierzesz od nich informacje."
        },
        {}
    };

    const DialogueData guardQuestInProgressDialogue{
        "Straznik",
        {
            "Wciaz czekam na wiesci.",
            "Porozmawiaj z kartografem i zielarka, a potem wroc."
        },
        {}
    };

    const DialogueData guardQuestCompleteDialogue{
        "Straznik",
        {
            "Dobra robota.",
            "Mamy juz to, czego potrzebowalem.",
            "To dopiero prosty poczatek, ale zadanie uznaje za wykonane."
        },
        {}
    };

    const DialogueData guardQuestFinishedDialogue{
        "Straznik",
        {
            "Dzieki za pomoc.",
            "Gdy bede mial kolejne zadanie, dam ci znac."
        },
        {}
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

    const DialogueData cartographerQuestDialogue{
        "Kartograf",
        {
            "A wiec straznik cie przyslal?",
            "Dobrze. Zanotuj, ze droga na wschod wciaz nie jest gotowa.",
            "Mozesz przekazac mu te informacje."
        },
        {}
    };

    const DialogueData cartographerAfterQuestDialogue{
        "Kartograf",
        {
            "Przekazales juz straznikowi to, co powiedzialem?",
            "Mam jeszcze sporo pracy nad mapami."
        },
        {}
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

    const DialogueData herbalistQuestDialogue{
        "Zielarka",
        {
            "Straznik pyta o okolice?",
            "Powiedz mu, ze na lace jest spokojnie, ale na dalsza wyprawe przydalby sie zapas ziol.",
            "To wszystko, co moge dodac."
        },
        {}
    };

    const DialogueData herbalistAfterQuestDialogue{
        "Zielarka",
        {
            "Jesli dalej pomagasz straznikowi, powiedz mu tez, zeby uwazal na zapasy.",
            "Bez leczenia daleko nie zajdzie."
        },
        {}
    };

    const DialogueData hillWatcherQuestDialogue{
        "Obserwator",
        {
            "Spotkałeś pewnie wedrowca na swojej drodze.",
            "Pozdrów go ode mnie."
        },
        {}
    };

    const DialogueData hillWatcherAfterQuestDialogue{
        "Obserwator",
        {
            "Lubię oglądać te wzgórza, ale lepije w towarzystwie."
            
        },
        {}
    };

    const DialogueData travelerQuestStartDialogue{
        "Wedrowiec",
        {
            "Czesc!",
            "Mógłbyś mi pomóc z pewnym zadaniem?",
            "Idz sprawdz co u wędrowca na wzgorzu.",
        },
        {}
    };

    const DialogueData travelerQuestInProgressDialogue{
        "Wedrowiec",
        {
            "Hej, jak idzie z tym zadaniem?",
            "Ja czekam na jakieś informacje."
        },
        {}
    };

    const DialogueData travelerQuestCompleteDialogue{
        "Wedrowiec",
        {
            "Dzieki za pomoc!",
            "To dobry czlowiek, ciesze sie, ze nic mu nie jest."
        },
        {}
    };

    const DialogueData travelerQuestFinishedDialogue{
        "Wedrowiec",
        {
            "Zrobiles wszystko, co moglbyś?",
            "To bardzo pomocne."
        },
        {}
    };
}


namespace DialogueDatabase
{
    const DialogueData& GetTravelerDialogue() { return travelerDialogue; }
    const DialogueData& GetGuardDialogue() { return guardDialogue; }
    const DialogueData& GetGuardAfterDialogue() { return guardAfterDialogue; }

    const DialogueData& GetGuardQuestStartDialogue() { return guardQuestStartDialogue; }
    const DialogueData& GetGuardQuestInProgressDialogue() { return guardQuestInProgressDialogue; }
    const DialogueData& GetGuardQuestCompleteDialogue() { return guardQuestCompleteDialogue; }
    const DialogueData& GetGuardQuestFinishedDialogue() { return guardQuestFinishedDialogue; }

    const DialogueData& GetLadderGuideDialogue() { return ladderGuideDialogue; }
    const DialogueData& GetHillWatcherDialogue() { return hillWatcherDialogue; }
    const DialogueData& GetHouseOwnerDialogue() { return houseOwnerDialogue; }

    const DialogueData& GetCartographerDialogue() { return cartographerDialogue; }
    const DialogueData& GetCartographerQuestDialogue() { return cartographerQuestDialogue; }
    const DialogueData& GetCartographerAfterQuestDialogue() { return cartographerAfterQuestDialogue; }

    const DialogueData& GetHerbalistDialogue() { return herbalistDialogue; }
    const DialogueData& GetHerbalistQuestDialogue() { return herbalistQuestDialogue; }
    const DialogueData& GetHerbalistAfterQuestDialogue() { return herbalistAfterQuestDialogue; }

    const DialogueData& GetHillWatcherQuestDialogue() { return hillWatcherQuestDialogue; }
    const DialogueData& GetHillWatcherAfterQuestDialogue() { return hillWatcherAfterQuestDialogue; }

    const DialogueData& GetTravelerQuestFinishedDialogue() { return travelerQuestFinishedDialogue; }
    const DialogueData& GetTravelerQuestCompleteDialogue() { return travelerQuestCompleteDialogue; }
    const DialogueData& GetTravelerQuestInProgressDialogue() { return travelerQuestInProgressDialogue; }
    const DialogueData& GetTravelerQuestStartDialogue() { return travelerQuestStartDialogue; }
}
