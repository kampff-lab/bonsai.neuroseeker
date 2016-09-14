using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bonsai.NeuroSeeker
{
    public class DataModel
    {
        readonly List<DataModelPart> modelParts;

        public DataModel(IEnumerable<DataModelPart> parts)
        {
            modelParts = new List<DataModelPart>(parts);
        }

        public IList<DataModelPart> ModelParts
        {
            get { return modelParts; }
        }
    }
}
