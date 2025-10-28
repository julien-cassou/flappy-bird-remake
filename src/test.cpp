#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "include/jeu.hpp"
#include <SFML/System/Vector2.hpp>

TEST_CASE("Bird") {
    SUBCASE("getColor") {
        Bird b {Color::Bleu};
        CHECK(b.getColor() == Color::Bleu);
        Bird b2 {Color::Rouge};
        CHECK(b2.getColor() == Color::Rouge);
    }
    SUBCASE("getPosition / tombe / saut") {
        Bird b {Color::Vert};
        b.saut();
        CHECK(b.getPosition() == sf::Vector2f {150.f, 380.f});
        b.saut();
        CHECK(b.getPosition() == sf::Vector2f {150.f, 360.f});
        b.tombe();
        CHECK(b.getPosition() == sf::Vector2f {150, 360.5});
    }
}