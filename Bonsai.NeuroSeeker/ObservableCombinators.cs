﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Bonsai.NeuroSeeker
{
    static class ObservableCombinators
    {
        public static IObservable<TResult> CombineEither<TSource1, TSource2, TResult>(
            this IObservable<TSource1> first,
            IObservable<TSource2> second,
            Func<TSource1, TSource2, TResult> resultSelector)
        {
            return first.Publish(ps1 => second.Publish(ps2 =>
                ps1.CombineLatest(ps2, resultSelector)
                   .TakeUntil(ps1.LastOrDefaultAsync())
                   .TakeUntil(ps2.LastOrDefaultAsync())));
        }
    }
}
