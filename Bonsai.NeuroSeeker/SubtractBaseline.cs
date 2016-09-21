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
        [Description("Alpha")]
        public double Alpha { get; set; }

        // Constructor (set defaults)
        public SubtractBaseline()
        {
            Alpha = 0.1;
        }

        public override IObservable<TResult> Process(IObservable<TSource> source)
        {
            // The function passed to Defer is called on every subscription ("every run/repeat") 
            return Observable.Defer(() =>
            {
                Mat baseline_vec = null;
                Mat acc = null;
                Mat baseline_mat = null;
                Mat output = null;
                int frame_count = 0;

                return source.Select(input =>
                {
                   // If "first frame", the prev = current
                   if (frame_count == 0)
                    {
                        // Determine MAT size
                        var n_rows = input.Size.Height;
                        var n_cols = input.Size.Width;
                        
                        // Pre-allocate space
                        baseline_vec = new Mat(n_rows, 1, Depth.F32, 1);
                        acc = new Mat(n_rows, 1, Depth.F32, 1);
                        baseline_mat = new Mat(n_rows, n_cols, Depth.F32, 1);
                        output = new Mat(n_rows, n_cols, Depth.F32, 1);

                        // Average along rows
                        CV.Reduce(input, baseline_vec, 1, ReduceOperation.Avg);
                        CV.Copy(baseline_vec, acc);
                    }
                    else
                    {
                        // Average along rows
                        CV.Reduce(input, baseline_vec, 1, ReduceOperation.Avg);
                    }

                    // Compute running average of baseline and save as previous
                    CV.RunningAvg(baseline_vec, acc, Alpha);

                    // Replicate (to produce a frame for subtraction)
                    CV.Repeat(acc, baseline_mat);

                    // Subtract basline_mat from inout output
                    CV.Sub(input, baseline_mat, output);

                    // Update frame counter
                    frame_count++;

                    return output;
                });
            });
        }
    }
}
