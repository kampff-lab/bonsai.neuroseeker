using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Reactive.Linq;
using System.ComponentModel;
using System.Drawing.Design;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using OpenCV.Net;

// TODO: replace this with the source output type.
using TSource = Bonsai.NeuroSeeker.NskDataFrame;

namespace Bonsai.NeuroSeeker
{
    public class File : Source<TSource>
    {
        // Class variables
        IObservable<NskDataFrame> source;
        private int n_channels = 1440;

        // Properties
        [Category("Acquisition")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("NeuroSeeker Data File")]
        public string DataFile { get; set; }

        [Category("Acquisition")]
        [Description("Sample Buffer Size")]
        public int BufferSize { get; set; }

        [Category("Acquisition")]
        [Description("Sample Time (ms)")]
        public int Interval { get; set; }

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void NSK_Open_File(string DataFile);

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int NSK_Read_File(IntPtr buffer, IntPtr sync, int buffer_size);
        public static NskDataFrame NSK_Read_File(int n_channels, int buffer_size)
        {
            var result = new OpenCV.Net.Mat(n_channels, buffer_size, OpenCV.Net.Depth.F32, 1);
            var sync = new OpenCV.Net.Mat(1, buffer_size, OpenCV.Net.Depth.U16, 1);
            var samplesRead = NSK_Read_File(result.Data, sync.Data, buffer_size);
            if (samplesRead == 0) return null;
            return new NskDataFrame(result, sync);
        }

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void NSK_Close_File();

        // Constructor for File Class 
        public File()
        {
            // Set Default values
            BufferSize = 500;

            // Create a source of CvMats
            source = Observable.Create<NskDataFrame>((observer, cancellationToken) =>
            {
                return Task.Factory.StartNew(() =>
                {
                    // Open and Initialize
                    NSK_Open_File(DataFile);

                    var bufferSize = BufferSize;
                    using (var close = Disposable.Create(NSK_Close_File))
                    using (var sampleSignal = new ManualResetEvent(false))
                    {
                        while (!cancellationToken.IsCancellationRequested)
                        {
                            var result = NSK_Read_File(n_channels, BufferSize);
                            if (result == null) break;
                            observer.OnNext(result);

                            var interval = Interval;
                            if (interval > 0)
                            {
                                sampleSignal.WaitOne(interval);
                            }
                        }

                        observer.OnCompleted();
                    }
                },
                cancellationToken,
                TaskCreationOptions.LongRunning,
                TaskScheduler.Default);
            });
        }

        // Generate source (whatever)
        public override IObservable<TSource> Generate()
        {
            return source;
        }
    }
}
