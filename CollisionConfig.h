#pragma once

#include <cstdint>

// 味方陣営 (0000 0001)
const uint32_t kCollisionAttributePlayer = 0b1;

// 敵陣営 (0000 0010)
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
