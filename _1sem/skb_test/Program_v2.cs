using System;
using System.Collections.Generic;

namespace EmptyProject
{
    internal class Program
    {
        private class Clone
        {
            private static LinkedList<int> _knowledges;
            private LinkedListNode<int> _current;

            public Clone(LinkedList<int> knowledges, LinkedListNode<int> current)
            {
                _knowledges = knowledges;
                _current = current;
            }

            public void Learn(int p)
            {
                _knowledges.AddAfter(_current, new LinkedListNode<int>(p));
                _current = _current.Next;
                while (_current?.Next != null)
                    _knowledges.RemoveLast();
            }

            public void Rollback() => _current = _current.Previous;
            public void Relearn() => _current = _current.Next;
            public override string ToString() => _current.Value == 0 ? "basic" : _current.Value.ToString();

            public Clone Copy()
            {
                var newKnowledges = new LinkedList<int>();
                foreach (var node in _knowledges)
                    newKnowledges.AddLast(new LinkedListNode<int>(node));
                return new Clone(newKnowledges, newKnowledges.Find(_current.Value));
            }
        }

        public static void Main()
        {
            string line;
            if ((line = Console.ReadLine()) == null) return;
            int n = int.Parse(line.Split(' ')[0]), j = 1, i = 0;

            var baseKnowledges = new LinkedList<int>(new[] {0});
            var cvs = new Dictionary<int, Clone> {[j] = new Clone(baseKnowledges, baseKnowledges.First)};

            var runcmd = new Dictionary<string, Action<int, int>>
            {
                ["learn"] = (k, l) => cvs[k].Learn(l),
                ["rollback"] = (k, l) => cvs[k].Rollback(),
                ["relearn"] = (k, l) => cvs[k].Relearn(),
                ["check"] = (k, l) => Console.WriteLine(cvs[k].ToString()),
                ["clone"] = (k, l) => cvs.Add(++j, cvs[k].Copy())
            };

            while (i++ < n)
            {
                if ((line = Console.ReadLine()) == null) continue;
                var cmd = line.Split(' ');
                runcmd[cmd[0]](int.Parse(cmd[1]), cmd.Length > 2 ? int.Parse(cmd[2]) : 0);
            }
        }
    }
}