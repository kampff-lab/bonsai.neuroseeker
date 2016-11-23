using OpenCV.Net;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bonsai.NeuroSeeker
{
    public class NskDataFrame
    {
        public Mat AmplifierData { get; private set; }
        public Mat SyncData { get; private set; }

        public NskDataFrame(Mat amplifier_data, Mat sync_data)
        {
            AmplifierData = amplifier_data;
            SyncData = sync_data;
        }
    }
}
