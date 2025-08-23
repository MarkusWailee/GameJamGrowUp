#pragma once

#include <math/vec.h>
#include <cassert>
#include "raylib/raylib.h"
#include "graphics.h"
// #include <vector>
// #include <queue>


namespace GJ
{

    struct Color { unsigned char r = 0, g = 0, b = 0, a = 0; };
    constexpr Color HexToRGBA(const char* text, bool* error);

    enum class Material : unsigned char
    {
        AIR,
        GRASS,
        SAND,
        ROCK,
    };


    Color MaterialToColor(Material mat);

    template<int WIDTH>
    struct PixelChunk
    {
        ivec2 pos;
        Material world_data[WIDTH * WIDTH]{};
        Material GetPixel(ivec2 pos, Material mat) const;
        void SetPixel(ivec2 pos, Material mat);
    };

    template<int WIDTH>
    class ChunkManager
    {

    };
}




//Implementation
namespace GJ
{
    constexpr Color HexToRGBA(const char* text, bool* error = nullptr)
    {
        auto HexToU32 = [](const char* text, bool* error = nullptr) -> uint32_t
        {
            if(!text)
            {
                if(error)
                    *error = true;
                return 0;
            }
            uint32_t result = 0;
            for(; *text; text++)
            {
                result <<= 4;
                char c = *text;
                c = (c >= 'A' && c <= 'Z')? c + 32: c;
                if(c >= '0' && c <= '9')
                    result |= c - '0';
                else if(c >= 'a' && c<= 'f')
                    result |= c - 87;
                else
                {
                    if(error)
                        *error = true;
                    return 0;
                }
            }
            return result;
        };

        bool err = false;
        if(!text)
            err = true;
        text = *text == '#'? text + 1: text;
        char hex[3]{};
        Color result = {0, 0, 0, 255};
        for(int i = 0; i<6; i++)
        {
            if(text[i] == '\0')
                err = true;
        }
        hex[0] = text[0]; hex[1] = text[1]; hex[2] = '\0';
        result.r = HexToU32(hex, &err);
        hex[0] = text[2]; hex[1] = text[3]; hex[2] = '\0';
        result.g = HexToU32(hex, &err);
        hex[0] = text[4]; hex[1] = text[5]; hex[2] = '\0';
        result.b = HexToU32(hex, &err);
        if(text[6] == '\0')
        {
            if(error)
                *error = err;
            return err? Color() : result;
        }
        if(text[7] == '\0')
        {
            if(error)
                *error = true;
            return Color();
        }
        hex[0] = text[6]; hex[1] = text[7]; hex[2] = '\0';
        result.a = HexToU32(hex, &err);
        if(text[8] != '\0')
            err = true;
        if(error)
            *error = err;
        return err? Color(): result;
    }

    inline Color MaterialToColor(Material mat)
    {
        const char* colors[] =
        {
            "#000000",
            "#d1b187",
            "#c77b58",
            "#ae5d40",
            "#79444a",
            "#4b3d44",
            "#ba9158",
            "#927441",
            "#4d4539",
            "#77743b",
            "#b3a555",
            "#d2c9a5",
            "#8caba1",
            "#4b726e",
            "#574852",
            "#847875",
            "#ab9b8e",
        };
        return HexToRGBA(colors[(int)mat]);
    }


    template<int WIDTH>
    Material PixelChunk<WIDTH>::GetPixel(ivec2 pos, Material mat) const
    {
        return Material::AIR;
    }

    template<int WIDTH>
    void PixelChunk<WIDTH>::SetPixel(ivec2 pos, Material mat)
    {
        if(pos.x >= 0 && pos.y >= 0 && pos.x < WIDTH && pos.y < WIDTH)
            world_data[pos.x + pos.y * WIDTH] = mat;
    }

}
