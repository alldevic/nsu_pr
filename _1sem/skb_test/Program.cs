using System;
using System.Collections.Generic;

namespace EmptyProject
{
    internal class Program
    {
        private class Clone
        {
            private readonly LinkedList<int> _knowledges;
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
                while (_current != null && _current.Next != null)
                    _knowledges.RemoveLast();
            }

            public void Rollback()
            {
                _current = _current.Previous;
            }

            public void Relearn()
            {
                _current = _current.Next;
            }

            public void Check()
            {
                if (_current.Value == 0)
                    Console.WriteLine("basic");
                else
                    Console.WriteLine(_current.Value);
            }

            public Clone Copy()
            {
                var newKnowledges = new LinkedList<int>();
                foreach (var node in _knowledges)
                    newKnowledges.AddLast(new LinkedListNode<int>(node));
                var newCurrent = newKnowledges.Find(_current.Value);
                return new Clone(newKnowledges, newCurrent);
            }
        }

        private static void Main()
        {
            var line = Console.ReadLine();
            if (line == null) return;
            var h = line.Split(' ');
            int n = int.Parse(h[0]), j = 1;
            var cvs = new Dictionary<int, Clone>();
            var firstKnowledges = new LinkedList<int>();
            firstKnowledges.AddFirst(new LinkedListNode<int>(0));
            cvs.Add(j, new Clone(firstKnowledges, firstKnowledges.First));
            for (var i = 0; i < n; i++)
            {
                var readLine = Console.ReadLine();
                if (readLine == null) continue;
                var cmd = readLine.Split(' ');
                var k = int.Parse(cmd[1]);
                switch (cmd[0])
                {
                    case "learn":
                        cvs[k].Learn(int.Parse(cmd[2]));
                        break;
                    case "rollback":
                        cvs[k].Rollback();
                        break;
                    case "relearn":
                        cvs[k].Relearn();
                        break;
                    case "check":
                        cvs[k].Check();
                        break;
                    default:
                        cvs.Add(++j, cvs[k].Copy());
                        break;
                }
            }
        }
    }
}