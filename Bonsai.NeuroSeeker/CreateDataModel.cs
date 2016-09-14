using Bonsai.Shaders;
using Bonsai.Shaders.Configuration;
using OpenCV.Net;
using OpenTK;
using OpenTK.Graphics.OpenGL4;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bonsai.NeuroSeeker
{
    public class CreateDataModel : Combinator<IplImage, DataModel>
    {
        readonly MeshConfiguration meshConfiguration = new MeshConfiguration();
        readonly TextureConfiguration textureConfiguration = new Texture2D();

        public int BufferLength { get; set; }

        public PixelInternalFormat InternalFormat { get; set; }

        static Mesh CreateTexturedQuad(Vector2 scale, Vector2 shift)
        {
            var mesh = new Mesh();
            var vao = mesh.VertexArray;
            var vbo = mesh.VertexBuffer;
            var vertices = new float[]
            {
                -1f * scale.X + shift.X, -1f * scale.Y + shift.Y, 0f, 0f,
                 1f * scale.X + shift.X, -1f * scale.Y + shift.Y, 1f, 0f,
                 1f * scale.X + shift.X,  1f * scale.Y + shift.Y, 1f, 1f,
                -1f * scale.X + shift.X,  1f * scale.Y + shift.Y, 0f, 1f,
            };

            GL.BindVertexArray(vao);
            GL.BindBuffer(BufferTarget.ArrayBuffer, vbo);
            GL.BufferData(BufferTarget.ArrayBuffer,
                          new IntPtr(vertices.Length * BlittableValueType<float>.Stride),
                          vertices,
                          BufferUsageHint.StaticDraw);
            GL.EnableVertexAttribArray(0);
            GL.EnableVertexAttribArray(1);
            GL.VertexAttribPointer(
                0, 2,
                VertexAttribPointerType.Float,
                false,
                4 * BlittableValueType<float>.Stride,
                0 * BlittableValueType<float>.Stride);
            GL.VertexAttribPointer(
                1, 2,
                VertexAttribPointerType.Float,
                false,
                4 * BlittableValueType<float>.Stride,
                2 * BlittableValueType<float>.Stride);
            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
            GL.BindVertexArray(0);

            mesh.DrawMode = PrimitiveType.Quads;
            mesh.VertexCount = 4;
            return mesh;
        }

        public override IObservable<DataModel> Process(IObservable<IplImage> source)
        {
            return Observable.Defer(() =>
            {
                var i = 0;
                var model = default(DataModel);
                var modelCount = BufferLength;
                return source.CombineEither(
                    ShaderManager.WindowSource.Do(window =>
                    {
                        window.Update(() =>
                        {
                            var scale = new Vector2(1f / modelCount, 1);
                            model = new DataModel(
                                from x in Enumerable.Range(0, modelCount)
                                let shift = new Vector2(-1 + scale.X + 2 * x * scale.X, 0)
                                let mesh = CreateTexturedQuad(scale, shift)
                                let texture = textureConfiguration.CreateResource()
                                select new DataModelPart(mesh, texture));
                        });
                    }),
                    (input, window) =>
                    {
                        window.Update(() =>
                        {
                            var activePart = model.ModelParts[i++ % model.ModelParts.Count];
                            TextureHelper.UpdateTexture(activePart.Texture.Id, InternalFormat, input);
                        });
                        return model;
                    }).Where(input => input != null).Finally(() =>
                    {
                        if (model != null)
                        {
                            foreach (var modelPart in model.ModelParts)
                            {
                                modelPart.Mesh.Dispose();
                                modelPart.Texture.Dispose();
                            }
                        }
                    });
            });
        }
    }
}
