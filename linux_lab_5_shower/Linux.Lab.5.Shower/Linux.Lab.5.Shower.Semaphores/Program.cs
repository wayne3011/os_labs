using Linux.Lab._5.Shower;
using System;
using System.Threading;

public class UnisexBathroom
{
    Semaphore maleSem, femaleSem;
    Semaphore mutex;
    int maleCount, femaleCount;

    public UnisexBathroom()
    {
        maleSem = new Semaphore(1, 1);
        femaleSem = new Semaphore(1, 1);
        mutex = new Semaphore(1, 1);
        maleCount = 0;
        femaleCount = 0;
    }

    public void MaleEnter()
    {
        femaleSem.WaitOne();
        femaleSem.Release();
        mutex.WaitOne();
        maleCount++;
        if (maleCount == 1)
            maleSem.WaitOne();


        mutex.Release();

        //maleSem.WaitOne();
    }

    public void MaleExit()
    {
        mutex.WaitOne();
        maleCount--;
        if (maleCount == 0)
            maleSem.Release();
        mutex.Release();

        //maleSem.Release();
    }

    public void FemaleEnter()
    {
        maleSem.WaitOne();
        maleSem.Release();
        mutex.WaitOne();

        femaleCount++;
        if (femaleCount == 1)
            femaleSem.WaitOne();
        mutex.Release();

        //femaleSem.WaitOne();
    }

    public void FemaleExit()
    {
        mutex.WaitOne();
        femaleCount--;
        if (femaleCount == 0)
            femaleSem.Release();
        mutex.Release();

        //femaleSem.Release();
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