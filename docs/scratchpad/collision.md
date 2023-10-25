# Collision Detection!

## Possible types of collision
- bullet + player
    - rotated trigger collider <> collider
- player + obstacle
    - point <> isocollider
    - maybe isocollider <> isocollider in the future
- enemy + obstacle
    - point <> isocollider
    - maybe isocollider <> isocollider in the future
- bullet + obstacle
    - rotated trigger collider <> collider
    - obstacle hurtbox is not the projection in this case!
- enemy + player
    - isocollider <> isocollider I think
    - Can't be point <> point !!

## Tags
- player, entity, bullet, terrain

## Collider matches TL;DR
- rotated trigger collider <> collider
- point <> isocollider
- isocollider <> isocollider in the future
