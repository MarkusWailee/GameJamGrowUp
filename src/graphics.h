#pragma once
#include <math/mat.h>
#include <cassert>
#include <unordered_map>
#include <string>

#include <raylib/raylib.h>
// a simple wrapper over raylib shaders
namespace gx
{
    class Shader
    {
        std::unordered_map<std::string, int> uniforms;
        ::Shader shader;
        int GetLocation(const std::string& loc);
    public:
        template<typename Func>
        void Start(Func&& func);
        void Begin();
        void End();
        ::Shader GetShader();

        void SetFloat(float v, const std::string& loc);       // Shader uniform type: float
        void SetVec2(vec2 v,const std::string& loc);          // Shader uniform type: vec2 (2 float)
        void SetVec3(vec3 v,const std::string& loc);           // Shader uniform type: vec3 (3 float)
        void SetVec4(vec4 v,const std::string& loc);            // Shader uniform type: vec4 (4 float)
        void SetInt(int v, const std::string& loc);             // Shader uniform type: int
        void SetIvec2(ivec4 v, const std::string& loc);           // Shader uniform type: ivec2 (2 int)
        void SetIvec3(ivec4 v, const std::string& loc);           // Shader uniform type: ivec3 (3 int)
        void SetIvec4(ivec4 v, const std::string& loc);           // Shader uniform type: ivec4 (4 int)
        void SetSampler2D(::Texture tex, const std::string& loc);        // Shader uniform type: sampler2d
        Shader(const char* vert, const char* frag);
        ~Shader();
    };

    class Texture
    {
        ::Texture texture{};
    public:
        Texture();
        Texture(Texture&& tex);
        Texture(void* data, int width, int height, PixelFormat pixel_format);
        Texture(const char* file);
        ~Texture();
        void Update(const void* pixels);
        ::Texture GetTexture();
    };




    inline Shader::Shader(const char* vert, const char* frag)
    {
        shader = LoadShader(vert, frag);
        assert(IsShaderValid(shader) && "Shader not valid");
    }
    inline Shader::~Shader()
    {
        if(IsShaderValid(shader))
            UnloadShader(shader);
    }
    inline ::Shader Shader::GetShader()
    {
        return this->shader;
    }

    template<typename Func>
    void Shader::Start(Func&& func)
    {
        this->Begin();
        func();
        this->End();
    }

    inline int Shader::GetLocation(const std::string& loc)
    {
        auto it = uniforms.find(loc);
        if(it == uniforms.end())
        {
            int loc_id = ::GetShaderLocation(shader, loc.c_str());
            if(loc_id != -1)
                uniforms[loc] = loc_id;
            return loc_id;
        }
        return uniforms[loc];
    }



    inline void Shader::Begin()
    {
        ::BeginShaderMode(shader); //
    }
    inline void Shader::End()
    {
        ::EndShaderMode();
    }

    inline void Shader::SetSampler2D(::Texture tex, const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValueTexture(shader, id, tex); //Raylib checks for -1
    }
    inline void Shader::SetFloat(float v, const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_FLOAT);
    }

    inline void Shader::SetVec2(vec2 v,const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_VEC2);
    }

    inline void Shader::SetVec3(vec3 v,const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_VEC3);
    }

    inline void Shader::SetVec4(vec4 v,const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_VEC4);
    }

    inline void Shader::SetInt(int v, const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_INT);
    }

    inline void Shader::SetIvec2(ivec4 v, const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_IVEC2);
    }

    inline void Shader::SetIvec3(ivec4 v, const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_IVEC3);
    }

    inline void Shader::SetIvec4(ivec4 v, const std::string& loc)
    {
        int id = GetLocation(loc);
        ::SetShaderValue(shader, id, &v, SHADER_UNIFORM_IVEC4);
    }



    inline Texture::Texture()
    {

    }
    inline Texture::Texture(Texture&& tex)
    {
        this->texture = tex.GetTexture();
        tex.texture = ::Texture{};
    }
    inline Texture::~Texture()
    {
        UnloadTexture(this->texture);
    }
    inline Texture::Texture(void* data, int width, int height, PixelFormat pixel_format)
    {
        Image image;
        image.data = data;
        image.width = width;
        image.height = height;
        image.format = pixel_format;
        this->texture = LoadTextureFromImage(image);
    }
    inline Texture::Texture(const char* file)
    {
        this->texture = LoadTexture(file);
    }
    inline void Texture::Update(const void* pixels)
    {
        assert(IsTextureValid(this->texture));
        UpdateTexture(this->texture, pixels);
    }
    inline ::Texture Texture::GetTexture()
    {
        assert(IsTextureValid(this->texture));
        return this->texture;
    }

}
