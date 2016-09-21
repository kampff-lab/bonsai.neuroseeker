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
        public override IObservable<TResult> Process(IObservable<TSource> source)
        {
            // The function passed to Defer is called on every subscription ("every run/repeat") 
            return Observable.Defer(() =>
            {
                Mat output = null;
                Scalar ref_val = new Scalar(0.0f);
                int frame_count = 0;
                var n_rows = 1440;
                var n_cols = 500;

                return source.Select(input =>
                {
                    // If "first frame", allocate space for new map
                    if (frame_count == 0)
                    {
                            // Determine MAT size
                            n_rows = input.Size.Height;
                            n_cols = input.Size.Width;

                            // Pre-allocate space
                            output = new Mat(n_rows, n_cols, Depth.F32, 1);
                    }

                    // Extract all "good" channels from top to bottom, set refs to "zero"
                    for (int r = 0; r < n_rows; r += 8)
                    {

                        for (int c = 0; c < n_cols; c++)
                        {
                            output[r * n_cols + c] = input[r * n_cols + c];
                        }

                        for (int c = 0; c < n_cols; c++)
                        {
                            output[(r + 4) * n_cols + c] = input[(r + 1) * n_cols + c];
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
                        
                        // Blank the reference channels
                        for (int c = 0; c < n_cols; c++)
                        {
                            if ((r % 120) == 56)
                            {
                                output[(r + 0) * n_cols + c] = ref_val;
                                output[(r + 1) * n_cols + c] = ref_val;
                                output[(r + 2) * n_cols + c] = ref_val;
                                output[(r + 3) * n_cols + c] = ref_val;
                                output[(r + 4) * n_cols + c] = ref_val;
                                output[(r + 5) * n_cols + c] = ref_val;
                                output[(r + 6) * n_cols + c] = ref_val;
                                output[(r + 7) * n_cols + c] = ref_val;
                            }
                        }
                    }

                    // Update frame counter
                    frame_count++;

                    return output;
                });
            });
        }
    }
}
