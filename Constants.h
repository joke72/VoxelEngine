#pragma once
#include <iostream>

const int32_t CHUNK_SIZE = 128;
const int32_t CHUNK_HEIGHT = 128;

const int32_t CHUNKS_X = 8;
const int32_t CHUNKS_Y = 1;
const int32_t CHUNKS_Z = 8;

const int32_t CHUNKS = CHUNKS_X * CHUNKS_Y * CHUNKS_Z;

const int32_t CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT;

const int32_t ARRAY_SIZE = CHUNK_VOLUME * CHUNKS + CHUNKS;
