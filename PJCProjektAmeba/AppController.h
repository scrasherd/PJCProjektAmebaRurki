#pragma once
#include <SFML/Graphics.hpp>
#include "IMapRenderer.h"
#include "FoodField.h"
#include "RenderController.h"
#include "SimulationController.h"
#include <memory>

/**
 * @brief G³ówny kontroler aplikacji.
 *
 * Odpowiada za inicjalizacjê, pêtlê g³ówn¹ oraz zarz¹dzanie komponentami takimi jak:
 * mapa po¿ywienia, kontroler symulacji i renderowania, oraz okno renderuj¹ce.
 */

class AppController {
private:
    /// Szerokoœæ mapy
    int mapWidth;
    /// Wysokoœæ mapy
    int mapHeight;

    /// G³ówne okno aplikacji
    sf::RenderWindow window;

    /// Klasa mapy po¿ywienia
    FoodField foodField;

    /// Kontroler Plasmodium
    std::unique_ptr<IPlasmodiumController> plasmodium;

    /// Kontroler renderowania
    std::unique_ptr<RenderController> renderController;
    /// Kontroler symulacji
    SimulationController simulationController;

    /// Zegar kontroluj¹cy czas pomiêdzy klatkami
    sf::Clock clock;
public:

     /**
     * @brief Konstruktor AppControllera.
     *
     * @param mapWidth Szerokoœæ mapy.
     * @param mapHeight Wysokoœæ mapy.
     * @param tubeLength D³ugoœæ podstawowej rurki w symulacji (przekazywana dalej).
     */
    AppController(int mapWidth, int mapHeight, float tubeLength);

    /**
     * @brief Uruchamia g³ówn¹ pêtlê aplikacji.
     *
     * Obs³uguje logikê symulacji, renderowanie oraz interakcjê z u¿ytkownikiem.
     */
    void run();

};
