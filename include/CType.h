#pragma once

enum class CType {
    Sprite,
    Sound,
    Face,
    KeepSoundAlive,
    TileMap,
    CameraFollower,
    Alien,
    Minion,
    Bullet,
    PenguinBody,
    PenguinCannon,
    Collider,
};

// This really should be generated code...
inline const char* CTypeString(CType type) {
    static const char* strings[] = {
        "Sprite", "Sound", "Face", "KeepSoundAlive", "TileMap", "CameraFollower", "Alien", "Minion", "Bullet", "PenguinBody", "PenguinCannon", "Collider"
    };
    return strings[(int)type];
}
