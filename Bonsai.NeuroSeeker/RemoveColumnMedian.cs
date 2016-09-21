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
    public class RemoveColumnMedian : Transform<TSource, TResult>
    {
        public override IObservable<TResult> Process(IObservable<TSource> source)
        {
            // The function passed to Defer is called on every subscription ("every run/repeat") 
            return Observable.Defer(() =>
            {
                Mat median_vec = null;
                Mat median_mat = null;
                Mat sorted = null;
                Mat output = null;
                int frame_count = 0;
                var n_rows = 1440;
                var n_cols = 500;

                return source.Select(input =>
                {
                    // If "first frame", the prev = current
                    if (frame_count == 0)
                    {
                        // Determine MAT size
                        n_rows = input.Size.Height;
                        n_cols = input.Size.Width;

                        // Pre-allocate space
                        median_vec = new Mat(1, n_cols, Depth.F32, 1);
                        median_mat = new Mat(n_rows, n_cols, Depth.F32, 1);
                        sorted= new Mat(n_rows, n_cols, Depth.F32, 1);
                        output = new Mat(n_rows, n_cols, Depth.F32, 1);                     
                    }

                    // Sort columns
                    CV.Sort(input, sorted, null, SortFlags.EveryColumn);

                    // Select middle row of sorted matrix as the Median
                    median_vec = sorted.GetRow(n_rows / 2);
                    
                    // Replicate (to produce a frame for subtraction)
                    CV.Repeat(median_vec, median_mat);

                    // Subtract basline_mat from inout output
                    CV.Sub(input, median_mat, output);

                    // Update frame counter
                    frame_count++;

                    return output;
                });
            });
        }
    }
}
