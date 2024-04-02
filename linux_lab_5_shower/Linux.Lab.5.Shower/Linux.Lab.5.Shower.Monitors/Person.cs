namespace Linux.Lab._5.Shower;

public abstract class Person
{
    public string Name { get; }
    public Gender Gender { get; }
    
    protected Person(string name, Gender gender)
    {
        Name = name;
        Gender = gender;
    }
    public abstract Task WashUp(UnisexBathroom bathroom);
    


}