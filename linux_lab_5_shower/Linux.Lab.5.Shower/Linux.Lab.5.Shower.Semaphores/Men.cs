namespace Linux.Lab._5.Shower;

public class Men : Person
{
    public Men(string name) : base(name, Gender.Men)
    {
        
    }
    public override async Task WashUp(UnisexBathroom bathroom)
    {
        Console.WriteLine($"{Name} want enter in bathroom. In {DateTime.Now.Millisecond}:{DateTime.Now.Nanosecond}");
        bathroom.MaleEnter();
        Console.WriteLine($"{Name} enter in bathroom. In {DateTime.Now.Millisecond}:{DateTime.Now.Nanosecond}");
        await Task.Delay(10000);
        bathroom.MaleExit();
        Console.WriteLine($"{Name} leave bathroom. In {DateTime.Now.Millisecond}:{DateTime.Now.Nanosecond}");
    }
}