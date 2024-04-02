using Linux.Lab._5.Shower;
using System;
using System.Threading;

public class UnisexBathroom
{
    private readonly object mutex = new object();
    private int maleCount = 0;
    private int femaleCount = 0;

    public void MaleEnter()
    {
        lock (mutex)
        {
            if (femaleCount > 0)
            {
                Monitor.Wait(mutex);
            }
            maleCount++;
        }
    }

    public void MaleExit()
    {
        lock (mutex)
        {
            maleCount--;
            if (maleCount == 0)
            {
                Monitor.PulseAll(mutex);
            }
        }
    }

    public void FemaleEnter()
    {
        lock (mutex)
        {
            if (maleCount > 0)
            {
                Monitor.Wait(mutex);
            }
            femaleCount++;
        }
    }

    public void FemaleExit()
    {
        lock (mutex)
        {
            femaleCount--;
            if (femaleCount == 0)
            {
                Monitor.PulseAll(mutex);
            }
        }
    }
}

class Program
{
    static UnisexBathroom bathroom = new UnisexBathroom();

    static async Task Main(string[] args)
    {
        List<Person> persons = new()
        {
            new Men("Паша"),
            new Men("Игорь"),
            new Women("Софа"),
            new Women("Жанна"),
            new Men("Алексей")
        };
        List<Task> tasks = new List<Task>();
        foreach (var person in persons)
        {
            tasks.Add(Task.Run(() => person.WashUp(bathroom)));
        }

        await Task.WhenAll(tasks);
    }
}