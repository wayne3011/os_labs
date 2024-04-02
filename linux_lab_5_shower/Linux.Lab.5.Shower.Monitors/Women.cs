namespace Linux.Lab._5.Shower;

public class Women : Person
{
    public Women(string name) : base(name, Gender.Women)
    {
    }

    public override async Task WashUp(UnisexBathroom bathroom)
    {
        Console.WriteLine($"{Name} want enter in bathroom. In {DateTime.Now.Millisecond}:{DateTime.Now.Nanosecond}");
        bathroom.FemaleEnter();
        Console.WriteLine($"{Name} enter in bathroom. In {DateTime.Now.Millisecond}:{DateTime.Now.Nanosecond}");
        await Task.Delay(1000 * Random.Shared.Next(1,5));
        bathroom.FemaleExit();
        Console.WriteLine($"{Name} leave bathroom. In {DateTime.Now.Millisecond}:{DateTime.Now.Nanosecond}");
    }
}