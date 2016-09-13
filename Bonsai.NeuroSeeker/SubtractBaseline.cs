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
    public class SubtractBaseline : Transform<TSource, TResult>
    {
        // Properties
        [Description("Number of Rows")]
        public int n_rows { get; set; }
        [Description("Number of Columns")]
        public int n_cols { get; set; }
        [Description("Alpha")]
        public double alpha { get; set; }

        public Mat baseline_vec;
        public Mat prev_baseline_vec;
        public Mat baseline_mat;
        public Mat output;
        private int frame_count = 0;

        // Constructor (set defaults)
        public SubtractBaseline()
        {
            n_rows = 1440;
            n_cols = 500;
            alpha = 0.1;
        }

        public override IObservable<TResult> Process(IObservable<TSource> source)
        {
            
            return source.Select(input =>
            {
                // If "first frame", the prev = current
                if (frame_count == 0)
                {
                    // Pre-allocate space
                    baseline_vec = new Mat(n_rows, 1, Depth.F32, 1);
                    prev_baseline_vec = new Mat(n_rows, 1, Depth.F32, 1);
                    baseline_mat = new Mat(n_rows, n_cols, Depth.F32, 1);
                    output = new Mat(n_rows, n_cols, Depth.F32, 1);

                    // Average along rows
                    CV.Reduce(input, baseline_vec, 1, ReduceOperation.Avg);
                    CV.Copy(baseline_vec, prev_baseline_vec);
                } else {
                    // Average along rows
                    CV.Reduce(input, baseline_vec, 1, ReduceOperation.Avg);
                }

                // Compute running average of baseline and save as previous
                CV.AddWeighted(baseline_vec, alpha, prev_baseline_vec, (1.0 - alpha), 0.0, baseline_vec);
                CV.Copy(baseline_vec, prev_baseline_vec);

                // Replicate (to produce a frame for subtraction)
                CV.Repeat(baseline_vec, baseline_mat);

                // Subtract basline_mat from inout output
                CV.Sub(input, baseline_mat, output);

                // Update frame counter
                frame_count++;

                return output;


            });
        }
    }
}
