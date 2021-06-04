#include "config.h"
#include "glmaterial.h"
#include "glshader_loader.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        static const char* shader_urls[] = {
                LOVELACE_ROOT_DIR "shaders/basic_color.glsl",
                LOVELACE_ROOT_DIR "shaders/basic_color_interpolated.glsl",
                LOVELACE_ROOT_DIR "shaders/basic_texture.glsl",
                LOVELACE_ROOT_DIR "shaders/basic_texture_interpolated.glsl",
                LOVELACE_ROOT_DIR "shaders/phong.glsl",
                LOVELACE_ROOT_DIR "shaders/gouraud.glsl",
                LOVELACE_ROOT_DIR "shaders/phong_textured.glsl",
        };

        ////////////////////////////////////////////////////////////////////////////////
        enum ShaderUrlIndex {
                BasicColor,
                BasicColorInterpolated,
                BasicTexture,
                BasicTextureInterpolated,
                Phong,
                Gouraud,
                PhongTextured,
        };

        ////////////////////////////////////////////////////////////////////////////////
        abstract_material::abstract_material(int shader_url_index)
                : mVshader{ shader_loader::load(GL_VERTEX_SHADER, shader_urls[shader_url_index]) },
                  mFshader{ shader_loader::load(GL_FRAGMENT_SHADER, shader_urls[shader_url_index]) },
                  mProgram{ mVshader, mFshader } {}

        ////////////////////////////////////////////////////////////////////////////////
        basic_color_material::basic_color_material()
                : abstract_material(BasicColor) {}

        ////////////////////////////////////////////////////////////////////////////////
        basic_color_interpolated_material::basic_color_interpolated_material()
                : abstract_material(BasicColorInterpolated) {}

        ////////////////////////////////////////////////////////////////////////////////
        basic_texture_material::basic_texture_material()
                : abstract_material(BasicTexture)
        {
                mProgram.use();
                mProgram.uniform("uTexture", 0);
                mProgram.halt();
        }

        ////////////////////////////////////////////////////////////////////////////////
        basic_texture_interpolated_material::basic_texture_interpolated_material()
                : abstract_material(BasicTextureInterpolated)
        {
                mProgram.use();
                mProgram.uniform("uTexture0", 0);
                mProgram.uniform("uTexture1", 1);
                mProgram.halt();
        }

        ////////////////////////////////////////////////////////////////////////////////
        phong_material::phong_material()
                : abstract_material(Phong) {}

        ////////////////////////////////////////////////////////////////////////////////
        phong_textured_material::phong_textured_material()
                : abstract_material(PhongTextured)
        {
                mProgram.use();
                mProgram.uniform("uMaterial.diffuse", 0);
                mProgram.uniform("uMaterial.specular", 1);
                mProgram.halt();
        }

        ////////////////////////////////////////////////////////////////////////////////
        gouraud_material::gouraud_material()
                : abstract_material(Gouraud) {}
}
