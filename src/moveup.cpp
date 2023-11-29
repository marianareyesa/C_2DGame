#include "moveup.h"

void moveUp(sf::CircleShape& player, float speed, sf::Time delta){
	sf::Vector2f currentPosition = player.getPosition();

	float displacement = speed * delta.asSeconds();

        currentPosition.x += displacement;

        player.setPosition(currentPosition);
}
