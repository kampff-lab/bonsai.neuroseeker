using Bonsai;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Linq;
using System.Reactive.Subjects;
using System.Text;

// TODO: replace this with the source output type.
namespace Bonsai.NeuroSeeker
{
    public class SelectedChannel : Source<int[]>
    {
        public static Subject<int> Channel = new Subject<int>();

        public override IObservable<int[]> Generate()
        {
            // TODO: generate the observable sequence.
            return Channel.Select(x => new[] { x });
        }
    }
}
