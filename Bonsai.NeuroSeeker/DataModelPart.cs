using Bonsai.Shaders;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bonsai.NeuroSeeker
{
    public class DataModelPart
    {
        public DataModelPart(Mesh mesh, Texture texture)
        {
            Mesh = mesh;
            Texture = texture;
        }

        public Mesh Mesh { get; private set; }

        public Texture Texture { get; private set; }
    }
}
