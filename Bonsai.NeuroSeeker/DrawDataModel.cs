using Bonsai.Shaders;
using OpenTK.Graphics.OpenGL4;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing.Design;
using System.Linq;
using System.Reactive.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bonsai.NeuroSeeker
{
    public class DrawDataModel : Sink<DataModel>
    {
        [Editor("Bonsai.Shaders.Configuration.Design.ShaderConfigurationEditor, Bonsai.Shaders.Design", typeof(UITypeEditor))]
        public string ShaderName { get; set; }

        public string TextureUniformName { get; set; }

        public override IObservable<DataModel> Process(IObservable<DataModel> source)
        {
            return source.CombineEither(
                ShaderManager.ReserveShader(ShaderName).Do(shader =>
                {
                    var samplerLocation = GL.GetUniformLocation(shader.Program, TextureUniformName);
                    if (samplerLocation >= 0)
                    {
                        GL.Uniform1(samplerLocation, 0);
                    }
                }),
                (input, shader) =>
                {
                    shader.Update(() =>
                    {
                        foreach (var modelPart in input.ModelParts)
                        {
                            GL.ActiveTexture(TextureUnit.Texture0);
                            GL.BindTexture(TextureTarget.Texture2D, modelPart.Texture.Id);
                            modelPart.Mesh.Draw();
                        }
                    });
                    return input;
                });
        }
    }
}
