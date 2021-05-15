#include "config.h"
#include "glmaterial.h"
#include "glshader_loader.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        static const char* shader_urls[] = {
                /* Vertex Shader */                                                       /* Fragment Shader */
                LOVELACE_ROOT_DIR "shaders/basic_color.vert",                             LOVELACE_ROOT_DIR "shaders/basic_color.frag",
                LOVELACE_ROOT_DIR "shaders/basic_color_interpolated.vert",                LOVELACE_ROOT_DIR "shaders/basic_color_interpolated.frag",
                LOVELACE_ROOT_DIR "shaders/basic_texture.vert",                           LOVELACE_ROOT_DIR "shaders/basic_texture.frag",
                LOVELACE_ROOT_DIR "shaders/basic_texture_interpolated.vert",              LOVELACE_ROOT_DIR "shaders/basic_texture_interpolated.frag",
                LOVELACE_ROOT_DIR "shaders/phong.vert",                                   LOVELACE_ROOT_DIR "shaders/phong.frag",
                LOVELACE_ROOT_DIR "shaders/gouraud.vert",                                 LOVELACE_ROOT_DIR "shaders/gouraud.frag",
                LOVELACE_ROOT_DIR "shaders/phong_textured.vert",                          LOVELACE_ROOT_DIR "shaders/phong_textured.frag"
        };

        ////////////////////////////////////////////////////////////////////////////////
        enum ShaderUrlIndex {
                /* Vertex Shader */                                     /* Fragment Shader */
                BasicColorVert,                                         BasicColorFrag,
                BasicColorInterpolatedVert,                             BasicColorInterpolatedFrag,
                BasicTextureVert,                                       BasicTextureFrag,
                BasicTextureInterpolatedVert,                           BasicTextureInterpolatedFrag,
                PhongVert,                                              PhongFrag,
                GouraudVert,                                            GouraudFrag,
                PhongTexturedVert,                                      PhongTexturedFrag
        };

        ////////////////////////////////////////////////////////////////////////////////
        abstract_material::abstract_material(int vshader_url_index, int fshader_url_index)
                : mVshader{shader_loader::load(shader_urls[vshader_url_index])},
                  mFshader{shader_loader::load(shader_urls[fshader_url_index])},
                  mProgram{ mVshader, mFshader } {}

        ////////////////////////////////////////////////////////////////////////////////
        basic_color_material::basic_color_material()
                : abstract_material(BasicColorVert, BasicColorFrag) {}

        ////////////////////////////////////////////////////////////////////////////////
        basic_color_interpolated_material::basic_color_interpolated_material()
                : abstract_material(BasicColorInterpolatedVert, BasicColorInterpolatedFrag) {}

        ////////////////////////////////////////////////////////////////////////////////
        basic_texture_material::basic_texture_material()
                : abstract_material(BasicTextureVert, BasicTextureFrag)
        {
                mProgram.use();
                mProgram.uniform("uTexture", 0);
                mProgram.halt();
        }

        ////////////////////////////////////////////////////////////////////////////////
        basic_texture_interpolated_material::basic_texture_interpolated_material()
                : abstract_material(BasicTextureInterpolatedVert, BasicTextureInterpolatedFrag)
        {
                mProgram.use();
                mProgram.uniform("uTexture0", 0);
                mProgram.uniform("uTexture1", 1);
                mProgram.halt();
        }

        ////////////////////////////////////////////////////////////////////////////////
        phong_material::phong_material()
                : abstract_material(PhongVert, PhongFrag) {}

        ////////////////////////////////////////////////////////////////////////////////
        phong_textured_material::phong_textured_material()
                : abstract_material(PhongTexturedVert, PhongTexturedFrag)
        {
                mProgram.use();
                mProgram.uniform("uMaterial.diffuse", 0);
                mProgram.uniform("uMaterial.specular", 1);
                mProgram.halt();
        }

        ////////////////////////////////////////////////////////////////////////////////
        gouraud_material::gouraud_material()
                : abstract_material(GouraudVert, GouraudFrag) {}
}
