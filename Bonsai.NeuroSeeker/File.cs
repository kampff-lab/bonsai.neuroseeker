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
using TSource = OpenCV.Net.Mat;

namespace Bonsai.NeuroSeeker
{
    public class File : Source<TSource>
    {
        // Class variables
        IObservable<OpenCV.Net.Mat> source;
        private int n_channels = 1440;
        private static float[] float_buffer;

        // Properties
        [Category("Acquisition")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("NeuroSeeker Data File")]
        public string DataFile { get; set; }

        [Category("Acquisition")]
        [Description("Sample Buffer Size")]
        public int BufferSize { get; set; }

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void NSK_Open_File(string DataFile, int BufferSize);

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr NSK_Read_File();
        public static OpenCV.Net.Mat NSK_Read_File(int n_channels, int buffer_size)
        {
            OpenCV.Net.Mat ReturnArray = new OpenCV.Net.Mat(n_channels, buffer_size, OpenCV.Net.Depth.F32, 1);
            // This seems unecessary
            Marshal.Copy(NSK_Read_File(), float_buffer, 0, n_channels * buffer_size);
            Marshal.Copy(float_buffer, 0, ReturnArray.Data, n_channels * buffer_size);
            return ReturnArray;
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
            source = Observable.Create<OpenCV.Net.Mat>(observer =>
            {
                // Open and Initialize
                NSK_Open_File(DataFile, BufferSize);

                // Start FIle thread
                float_buffer = new float[n_channels * BufferSize];
                var running = true;
                var thread = new Thread(() =>
                {
                    while (running)
                    {
                        // Read all channels from File
                        observer.OnNext(NSK_Read_File(n_channels, BufferSize));
                        Thread.Sleep(50);

                        // Read counter and synchro
                    }
                });

                thread.Start();
                return () =>
                {
                    // Stop acquisition thread
                    running = false;
                    thread.Join();

                    // Close File
                    NSK_Close_File();

                };
            }).PublishReconnectable().RefCount();
        }

        // Generate source (whatever)
        public override IObservable<TSource> Generate()
        {
            return source;
        }
    }
}
