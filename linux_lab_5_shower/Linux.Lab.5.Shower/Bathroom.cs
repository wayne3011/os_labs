using System.Diagnostics;
using System.Transactions;

namespace Linux.Lab._5.Shower;

public class Bathroom : IDisposable
{
    private int _personCount = 0;
    private object enterLocker = new();
    private object leaveLocker = new();
    public int Capacity { get; }
    private readonly Semaphore _bathRoomSemaphore;
    private Gender _currentGender = Gender.None;
    private readonly Semaphore _genderSemaphore = new Semaphore(0, 1);
    private Stopwatch _time = new();

    public Bathroom(int capacity)
    {
        _bathRoomSemaphore =  new Semaphore(5, capacity);
        Capacity = capacity;
        _time.Start();
    }


    public void Enter(Person person)
    {
        lock (enterLocker)
        {
            if (_currentGender == Gender.None) _currentGender = person.Gender;
        }
        if (_currentGender != person.Gender)
        {
            _genderSemaphore.WaitOne();
            _currentGender = person.Gender;
        }


        _bathRoomSemaphore.WaitOne();
        // if (person.Gender == Gender.Men)
        // {
        //     _menSemaphore.WaitOne();
        // }
        // else
        // {
        //     _womenSemaphore.WaitOne();
        // }
        Console.WriteLine($"{person.Name} enter the bathroom in {_time.ElapsedMilliseconds}");
        _personCount++;
        
        Thread.Sleep(1000);

        lock (leaveLocker)
        {
            _personCount--;
            if (_personCount == 0)
            {
                // if (person.Gender == Gender.Men)
                // {
                //     _menSemaphore.Release();
                // }
                // else
                // {
                //     _womenSemaphore.Release();
                // }
                _genderSemaphore.Release();
            }
        }

        _bathRoomSemaphore.Release();
        Console.WriteLine($"{person.Name} leave the bathroom in {_time.ElapsedMilliseconds}");
    }

    public void Dispose()
    {
        _time.Stop();
    }
}