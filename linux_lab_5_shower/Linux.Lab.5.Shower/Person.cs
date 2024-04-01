namespace Linux.Lab._5.Shower;

public class Person
{
    public Gender Gender { get; }
    public string Name { get; }

    public Person(Gender gender, string name)
    {
        Gender = gender;
        Name = name;
    }

    
}