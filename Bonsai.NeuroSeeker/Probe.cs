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
    public class Probe : Source<TSource>
    {
        // Class variables
        IObservable<OpenCV.Net.Mat> source;
        private int n_channels = 1440;
        private static float[] float_buffer;

        // Properties
        [Category("Acquisition")]
        [Description("Sample Buffer Size")]
        public int BufferSize { get; set; }

        [Description("Headstage LEDs")]
        public bool LEDs { get; set; }

        [Category("Testing")]
        [Description("Enable TEST mode (generate sinewave on specified channel)")]
        public bool TestMode { get; set; }

        [Category("Acquisition")]
        [Description("Stream Recording Switch")]
        public bool Stream { get; set; }

        [Category("Acquisition")]
        [Description("Stream Recording File")]
        public string StreamFile { get; set; }

        [Category("Configuration")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("ADC Offset Calibration CSV")]
        public string OffsetCSV { get; set; }

        [Category("Configuration")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("ADC Slope Calibration CSV")]
        public string SlopeCSV { get; set; }

        [Category("Configuration")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("Comparator Calibration CSV")]
        public string CompCSV { get; set; }

        [Category("Configuration")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("Channels Calibration CSV")]
        public string ChannelsCSV { get; set; }

        [Category("Configuration")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("Bias Volatge (0 - 2.5V)")]
        public float BiasVoltage { get; set; }

        [Category("Configuration")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("Update Bias Volatge")]
        public bool UpdateBias { get; set; }

        [Category("Configuration")]
        [Editor("Bonsai.Design.OpenFileNameEditor, Bonsai.Design", typeof(UITypeEditor))]
        [Description("Active Regions")]
        public bool[] ActiveRegions { get; set; }

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void NSK_Open(bool LEDs);

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void NSK_Configure(int[] ActiveRegions, bool TestMode, float BiasVoltage, string OffsetCSV, string SlopeCSV, string CompCSV, string ChannelsCSV);


        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void NSK_Start(int BufferSize, bool Stream, string StreamFile);

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr NSK_Read();
        public static OpenCV.Net.Mat NSK_Read(int n_channels, int buffer_size)
        {
            OpenCV.Net.Mat ReturnArray = new OpenCV.Net.Mat(n_channels, buffer_size, OpenCV.Net.Depth.F32, 1);
            // This seems unecessary
            Marshal.Copy(NSK_Read(), float_buffer, 0, n_channels * buffer_size);
            Marshal.Copy(float_buffer, 0, ReturnArray.Data, n_channels * buffer_size);
            return ReturnArray;
        }


        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void NSK_SetBiasVoltage(float BiasVoltage);

        // Import relevant functions from Nsk C DLL
        [DllImport("NeuroSeeker_C_DLL", CallingConvention = CallingConvention.Cdecl)]
        public static extern void NSK_Close();

        // Constructor for Probe Class 
        public Probe()
        {
            // Set Default values
            BufferSize = 500;

            ActiveRegions = new bool[12];
            int[] ActiveRegionsMarshal = new int[12];
            // Create a source of CvMats
            source = Observable.Create<OpenCV.Net.Mat>(observer =>
            {
                // Open and Initialize
                NSK_Open(LEDs);

                // Configure ADCs and Channels
                for (int i = 0; i < ActiveRegions.Length; ++i)
                    ActiveRegionsMarshal[i] = Convert.ToByte(ActiveRegions[i]);
                NSK_Configure(ActiveRegionsMarshal, TestMode, BiasVoltage, OffsetCSV, SlopeCSV, CompCSV, ChannelsCSV);

                // Start Probe thread
                float_buffer = new float[n_channels * BufferSize];
                if (StreamFile == null)
                {
                    StreamFile = System.IO.Path.Combine(System.IO.Directory.GetParent(CompCSV).ToString(), "datalog.nsk");
                }
                NSK_Start(BufferSize, Stream, StreamFile);
                var running = true;
                var thread = new Thread(() =>
                {
                    while (running)
                    {
                        // Read all channels from probe
                        observer.OnNext(NSK_Read(n_channels, BufferSize));

                        // Read counter and synchro

                        // Adjust Bias Voltage (online)
                        if(UpdateBias)
                        {
                            NSK_SetBiasVoltage(BiasVoltage);
                            UpdateBias = false;
                        }
                    }
                });

                thread.Start();
                return () =>
                {
                    // Stop acquisition thread
                    running = false;
                    thread.Join();

                    // Close probe
                    NSK_Close();

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
