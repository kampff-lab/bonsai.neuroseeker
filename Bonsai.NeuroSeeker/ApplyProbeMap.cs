using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Reactive.Linq;
using System.ComponentModel;
using System.Reactive.Disposables;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using OpenCV.Net;

// TODO: replace this with the transform input and output types.
using TSource = OpenCV.Net.Mat;
using TResult = OpenCV.Net.Mat;

namespace Bonsai.NeuroSeeker
{
    public class ApplyProbeMap : Transform<TSource, TResult>
    {
        // Properties
        [Description("Number of Rows")]
        public int n_rows { get; set; }

        [Description("Number of Columns")]
        public int n_cols { get; set; }

        private int frame_count = 0;
        public Mat output;

        // Constructor (set defaults)
        public ApplyProbeMap()
        {
            n_rows = 1440;
            n_cols = 500;
        }

        public override IObservable<TResult> Process(IObservable<TSource> source)
        {
            return source.Select(input =>
            {
                // If "first frame", allocate space for new map
                if (frame_count == 0)
                {
                    // Pre-allocate space
                    output = new Mat(n_rows, n_cols, Depth.F32, 1);
                }

                // Extract all "good" channels from top to bottom
                for (int r = 0; r < n_rows; r+=8)
                {
                    for (int c = 0; c < n_cols; c++)
                    {
                        output[r * n_cols + c] = input[r * n_cols + c];
                    }

                    for (int c = 0; c < n_cols; c++)
                    {
                        output[(r+4) * n_cols + c] = input[(r + 1) * n_cols + c];
                    }

                    for (int c = 0; c < n_cols; c++)
                    {
                        output[(r + 1) * n_cols + c] = input[(r + 2) * n_cols + c];
                    }
                    
                    for (int c = 0; c < n_cols; c++)
                    {
                        output[(r + 5) * n_cols + c] = input[(r + 3) * n_cols + c];
                    }

                    for (int c = 0; c < n_cols; c++)
                    {
                        output[(r + 2) * n_cols + c] = input[(r + 4) * n_cols + c];
                    }

                    for (int c = 0; c < n_cols; c++)
                    {
                        output[(r + 6) * n_cols + c] = input[(r + 5) * n_cols + c];
                    }

                    for (int c = 0; c < n_cols; c++)
                    {
                        output[(r + 3) * n_cols + c] = input[(r + 6) * n_cols + c];
                    }

                    for (int c = 0; c < n_cols; c++)
                    {
                        output[(r + 7) * n_cols + c] = input[(r + 7) * n_cols + c];
                    }
                }

                // Update frame counter
                frame_count++;

                return output;
         

            });
        }
    }
}
